"""Linalg-level (xDSL) representations of the six real DPU kernels this
repo's cost model covers: Gemm, Gemv, ReduceOp, Scan, Sel, Hst.

This is the "front end" input the CoMoNM paper's own pipeline starts from
(hardware_summary/CoMoNM___TACO) -- MLIR-linalg operating on flat, untiled
arrays, before `cnm-gen` introduces the chunk/DMA/WRAM staging that the real
kernels all use (see AsmLoopExtractor's output and cnm/build_vecadd.py's
docstring for that two-level structure). The chunking is a lowering decision
made later by `lowering/linalg_to_cnm.py`, not something linalg itself
expresses -- this mirrors the paper's own figure, which shows `linalg.matvec`
on a flat `tensor<MxNxi32>`/`tensor<Nxi32>` with no tiling in sight
(figures/background/linalg_mv.mlir).

Gemm and Gemv map onto real linalg ops cleanly (`linalg.matmul`, and
`linalg.generic` with the exact indexing maps/iterator types the paper's own
example uses for matvec -- #m1/#m2/#m3, ["parallel","reduction"]).
ReduceOp maps cleanly too, but NOT via `linalg.reduce`: the real kernel
(Predictor/kernels/ReduceOp.py) sums TWO input arrays elementwise before
accumulating (`result += A[c]; result += B[c]`),
and `linalg.reduce` only takes one input operand. This is built as a
`linalg.generic` instead -- two inputs, one reduction iterator, output
indexed by the empty map `(d0) -> ()` (the standard linalg idiom for
"reduce down to a scalar") -- which is still a faithful, non-placeholder
linalg representation, not an opaque marker like Scan/Sel/Hst below.

Scan, Sel and Hst do NOT fit linalg's affine, data-independent indexing
model:
  - Scan's output at position i depends on the output at i-1 (a recurrence),
    not just the input at i -- no affine map expresses "the previous loop
    iteration's result".
  - Sel is stream compaction: which output slot an element lands in depends
    on a runtime predicate over prior elements, not on the loop index.
  - Hst's write address is the data value itself (a scatter), not an affine
    function of the loop index.
For these three, the `linalg.generic` body/indexing-maps are necessarily a
placeholder (documented at each call site below) -- the operation actually
being modeled is identified by the enclosing `func.func`'s name, which
`lowering/linalg_to_cnm.py` dispatches on to emit real, hand-designed `cnm`
IR faithful to each kernel's true behavior. (`library_call` is also set on
those three generics, mirroring real MLIR's own convention for "this body is
not the real semantics, see the named external function" -- redundant with
the func name here, but kept since it's the standard, self-describing way to
flag an opaque linalg.generic.)

Element type is fixed to i32 throughout, matching this repo's uint32_t
benchmark configuration (the one AsmLoopExtractor was validated against).
"""

from __future__ import annotations

from xdsl.builder import Builder
from xdsl.dialects.builtin import (
    AffineMapAttr,
    ModuleOp,
    StringAttr,
    i32,
    MemRefType,
)
from xdsl.dialects.func import FuncOp, ReturnOp
from xdsl.dialects.linalg.attrs import IteratorTypeAttr
from xdsl.dialects.linalg.ops import GenericOp, MatmulOp, YieldOp
from xdsl.dialects.arith import AddiOp, MuliOp
from xdsl.ir import Block, Region
from xdsl.ir.affine import AffineMap
from xdsl.rewriter import InsertPoint


def _func(name: str, arg_types: list, build_body) -> ModuleOp:
    """Builds `func.func @name(args...) { ...build_body(builder, args)... }`
    wrapped in a module, so each kernel is a standalone, printable/parsable
    unit. `build_body` must insert the kernel's linalg op(s) but not the
    trailing `return` -- that's added here uniformly."""
    block = Block(arg_types=arg_types)
    builder = Builder(InsertPoint.at_end(block))
    build_body(builder, block.args)
    builder.insert(ReturnOp())
    func = FuncOp(name, ((*arg_types,), ()), Region(block))
    return ModuleOp([func])


