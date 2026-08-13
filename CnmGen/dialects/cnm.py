"""The `cnm` dialect: an xDSL dialect representing CoMoNM's CNM IR."""

from __future__ import annotations

from collections.abc import Sequence

from xdsl.dialects.builtin import IndexType, IntegerAttr, IntegerType, MemRefType, StringAttr
from xdsl.ir import Attribute, Dialect, Operation, Region, SSAValue
from xdsl.irdl import (
    AnyAttr,
    IRDLOperation,
    irdl_op_definition,
    lazy_traits_def,
    operand_def,
    prop_def,
    region_def,
    result_def,
    traits_def,
    var_operand_def,
    var_result_def,
)
from xdsl.traits import HasParent, IsTerminator, SingleBlockImplicitTerminator
from xdsl.utils.exceptions import VerifyException


@irdl_op_definition
class OperandOp(IRDLOperation):
    """Declares a kernel input operand -- CNM IR's `Operand(name, TENSOR, DTYPE)`.
    The result's MemRefType carries both "is this an array or a scalar"
    (via its shape: empty shape == scalar) and DTYPE (its element type)."""

    name = "cnm.operand"

    sym_name = prop_def(StringAttr)
    res = result_def(MemRefType)

    assembly_format = "$sym_name attr-dict `:` type($res)"

    def __init__(self, sym_name: str, result_type: MemRefType) -> None:
        super().__init__(
            result_types=[result_type],
            properties={"sym_name": StringAttr(sym_name)},
        )


@irdl_op_definition
class OutputOp(IRDLOperation):
    """Declares a kernel output -- CNM IR's `Output(name, TENSOR, DTYPE)`.
    Kept distinct from OperandOp (rather than an `is_output` flag on one op)
    since the paper itself treats inputs and outputs as separate node kinds."""

    name = "cnm.output"

    sym_name = prop_def(StringAttr)
    res = result_def(MemRefType)

    assembly_format = "$sym_name attr-dict `:` type($res)"

    def __init__(self, sym_name: str, result_type: MemRefType) -> None:
        super().__init__(
            result_types=[result_type],
            properties={"sym_name": StringAttr(sym_name)},
        )


@irdl_op_definition
class LoadOp(IRDLOperation):
    """A memory read -- CNM IR's `MEM_OP(operand, level, LOAD)`. `level` names
    the memory hierarchy level being read (e.g. "MRAM"/"WRAM" on UPMEM)."""

    name = "cnm.load"

    level = prop_def(StringAttr)
    memref = operand_def(MemRefType)
    indices = var_operand_def(IndexType)
    res = result_def(AnyAttr())

    assembly_format = (
        "$memref `[` $indices `]` `level` $level attr-dict "
        "`:` type($memref) `->` type($res)"
    )

    def __init__(
        self,
        memref: SSAValue | Operation,
        indices: Sequence[SSAValue | Operation],
        level: str,
        result_type: Attribute,
    ) -> None:
        super().__init__(
            operands=[memref, indices],
            result_types=[result_type],
            properties={"level": StringAttr(level)},
        )

    def verify_(self) -> None:
        memref_type = self.memref.type
        if not isinstance(memref_type, MemRefType):
            raise VerifyException("expected a memref type")
        if memref_type.get_num_dims() != len(self.indices):
            raise VerifyException("expected one index per memref dimension")


@irdl_op_definition
class StoreOp(IRDLOperation):
    """A memory write -- CNM IR's `MEM_OP(operand, level, STORE)`."""

    name = "cnm.store"

    level = prop_def(StringAttr)
    value = operand_def(AnyAttr())
    memref = operand_def(MemRefType)
    indices = var_operand_def(IndexType)

    assembly_format = (
        "$value `,` $memref `[` $indices `]` `level` $level attr-dict "
        "`:` type($value) `,` type($memref)"
    )

    def __init__(
        self,
        value: SSAValue | Operation,
        memref: SSAValue | Operation,
        indices: Sequence[SSAValue | Operation],
        level: str,
    ) -> None:
        super().__init__(
            operands=[value, memref, indices],
            properties={"level": StringAttr(level)},
        )

    def verify_(self) -> None:
        memref_type = self.memref.type
        if not isinstance(memref_type, MemRefType):
            raise VerifyException("expected a memref type")
        if memref_type.get_num_dims() != len(self.indices):
            raise VerifyException("expected one index per memref dimension")


@irdl_op_definition
class AllocOp(IRDLOperation):
    """Declares a local scratch buffer (UPMEM's `mem_alloc()`). `cnm.load`/
    `cnm.store` operate on buffers like this one, while bulk transfers to/from
    the kernel's MRAM-level operands go through `cnm.dma_load`/`cnm.dma_store`."""

    name = "cnm.alloc"

    res = result_def(MemRefType)

    assembly_format = "attr-dict `:` type($res)"

    def __init__(self, result_type: MemRefType) -> None:
        super().__init__(result_types=[result_type])


