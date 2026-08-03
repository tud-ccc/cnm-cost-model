"""Lowers `cnm` dialect IR into `llvcnm`, the target-agnostic virtual assembly
described in the CoMoNM paper (hardware_summary/CoMoNM___TACO,
contents/costmodelInputs.tex Sec 3.3 "CNM virtual assembly code generation",
and figures/background/simple_asm.asm/mv_asm_in.asm/mv_asm_out.asm).

Typed pseudo-instructions over virtual registers: LOAD_<TYPE>, STORE_<TYPE>,
<OP>_<TYPE> for compute, INC for pointer/counter bumps, and JUMP/JNEQ/ADDJ
for control flow. Real UPMEM assembly fuses a trailing ALU op directly into
the following branch (see DPUMacroFusion.cpp) and expands software-emulated
ops (64-bit add/mul, float ops) into many real instructions -- neither is
reproduced here; those are real-ISA-specific facts that belong in a later,
per-target backend (and in the cost model's calibrated LUTs, which already
account for them empirically), not in this target-agnostic virtual assembly.
This is why the output is "close, not exact": same instruction shape and
loop structure as real -O3 output, without target-specific fusion/expansion.

This module only walks a `cnm.kernel` and prints llvcnm text. It has no
dependency on Predictor and isn't wired into it.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from xdsl.dialects.arith import ConstantOp
from xdsl.dialects.builtin import Float32Type, Float64Type, IndexType, IntegerAttr, IntegerType
from xdsl.ir import Block, Operation, SSAValue

from dialects.cnm import (
    AllocOp,
    BarrierOp,
    ComputeOp,
    DmaLoadOp,
    DmaStoreOp,
    ForOp,
    IfOp,
    KernelOp,
    LoadOp,
    OperandOp,
    OutputOp,
    ReturnOp,
    StoreOp,
    YieldOp,
)


def type_suffix(ty) -> str:
    """Maps a CNM/xDSL element type to llvcnm's type suffix, e.g. i32 -> "I32",
    f64 -> "F64". Falls back to the type's own repr for anything unexpected
    rather than guessing -- an unhandled type should be visible, not silently
    mislabeled."""
    if isinstance(ty, IntegerType):
        return f"I{ty.width.data}"
    if isinstance(ty, Float32Type):
        return "F32"
    if isinstance(ty, Float64Type):
        return "F64"
    if isinstance(ty, IndexType):
        return "IDX"
    return str(ty).upper()


@dataclass
class _Emitter:
    lines: list[str] = field(default_factory=list)
    _vreg_names: dict[SSAValue, str] = field(default_factory=dict)
    _vreg_counter: int = 0
    _label_counter: int = 0
    _operand_names: dict[SSAValue, str] = field(default_factory=dict)
    _addr_cache: dict[tuple, str] = field(default_factory=dict)
    _pointer_indvars: set = field(default_factory=set)

    def vreg(self, value: SSAValue) -> str:
        """Assigns (or looks up) this SSA value's virtual register name.
        One real xDSL SSA value maps to exactly one llvcnm virtual register --
        no attempt to do real-ISA-style register allocation/reuse, since
        llvcnm registers are virtual and unlimited, matching the paper's own
        "operands are virtual, not used for functional correctness" note."""
        if value not in self._vreg_names:
            name = self._operand_names.get(value)
            if name is None:
                name = f"V{self._vreg_counter}"
                self._vreg_counter += 1
            self._vreg_names[value] = name
        return self._vreg_names[value]

    def fresh_vreg(self) -> str:
        """A throwaway virtual register not tied to any SSA value -- for
        address-scale instructions (see `_emit_addr_scale`) whose result
        nothing downstream references, the same way real UPMEM code
        computes a byte address into a register it only uses once."""
        name = f"V{self._vreg_counter}"
        self._vreg_counter += 1
        return name

    def fresh_label(self, hint: str) -> str:
        self._label_counter += 1
        return f".L{hint}{self._label_counter}"

    def emit(self, text: str) -> None:
        self.lines.append(text)

    def emit_label(self, label: str) -> None:
        self.lines.append(f"{label}:")


def _element_type(memref_type) -> object:
    return memref_type.element_type


def _emit_addr_scale(em: _Emitter, memref, indices) -> None:
    """Real UPMEM has no index-based load/store addressing mode -- every
    array access needs an explicit index-to-byte-address computation first
    (`lsl_add`). One `SCALE_IDX` per index dimension: a single shift for a
    1-D access, one extra combining step per additional dimension for a
    multi-dimensional one (e.g. Gemm's `cache_b[k, col]`).

    CSE'd per (memref, index) pair within one kernel body: a real in-place
    read-modify-write (e.g. Scan's load then store at the same address)
    computes the address once and reuses it for both, matching real
    common-subexpression elimination.

    Skipped entirely when `idx` is a "pointer"-addressing loop's own
    induction variable (see ForOp's docstring in dialects/cnm.py and
    `_lower_for` below): that idiom bumps a dedicated pointer register per
    array instead of scaling an index, so the access is already a direct,
    zero-offset load/store off that bumped pointer."""
    for idx in indices:
        if idx in em._pointer_indvars:
            continue
        key = (memref, idx)
        if key in em._addr_cache:
            continue
        scale_reg = em.fresh_vreg()
        em.emit(f"SCALE_IDX({scale_reg}, {em.vreg(idx)})  ; index -> byte address")
        em._addr_cache[key] = scale_reg


def lower_kernel(kernel: KernelOp) -> str:
    """Entry point: returns the llvcnm text for one `cnm.kernel`."""
    em = _Emitter()
    em.emit(f"; kernel {kernel.sym_name.data}")

    # Operand/output/local-buffer declarations aren't instructions in real
    # assembly either -- they just name memory that later loads/stores/DMAs
    # address (mem_alloc() calls happen once at kernel entry too, not per
    # chunk). Emitted as comments purely for readability of the listing.
    for op in kernel.body.block.walk():
        if isinstance(op, OperandOp | OutputOp):
            reg = em.vreg(op.res)
            kind = "in" if isinstance(op, OperandOp) else "out"
            em.emit(f"; {kind} {op.sym_name.data} -> {reg} : {op.res.type}")
        elif isinstance(op, AllocOp):
            reg = em.vreg(op.res)
            em.emit(f"; local {reg} : {op.res.type}")

    _lower_block(kernel.body.block, em)
    return "\n".join(em.lines) + "\n"


def _lower_block(block: Block, em: _Emitter) -> None:
    for op in block.ops:
        _lower_op(op, em)


def _lower_op(op: Operation, em: _Emitter) -> None:
    if isinstance(op, OperandOp | OutputOp | AllocOp):
        return  # already emitted as a header comment in lower_kernel

    if isinstance(op, DmaLoadOp):
        dst = em.vreg(op.dst)
        src = em.vreg(op.src)
        off = em.vreg(op.src_offset)
        suffix = type_suffix(_element_type(op.src.type))
        em.emit(f"DMA_LOAD_{suffix}({dst}, {src}[{off}], {op.size.value.data})")
        return

    if isinstance(op, DmaStoreOp):
        src = em.vreg(op.src)
        dst = em.vreg(op.dst)
        off = em.vreg(op.dst_offset)
        suffix = type_suffix(_element_type(op.dst.type))
        em.emit(f"DMA_STORE_{suffix}({src}, {dst}[{off}], {op.size.value.data})")
        return

    if isinstance(op, LoadOp):
        _emit_addr_scale(em, op.memref, op.indices)
        dst = em.vreg(op.res)
        addr = em.vreg(op.memref)
        idx = ", ".join(em.vreg(i) for i in op.indices)
        suffix = type_suffix(_element_type(op.memref.type))
        em.emit(f"LOAD_{suffix}({dst}, {addr}[{idx}])  ; level={op.level.data}")
        return

    if isinstance(op, StoreOp):
        _emit_addr_scale(em, op.memref, op.indices)
        val = em.vreg(op.value)
        addr = em.vreg(op.memref)
        idx = ", ".join(em.vreg(i) for i in op.indices)
        suffix = type_suffix(_element_type(op.memref.type))
        em.emit(f"STORE_{suffix}({val}, {addr}[{idx}])  ; level={op.level.data}")
        return

    if isinstance(op, ComputeOp):
        dst = em.vreg(op.res)
        srcs = ", ".join(em.vreg(i) for i in op.inputs)
        suffix = type_suffix(op.res.type)
        em.emit(f"{op.kind.data.upper()}_{suffix}({dst}, {srcs})")
        return

    if isinstance(op, ForOp):
        _lower_for(op, em)
        return

    if isinstance(op, IfOp):
        _lower_if(op, em)
        return

    if isinstance(op, BarrierOp):
        # Cost is NOT a fixed per-instruction latency (see BarrierOp's
        # docstring) -- this just marks where the barrier happens; whatever
        # lowers this further down (e.g. Predictor/kernels/Scan_xdsl.py)
        # supplies the real, thread-count-dependent cost from the barrier
        # LUT, not a number baked in here.
        em.emit("BARRIER()")
        return

    if isinstance(op, ConstantOp):
        # Loop bounds and other compile-time-known values arrive as ordinary
        # `arith.constant`s (not a `cnm` op -- cnm.for's bound is a plain SSA
        # operand so genuinely dynamic bounds are representable too), so
        # this is the one non-cnm op the emitter has to know about. Lowers
        # to a load-immediate.
        dst = em.vreg(op.result)
        value = op.value
        assert isinstance(value, IntegerAttr)
        em.emit(f"MOVE_{type_suffix(op.result.type)}({dst}, {value.value.data})")
        return

    if isinstance(op, ReturnOp):
        outs = ", ".join(em.vreg(a) for a in op.arguments)
        em.emit(f"RET({outs})" if outs else "RET")
        return

    if isinstance(op, YieldOp):
        # Handled by the enclosing ForOp (loop-carried values need to be
        # moved into their carrying registers before the branch back to the
        # loop header -- see _lower_for), so there's nothing to do here
        # standalone.
        return

    em.emit(f"; <unhandled op {op.name}>")


def _lower_for(op: ForOp, em: _Emitter) -> None:
    header = em.fresh_label("loop")
    exit_label = em.fresh_label("end")

    indvar, *carried_args = op.body.block.args
    ivreg = em.vreg(indvar)
    ub_reg = em.vreg(op.ub)

    # "pointer" loops (see ForOp's docstring): every array this loop's own
    # induction variable directly indexes gets its own bumped pointer
    # register instead of a per-access address computation, and the
    # trip-count decrement fuses into the closing branch (DPUMacroFusion.cpp).
    pointer_mode = op.addressing.data == "pointer"
    bumped_memrefs: list[SSAValue] = []
    if pointer_mode:
        seen: set = set()
        for inner in op.body.block.walk():
            if isinstance(inner, LoadOp | StoreOp) and list(inner.indices) == [indvar]:
                if inner.memref not in seen:
                    seen.add(inner.memref)
                    bumped_memrefs.append(inner.memref)
        em._pointer_indvars.add(indvar)

    # Loop-carried values (e.g. a running reduction accumulator) start out
    # as the operands passed into cnm.for; alias each carried block argument
    # to the SAME virtual register as its initial value, then re-point it at
    # the yielded value's register at the bottom of the loop, mirroring how
    # a real backend keeps a reduction in one physical register across
    # iterations instead of renaming it every trip.
    #
    # Only safe when init_val has no OTHER use: if a hoisted shared constant
    # (e.g. a reusable `zero_idx`) is used as this ForOp's initial value AND
    # separately elsewhere in the kernel, aliasing it here would let the
    # loop's own mutations of that register corrupt the other, unrelated
    # use once the loop has run. Falls back to a fresh register + explicit
    # MOVE when init_val is shared, exactly like a real register allocator
    # would refuse to clobber a still-live value.
    for block_arg, init_val in zip(carried_args, op.iter_args, strict=True):
        if init_val.uses.get_length() == 1:
            em._vreg_names[block_arg] = em.vreg(init_val)
        else:
            fresh = em.fresh_vreg()
            em.emit(f"MOVE_{type_suffix(block_arg.type)}({fresh}, {em.vreg(init_val)})")
            em._vreg_names[block_arg] = fresh

    # Register coalescing: if the yielded value is used NOWHERE else (its
    # only use is this yield), the op that produces it can just target the
    # carried register directly instead of a fresh one -- eliding the
    # closing MOVE below entirely. Mirrors what a real register allocator
    # does for an in-place accumulator (`add r1, r7, r1` targets the same
    # register it read, real ReduceOp assembly does this on every add, not
    # a fresh destination + copy). Only safe for single-use values: if the
    # result is ALSO read elsewhere in the body (e.g. Scan's `new_running`,
    # both stored and yielded), coalescing it early could rename that other
    # use too, so this leaves those to the explicit MOVE below instead.
    yield_op = op.body.block.last_op
    assert isinstance(yield_op, YieldOp)
    for block_arg, yielded_val in zip(carried_args, yield_op.arguments, strict=True):
        if yielded_val.uses.get_length() == 1:
            em._vreg_names[yielded_val] = em.vreg(block_arg)

    em.emit(f"MOVE_IDX({ivreg}, 0)")
    em.emit_label(header)
    _lower_block(op.body.block, em)

    for block_arg, result_val in zip(carried_args, yield_op.arguments, strict=True):
        result_reg = em.vreg(result_val)
        carried_reg = em.vreg(block_arg)
        if result_reg != carried_reg:
            em.emit(f"MOVE_{type_suffix(block_arg.type)}({carried_reg}, {result_reg})")

    if pointer_mode:
        for memref in bumped_memrefs:
            reg = em.vreg(memref)
            em.emit(f"ADD_IDX({reg}, {reg}, 1)  ; pointer bump")
        em._pointer_indvars.discard(indvar)

    if op.fuse_branch.value.data != 0:
        em.emit(f"INCJNEQ({ivreg}, {ub_reg}, {header})  ; fused trip-decrement+branch")
    else:
        em.emit(f"INC({ivreg})")
        em.emit(f"JNEQ({ivreg}, {ub_reg}, {header})")
    em.emit_label(exit_label)

    for res_val, block_arg in zip(op.res, carried_args, strict=True):
        em._vreg_names[res_val] = em.vreg(block_arg)


def _lower_if(op: IfOp, em: _Emitter) -> None:
    """`cond` is an i1 virtual register -- real UPMEM has no separate boolean
    registers (comparisons fuse directly into a branch, see
    DPUMacroFusion.cpp), but llvcnm is target-agnostic, so a plain
    JEQ(cond, 0, ...) test on the (virtual) i1 value is the closest faithful
    equivalent without hardcoding real-ISA fusion here."""
    cond = em.vreg(op.cond)
    has_else = len(op.false_region.blocks) > 0
    else_label = em.fresh_label("else")
    end_label = em.fresh_label("endif")

    em.emit(f"JEQ({cond}, 0, {else_label if has_else else end_label})")
    _lower_block(op.true_region.block, em)
    true_yield = op.true_region.block.last_op
    assert isinstance(true_yield, YieldOp)
    for res_val, yielded in zip(op.res, true_yield.arguments, strict=True):
        res_reg = em.vreg(res_val)
        y_reg = em.vreg(yielded)
        if res_reg != y_reg:
            em.emit(f"MOVE_{type_suffix(res_val.type)}({res_reg}, {y_reg})")

    if has_else:
        em.emit(f"JUMP({end_label})")
        em.emit_label(else_label)
        _lower_block(op.false_region.block, em)
        false_yield = op.false_region.block.last_op
        assert isinstance(false_yield, YieldOp)
        for res_val, yielded in zip(op.res, false_yield.arguments, strict=True):
            res_reg = em.vreg(res_val)
            y_reg = em.vreg(yielded)
            if res_reg != y_reg:
                em.emit(f"MOVE_{type_suffix(res_val.type)}({res_reg}, {y_reg})")

    em.emit_label(end_label)