def build_gemm(M: int = 64, N: int = 256, K: int = 256) -> ModuleOp:
    """C[i,j] += A[i,k] * B[k,j] -- the clean case, `linalg.matmul` names it
    directly with no hand-built indexing maps needed.

    N=K=256 (not 64): lowering/linalg_to_cnm.py's _lower_gemm chunks over
    both K and N like the real kernel does, and a default of 64
    (< CHUNK=128) would make both chunking loops degenerate to 0
    iterations -- 256 gives a real, non-trivial 2x2-tile example."""
    a_t = MemRefType(i32, [M, K])
    b_t = MemRefType(i32, [K, N])
    c_t = MemRefType(i32, [M, N])

    def body(b: Builder, args):
        a, b_, c = args
        b.insert(MatmulOp(inputs=[a, b_], outputs=[c]))

    return _func("gemm", [a_t, b_t, c_t], body)


def build_gemv(M: int = 64, N: int = 256) -> ModuleOp:
    """y[i] += A[i,k] * x[k] -- reproduces the paper's own
    figures/background/linalg_mv.mlir verbatim: indexing maps
    #m1=(d0,d1)->(d0,d1) for A, #m2=(d0,d1)->(d1) for x, #m3=(d0,d1)->(d0)
    for y, iterator_types=["parallel","reduction"]. xDSL's linalg dialect has
    no dedicated `linalg.matvec`-with-explicit-maps constructor, so this is
    built as a `linalg.generic`, exactly as the paper's figure shows it.

    N=256 (not 64): lowering/linalg_to_cnm.py's _lower_gemv chunks over N
    like the real kernel does, and a default of 64 (< CHUNK=128) would make
    that chunking loop degenerate to 0 iterations -- 256 gives a real,
    non-trivial 2-chunk example."""
    a_t = MemRefType(i32, [M, N])
    x_t = MemRefType(i32, [N])
    y_t = MemRefType(i32, [M])

    m1 = AffineMapAttr(AffineMap.from_callable(lambda d0, d1: (d0, d1)))
    m2 = AffineMapAttr(AffineMap.from_callable(lambda d0, d1: (d1,)))
    m3 = AffineMapAttr(AffineMap.from_callable(lambda d0, d1: (d0,)))

    def body(b: Builder, args):
        a, x, y = args

        block = Block(arg_types=[i32, i32, i32])
        bb = Builder(InsertPoint.at_end(block))
        a_elem, x_elem, y_acc = block.args
        prod = bb.insert(MuliOp(a_elem, x_elem)).result
        acc = bb.insert(AddiOp(prod, y_acc)).result
        bb.insert(YieldOp(acc))

        b.insert(
            GenericOp(
                inputs=[a, x],
                outputs=[y],
                body=Region(block),
                indexing_maps=[m1, m2, m3],
                iterator_types=[IteratorTypeAttr.parallel(), IteratorTypeAttr.reduction()],
            )
        )

    return _func("gemv", [a_t, x_t, y_t], body)


def build_reduceop(N: int = 1024) -> ModuleOp:
    """acc = sum(A[i] + B[i] for i in 0..N) -- matches the real kernel's
    actual computation (see this function's module-level docstring note on
    why `linalg.reduce` doesn't fit: it only takes one input). `linalg.generic`
    with two inputs, a single reduction iterator, and the output's indexing
    map `(d0) -> ()` -- standard linalg.generic reduction-to-scalar, not a
    placeholder."""
    a_t = MemRefType(i32, [N])
    b_t = MemRefType(i32, [N])
    acc_t = MemRefType(i32, [])

    in_map = AffineMapAttr(AffineMap.from_callable(lambda d0: (d0,)))
    acc_map = AffineMapAttr(AffineMap.from_callable(lambda d0: ()))

    def body(builder: Builder, args):
        a, b_arr, acc = args

        block = Block(arg_types=[i32, i32, i32])
        bb = Builder(InsertPoint.at_end(block))
        a_elem, b_elem, acc_in = block.args
        acc_plus_a = bb.insert(AddiOp(acc_in, a_elem)).result
        acc_plus_ab = bb.insert(AddiOp(acc_plus_a, b_elem)).result
        bb.insert(YieldOp(acc_plus_ab))

        builder.insert(
            GenericOp(
                inputs=[a, b_arr],
                outputs=[acc],
                body=Region(block),
                indexing_maps=[in_map, in_map, acc_map],
                iterator_types=[IteratorTypeAttr.reduction()],
            )
        )

    return _func("reduceop", [a_t, b_t, acc_t], body)