@irdl_op_definition
class DmaLoadOp(IRDLOperation):
    """A bulk MRAM->WRAM transfer -- copies `size` contiguous elements
    starting at `src_offset` in `src` (a kernel operand) into `dst` (a local
    buffer from `cnm.alloc`), all at once. UPMEM's `ldma`/`mram_read`."""

    name = "cnm.dma_load"

    src = operand_def(MemRefType)
    src_offset = operand_def(IndexType)
    dst = operand_def(MemRefType)
    size = prop_def(IntegerAttr)

    assembly_format = (
        "$src `[` $src_offset `]` `,` $dst `size` $size attr-dict "
        "`:` type($src) `,` type($dst)"
    )

    def __init__(
        self,
        src: SSAValue | Operation,
        src_offset: SSAValue | Operation,
        dst: SSAValue | Operation,
        size: int,
    ) -> None:
        super().__init__(
            operands=[src, src_offset, dst],
            properties={"size": IntegerAttr(size, 64)},
        )


@irdl_op_definition
class DmaStoreOp(IRDLOperation):
    """The reverse of `cnm.dma_load`: a bulk WRAM->MRAM transfer, copying
    `size` contiguous elements from `src` (a local buffer) into `dst` (a
    kernel operand) starting at `dst_offset` -- UPMEM's `sdma`/`mram_write`."""

    name = "cnm.dma_store"

    src = operand_def(MemRefType)
    dst = operand_def(MemRefType)
    dst_offset = operand_def(IndexType)
    size = prop_def(IntegerAttr)

    assembly_format = (
        "$src `,` $dst `[` $dst_offset `]` `size` $size attr-dict "
        "`:` type($src) `,` type($dst)"
    )

    def __init__(
        self,
        src: SSAValue | Operation,
        dst: SSAValue | Operation,
        dst_offset: SSAValue | Operation,
        size: int,
    ) -> None:
        super().__init__(
            operands=[src, dst, dst_offset],
            properties={"size": IntegerAttr(size, 64)},
        )


@irdl_op_definition
class ComputeOp(IRDLOperation):
    """A compute operation -- CNM IR's `Operation(kind, [operands], result,
    DTYPE)`. One generic op parametrized by `kind` (e.g. "add", "mul", "sub"),
    matching the paper's own single-node-type choice, rather than a separate
    xDSL op per arithmetic kind."""

    name = "cnm.compute"

    kind = prop_def(StringAttr)
    inputs = var_operand_def(AnyAttr())
    res = result_def(AnyAttr())

    assembly_format = "$kind `(` $inputs `)` attr-dict `:` functional-type($inputs, $res)"

    def __init__(
        self,
        kind: str,
        inputs: Sequence[SSAValue | Operation],
        result_type: Attribute,
    ) -> None:
        super().__init__(
            operands=[inputs],
            result_types=[result_type],
            properties={"kind": StringAttr(kind)},
        )


@irdl_op_definition
class BarrierOp(IRDLOperation):
    """A cross-tasklet synchronization point -- UPMEM's `barrier_wait()`. No
    operands/results: it just blocks every tasklet until all have arrived.
    Marks where a barrier happens in the instruction sequence; its cost is
    supplied by the lowering/prediction stage (Predictor/load_lut.py's
    barrier cost model), not baked into the dialect."""

    name = "cnm.barrier"

    assembly_format = "attr-dict"

    def __init__(self) -> None:
        super().__init__()


@irdl_op_definition
class YieldOp(IRDLOperation):
    """Terminates a `cnm.for` body, optionally carrying this iteration's
    updated loop-carried values (e.g. a running reduction accumulator) --
    the same role as `scf.yield`."""

    name = "cnm.yield"

    arguments = var_operand_def(AnyAttr())

    # ForOp/IfOp are defined later in this file -- lazy_traits_def defers
    # looking them up until traits are actually needed, sidestepping the
    # forward reference (same pattern scf.YieldOp uses for scf.ForOp/IfOp).
    traits = lazy_traits_def(lambda: (IsTerminator(), HasParent(ForOp, IfOp)))

    assembly_format = "($arguments^ `:` type($arguments))? attr-dict"

    def __init__(self, arguments: Sequence[SSAValue | Operation] = ()) -> None:
        super().__init__(operands=[arguments])


@irdl_op_definition
class IfOp(IRDLOperation):
    """A conditional, mirroring `scf.if`: `true_region` always runs when
    `cond` holds, `false_region` always runs otherwise (empty by default).
    Both are single-block regions terminated by `cnm.yield` and must yield
    the same types."""

    name = "cnm.if"

    cond = operand_def(IntegerType(1))  # i1 boolean condition, same as scf.if

    res = var_result_def(AnyAttr())
    true_region = region_def("single_block")
    false_region = region_def()

    traits = traits_def(SingleBlockImplicitTerminator(YieldOp))

    def __init__(
        self,
        cond: SSAValue | Operation,
        result_types: Sequence[Attribute],
        true_region: Region | Sequence[Operation],
        false_region: Region | Sequence[Operation] | None = None,
    ) -> None:
        super().__init__(
            operands=[cond],
            result_types=[result_types],
            regions=[true_region, false_region if false_region is not None else Region()],
        )


@irdl_op_definition
class ForOp(IRDLOperation):
    """A counting loop over `[0, ub)` -- CNM IR's `For([body...], induction_var)`.
    The body is a single-block Region whose entry block argument is the
    induction variable (an `IndexType` value), following `scf.for`.
    `iter_args`/`res` support loop-carried values for reduction loops.

    `addressing` ("index" or "pointer") records which UPMEM -O3 addressing
    idiom this loop's compiled form uses, set by lowering/linalg_to_cnm.py.
    `fuse_branch` independently records whether the trip-count decrement
    fuses into the closing branch as one instruction (DPUMacroFusion.cpp).
    lowering/cnm_to_llvcnm.py's `_lower_for` is where both change emitted
    instructions."""

    name = "cnm.for"

    ub = operand_def(IndexType)
    iter_args = var_operand_def(AnyAttr())
    res = var_result_def(AnyAttr())
    body = region_def("single_block")
    addressing = prop_def(StringAttr)
    fuse_branch = prop_def(IntegerAttr)

    traits = traits_def(SingleBlockImplicitTerminator(YieldOp))

    def __init__(
        self,
        ub: SSAValue | Operation,
        iter_args: Sequence[SSAValue | Operation],
        body: Region | Sequence[Operation],
        addressing: str = "index",
        fuse_branch: bool = False,
    ) -> None:
        super().__init__(
            operands=[ub, iter_args],
            result_types=[[SSAValue.get(a).type for a in iter_args]],
            regions=[body],
            properties={
                "addressing": StringAttr(addressing),
                # i8, not i1: IntegerAttr(1, 1) wraps to -1 in i1's signed
                # representation (still truthy, but confusing to inspect) --
                # i8 stores 0/1 as written.
                "fuse_branch": IntegerAttr(1 if fuse_branch else 0, 8),
            },
        )

    def verify_(self) -> None:
        if not self.body.block.args:
            raise VerifyException(
                "cnm.for body must have the induction variable as its first block arg"
            )
        indvar, *carried = self.body.block.args
        if not isinstance(indvar.type, IndexType):
            raise VerifyException("induction variable must be of IndexType")
        if len(carried) != len(self.iter_args):
            raise VerifyException(
                f"expected {len(self.iter_args)} loop-carried block args, got {len(carried)}"
            )


@irdl_op_definition
class KernelOp(IRDLOperation):
    """The top-level container for one kernel -- everything the paper's flat
    CNM IR listing describes (operand/output declarations, loops, and the
    final `ret(...)`) lives inside this single-block region."""

    name = "cnm.kernel"

    sym_name = prop_def(StringAttr)
    body = region_def("single_block")

    # ReturnOp is defined after this class, so its terminator trait is
    # resolved lazily too (same pattern as YieldOp/ForOp above).
    traits = lazy_traits_def(lambda: (SingleBlockImplicitTerminator(ReturnOp),))

    def __init__(self, sym_name: str, body: Region | Sequence[Operation]) -> None:
        super().__init__(
            properties={"sym_name": StringAttr(sym_name)},
            regions=[body],
        )


@irdl_op_definition
class ReturnOp(IRDLOperation):
    """Terminates a `cnm.kernel` -- CNM IR's final `ret([...])` statement."""

    name = "cnm.return"

    arguments = var_operand_def(AnyAttr())

    traits = traits_def(IsTerminator(), HasParent(KernelOp))

    assembly_format = "($arguments^ `:` type($arguments))? attr-dict"

    def __init__(self, arguments: Sequence[SSAValue | Operation] = ()) -> None:
        super().__init__(operands=[arguments])


CNM = Dialect(
    "cnm",
    [
        OperandOp,
        OutputOp,
        AllocOp,
        LoadOp,
        StoreOp,
        DmaLoadOp,
        DmaStoreOp,
        ComputeOp,
        IfOp,
        ForOp,
        BarrierOp,
        YieldOp,
        KernelOp,
        ReturnOp,
    ],
    [],
)