def build_scan(N: int = 1024) -> ModuleOp:
    """Prefix sum: out[i] = sum(x[0..i]). Real semantics is a recurrence
    (out[i] depends on out[i-1]), which no affine indexing map can express --
    the identity maps/body below are a placeholder shape only (same rank/size
    input->output, which IS accurate for scan; the elementwise body is not).
    `lowering/linalg_to_cnm.py` ignores this op's body entirely and emits the
    real running-accumulator `cnm.for` loop by matching on the enclosing
    func's name ("scan")."""
    x_t = MemRefType(i32, [N])
    y_t = MemRefType(i32, [N])
    identity = AffineMapAttr(AffineMap.from_callable(lambda d0: (d0,)))

    def body(b: Builder, args):
        x, y = args

        block = Block(arg_types=[i32, i32])
        bb = Builder(InsertPoint.at_end(block))
        x_elem, _y_elem = block.args
        bb.insert(YieldOp(x_elem))  # placeholder: real op is a recurrence, not elementwise

        b.insert(
            GenericOp(
                inputs=[x],
                outputs=[y],
                body=Region(block),
                indexing_maps=[identity, identity],
                iterator_types=[IteratorTypeAttr.parallel()],
                library_call=StringAttr("scan"),
            )
        )

    return _func("scan", [x_t, y_t], body)


def build_sel(N: int = 1024) -> ModuleOp:
    """Stream compaction: out = [x[i] for i in 0..N if pred(x[i])], packed
    contiguously. Which output slot an element lands in depends on a running,
    data-dependent count, not the loop index -- not affine. `out` is sized
    N (an upper bound on the compacted result) as a placeholder; the
    elementwise identity body below is likewise a placeholder, since the real
    op is a predicated write to a moving offset, not a per-index map.
    `lowering/linalg_to_cnm.py` matches on the enclosing func's name ("sel")
    and emits the real `cnm.if`-guarded write with a running-offset
    accumulator."""
    x_t = MemRefType(i32, [N])
    y_t = MemRefType(i32, [N])
    identity = AffineMapAttr(AffineMap.from_callable(lambda d0: (d0,)))

    def body(b: Builder, args):
        x, y = args

        block = Block(arg_types=[i32, i32])
        bb = Builder(InsertPoint.at_end(block))
        x_elem, _y_elem = block.args
        bb.insert(YieldOp(x_elem))  # placeholder: real op is predicated/compacting, not elementwise

        b.insert(
            GenericOp(
                inputs=[x],
                outputs=[y],
                body=Region(block),
                indexing_maps=[identity, identity],
                iterator_types=[IteratorTypeAttr.parallel()],
                library_call=StringAttr("sel"),
            )
        )

    return _func("sel", [x_t, y_t], body)


def build_hst(N: int = 1024, BINS: int = 256) -> ModuleOp:
    """Histogram: for each x[i], bins[x[i] % BINS] += 1. The write address
    is the *data value itself*, not an affine function of the loop index --
    genuinely not representable as an affine map, so the output's indexing
    map here is `(d0) -> (0)` (a constant), a syntactic placeholder only:
    it type-checks (rank matches `bins`'s rank) without asserting a false
    per-iteration correspondence. `lowering/linalg_to_cnm.py` matches on the
    enclosing func's name ("hst") and emits the real data-dependent
    load-increment-store sequence."""
    x_t = MemRefType(i32, [N])
    bins_t = MemRefType(i32, [BINS])
    in_map = AffineMapAttr(AffineMap.from_callable(lambda d0: (d0,)))
    out_map = AffineMapAttr(AffineMap.from_callable(lambda d0: (0,)))

    def body(b: Builder, args):
        x, bins = args

        block = Block(arg_types=[i32, i32])
        bb = Builder(InsertPoint.at_end(block))
        _x_elem, bin_acc = block.args
        bb.insert(YieldOp(bin_acc))  # placeholder: real op is a data-dependent scatter+increment

        b.insert(
            GenericOp(
                inputs=[x],
                outputs=[bins],
                body=Region(block),
                indexing_maps=[in_map, out_map],
                iterator_types=[IteratorTypeAttr.parallel()],
                library_call=StringAttr("hst"),
            )
        )

    return _func("hst", [x_t, bins_t], body)


KERNEL_BUILDERS = {
    "gemm": build_gemm,
    "gemv": build_gemv,
    "reduceop": build_reduceop,
    "scan": build_scan,
    "sel": build_sel,
    "hst": build_hst,
}


if __name__ == "__main__":
    from xdsl.printer import Printer

    for kernel_name, builder in KERNEL_BUILDERS.items():
        module = builder()
        module.verify()
        print(f"; ---- {kernel_name} ----")
        Printer().print_op(module)
        print()
