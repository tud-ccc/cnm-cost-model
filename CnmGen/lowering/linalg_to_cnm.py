"""Lowers the linalg kernels in `frontend/linalg_kernels.py` into `cnm.kernel`
IR, using the real two-level chunk/DMA/WRAM structure of each kernel's
compiled UPMEM assembly (AsmLoopExtractor's output for VectorOp/ReduceOp/
Scan/Hst/Sel/Gemm/Gemv).

This does NOT mechanically interpret `linalg.generic`'s indexing_maps/
iterator_types to derive loop nests. Instead, each kernel gets a
hand-designed `cnm.kernel` builder, dispatched purely by the linalg module's
`func.func` name ("gemm", "gemv", "reduceop", "scan", "sel", "hst"). Only
operand *shapes* are read out of the linalg IR; its body/indexing-maps are
not interpreted.

Two general conventions apply across every kernel builder below:
  - Compile-time-constant bounds (chunk size, loop upper bounds, BINS, a
    predicate threshold, etc.) are computed once at kernel scope and reused
    by every nested loop level, matching a real -O3 compiler hoisting a
    loop-invariant constant. Per-iteration *resets* (an accumulator or index
    restarting at 0 each chunk) are NOT hoisted -- those are real work every
    compiled loop repeats.
  - `cnm.for` has an `addressing` property ("index" vs "pointer", see its
    docstring in dialects/cnm.py) recording which of two real UPMEM -O3
    addressing idioms a given loop's compiled form uses, validated per
    kernel against real hardware via the matching Predictor/kernels/*_xdsl.py
    module.

Scan additionally needs `cnm.barrier` (see dialects/cnm.py): its real kernel
is a three-phase-per-tasklet algorithm -- reduce this tasklet's own chunk,
publish the sum to a DPU-wide shared array and barrier, compute a starting
offset from every other tasklet's published sum, then scan from that offset
-- see _lower_scan's own docstring for how the per-tasklet predicate is
handled.

Known, still-open simplifications (consistent with cnm_to_llvcnm.py's own
"close, not exact" framing):
  - Gemm/Gemv: assumes each matrix row/vector fits in one WRAM chunk (true at
    this file's default sizes, CHUNK=128 >= K=N=64); the real kernels always
    chunk over K/N regardless of size, re-fetch reused vectors/rows from MRAM
    every iteration rather than caching them once, and batch multiple output
    rows into one `sdma` instead of storing per-row/per-cell scalars.
  - Hst: keeps the whole `bins` histogram resident in one WRAM buffer for the
    kernel's full duration, merged out via a single `cnm.dma_store` at the
    very end -- matches Hst/dpu/dpu.c's own single final `mram_write`.
"""

from __future__ import annotations

from xdsl.builder import Builder
from xdsl.dialects.arith import ConstantOp
from xdsl.dialects.builtin import IndexType, IntegerAttr, IntegerType, MemRefType, ModuleOp, i32
from xdsl.dialects.func import FuncOp
from xdsl.ir import Block, Region
from xdsl.rewriter import InsertPoint

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

CHUNK = 128
index_t = IndexType()
i1 = IntegerType(1)


def _const(builder: Builder, value: int, ty=index_t):
    return builder.insert(ConstantOp(IntegerAttr(value, ty))).result


def _find_func(module: ModuleOp) -> FuncOp:
    for op in module.body.block.ops:
        if isinstance(op, FuncOp):
            return op
    raise ValueError("expected a module containing exactly one func.func")


def _shape(func: FuncOp, arg_index: int) -> list[int]:
    ty = func.body.block.args[arg_index].type
    assert isinstance(ty, MemRefType)
    return list(ty.get_shape())


def lower_module(module: ModuleOp) -> KernelOp:
    """Dispatches on the linalg module's single `func.func`'s name to the
    matching hand-designed `cnm.kernel` builder."""
    func = _find_func(module)
    builders = {
        "gemm": _lower_gemm,
        "gemv": _lower_gemv,
        "reduceop": _lower_reduceop,
        "scan": _lower_scan,
        "sel": _lower_sel,
        "hst": _lower_hst,
    }
    build = builders.get(func.sym_name.data)
    if build is None:
        raise ValueError(f"no cnm lowering registered for kernel {func.sym_name.data!r}")
    return build(func)


def _lower_gemm(func: FuncOp) -> KernelOp:
    """Real Gemm (Gemm/dpu/dpu.c) is a 4-level tiled algorithm: per row, per
    N-tile (a CHUNK-wide slice of output columns), zero the tile's output
    accumulator, then per K-chunk, DMA this row's A K-chunk once and reuse it
    against every column in the tile, DMA-ing that column's own B K-chunk
    fresh each time. B is stored TRANSPOSED on real hardware, modeled here as
    `B` having shape [N, K] instead of the mathematical [K, N] -- a
    lowering-only decision; the linalg level above still uses standard,
    untransposed matmul semantics.

    The innermost dot-product loop is left at the default addressing
    ("index", `fuse_branch=False`), not pointer mode like ReduceOp/Gemv's
    2-read-only loops: Gemm.py's real c_loop scales the address before every
    load and closes with a separate, unfused ADD+J, because the compiler
    can't prove `cache_C` doesn't alias `cache_A`/`cache_B` and so stores the
    running sum back to `cache_C[col]` every iteration -- modeled faithfully
    here too.

    Not modeled: real cache_C zeroing is a `memset()` library call; this
    lowering zeros it with a plain per-element store loop instead."""
    M, K = _shape(func, 0)
    _, N = _shape(func, 1)
    num_k_chunks = K // CHUNK
    num_n_tiles = N // CHUNK

    a_chunk_t = MemRefType(i32, [CHUNK])
    b_chunk_t = MemRefType(i32, [CHUNK])
    c_tile_t = MemRefType(i32, [CHUNK])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    a_op = kb.insert(OperandOp("A", MemRefType(i32, [M, K])))
    b_op = kb.insert(OperandOp("B", MemRefType(i32, [N, K])))  # transposed storage
    c_op = kb.insert(OutputOp("C", MemRefType(i32, [M, N])))

    cache_a = kb.insert(AllocOp(a_chunk_t))
    cache_b = kb.insert(AllocOp(b_chunk_t))
    cache_c = kb.insert(AllocOp(c_tile_t))

    m_ub = _const(kb, M)
    n_tiles_ub = _const(kb, num_n_tiles)
    k_chunks_ub = _const(kb, num_k_chunks)
    tile_size_const = _const(kb, CHUNK)  # doubles as BUFFER_COUNT (col count per tile)
    k_const = _const(kb, K)
    n_const = _const(kb, N)

    row_block = Block(arg_types=[index_t])
    rb = Builder(InsertPoint.at_end(row_block))
    row = row_block.args[0]
    row_a_base = rb.insert(ComputeOp("mul", [row, k_const], index_t)).res
    row_c_base = rb.insert(ComputeOp("mul", [row, n_const], index_t)).res

    n_tile_block = Block(arg_types=[index_t])
    ntb = Builder(InsertPoint.at_end(n_tile_block))
    n_tile = n_tile_block.args[0]

    # Zero cache_C for this tile -- real hardware calls memset(); see this
    # function's docstring for why a plain per-element loop stands in here.
    zero_i32 = _const(ntb, 0, i32)
    zero_init_block = Block(arg_types=[index_t])
    zib = Builder(InsertPoint.at_end(zero_init_block))
    zib.insert(StoreOp(zero_i32, cache_c, [zero_init_block.args[0]], level="WRAM"))
    zib.insert(YieldOp())
    ntb.insert(ForOp(ub=tile_size_const, iter_args=[], body=Region(zero_init_block)))

    n_tile_base = ntb.insert(ComputeOp("mul", [n_tile, tile_size_const], index_t)).res

    k_chunk_block = Block(arg_types=[index_t])
    kcb = Builder(InsertPoint.at_end(k_chunk_block))
    k_chunk = k_chunk_block.args[0]

    k_offset = kcb.insert(ComputeOp("mul", [k_chunk, tile_size_const], index_t)).res
    a_offset = kcb.insert(ComputeOp("add", [row_a_base, k_offset], index_t)).res
    kcb.insert(DmaLoadOp(a_op, a_offset, cache_a, size=CHUNK))  # once per k-chunk, reused across columns

    col_block = Block(arg_types=[index_t])
    colb = Builder(InsertPoint.at_end(col_block))
    col = col_block.args[0]

    col_idx = colb.insert(ComputeOp("add", [n_tile_base, col], index_t)).res
    b_row_base = colb.insert(ComputeOp("mul", [col_idx, k_const], index_t)).res
    b_offset = colb.insert(ComputeOp("add", [b_row_base, k_offset], index_t)).res
    colb.insert(DmaLoadOp(b_op, b_offset, cache_b, size=CHUNK))  # once per column

    cur_c = colb.insert(LoadOp(cache_c, [col], level="WRAM", result_type=i32)).res

    c_block = Block(arg_types=[index_t, i32])
    ccb = Builder(InsertPoint.at_end(c_block))
    c, acc = c_block.args
    a_val = ccb.insert(LoadOp(cache_a, [c], level="WRAM", result_type=i32)).res
    b_val = ccb.insert(LoadOp(cache_b, [c], level="WRAM", result_type=i32)).res
    prod = ccb.insert(ComputeOp("mul", [a_val, b_val], i32)).res
    new_acc = ccb.insert(ComputeOp("add", [acc, prod], i32)).res
    # Stored every iteration, not just at the end -- see this function's
    # docstring (real compiler can't prove non-aliasing between cache_C and
    # cache_A/cache_B).
    ccb.insert(StoreOp(new_acc, cache_c, [col], level="WRAM"))
    ccb.insert(YieldOp([new_acc]))
    c_loop = colb.insert(ForOp(ub=tile_size_const, iter_args=[cur_c], body=Region(c_block)))
    colb.insert(YieldOp())
    kcb.insert(ForOp(ub=tile_size_const, iter_args=[], body=Region(col_block)))
    kcb.insert(YieldOp())
    ntb.insert(ForOp(ub=k_chunks_ub, iter_args=[], body=Region(k_chunk_block)))

    c_offset = ntb.insert(ComputeOp("add", [row_c_base, n_tile_base], index_t)).res
    ntb.insert(DmaStoreOp(cache_c, c_op, c_offset, size=CHUNK))
    ntb.insert(YieldOp())
    rb.insert(ForOp(ub=n_tiles_ub, iter_args=[], body=Region(n_tile_block)))
    rb.insert(YieldOp())

    kb.insert(ForOp(ub=m_ub, iter_args=[], body=Region(row_block)))
    kb.insert(ReturnOp([c_op]))

    return KernelOp("gemm", Region(kernel_block))


def _lower_gemv(func: FuncOp) -> KernelOp:
    """Real Gemv (Gemv/dpu/dpu.c) chunks over the vector/K dimension in
    general, re-fetches both this row's A-chunk and the shared vector x
    fresh every column-chunk of every row rather than caching x once for
    the whole kernel, and its inner dot-product loop uses pointer
    addressing with a fused branch (two bumped pointers, no per-access
    scale) -- same shape as ReduceOp's pointer+fused loop. All three
    modeled here.

    Not modeled: real Gemv batches multiple rows' results into one `sdma`
    instead of this lowering's per-row scalar MRAM store, and spills several
    per-row/per-chunk values to the stack that this lowering's simpler
    index-based addressing never needs."""
    M, N = _shape(func, 0)
    num_col_chunks = N // CHUNK
    chunk_t = MemRefType(i32, [CHUNK])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    a_op = kb.insert(OperandOp("A", MemRefType(i32, [M, N])))
    x_op = kb.insert(OperandOp("x", MemRefType(i32, [N])))
    y_op = kb.insert(OutputOp("y", MemRefType(i32, [M])))

    cache_a = kb.insert(AllocOp(chunk_t))
    cache_x = kb.insert(AllocOp(chunk_t))

    m_ub = _const(kb, M)
    col_chunks_ub = _const(kb, num_col_chunks)
    chunk_size_const = _const(kb, CHUNK)
    elem_ub = _const(kb, CHUNK)
    n_const = _const(kb, N)

    row_block = Block(arg_types=[index_t])
    rb = Builder(InsertPoint.at_end(row_block))
    row = row_block.args[0]

    # Per-row accumulator reset -- NOT hoistable to kernel scope like the
    # bound constants above: each row's dot-product sum must start fresh,
    # same as ReduceOp/Scan's per-chunk resets.
    zero_i32 = _const(rb, 0, i32)
    row_base = rb.insert(ComputeOp("mul", [row, n_const], index_t)).res

    col_block = Block(arg_types=[index_t, i32])
    cb = Builder(InsertPoint.at_end(col_block))
    col_chunk_idx, sum_in = col_block.args

    col_offset = cb.insert(ComputeOp("mul", [col_chunk_idx, chunk_size_const], index_t)).res
    a_offset = cb.insert(ComputeOp("add", [row_base, col_offset], index_t)).res
    cb.insert(DmaLoadOp(a_op, a_offset, cache_a, size=CHUNK))
    cb.insert(DmaLoadOp(x_op, col_offset, cache_x, size=CHUNK))

    elem_block = Block(arg_types=[index_t, i32])
    eb = Builder(InsertPoint.at_end(elem_block))
    c, acc = elem_block.args
    a_val = eb.insert(LoadOp(cache_a, [c], level="WRAM", result_type=i32)).res
    x_val = eb.insert(LoadOp(cache_x, [c], level="WRAM", result_type=i32)).res
    prod = eb.insert(ComputeOp("mul", [a_val, x_val], i32)).res
    new_acc = eb.insert(ComputeOp("add", [acc, prod], i32)).res
    eb.insert(YieldOp([new_acc]))
    elem_loop = cb.insert(
        ForOp(
            ub=elem_ub,
            iter_args=[sum_in],
            body=Region(elem_block),
            addressing="pointer",
            fuse_branch=True,
        )
    )
    cb.insert(YieldOp([elem_loop.res[0]]))
    col_loop = rb.insert(
        ForOp(ub=col_chunks_ub, iter_args=[zero_i32], body=Region(col_block))
    )

    rb.insert(StoreOp(col_loop.res[0], y_op, [row], level="MRAM"))
    rb.insert(YieldOp())
    kb.insert(ForOp(ub=m_ub, iter_args=[], body=Region(row_block)))
    kb.insert(ReturnOp([y_op]))

    return KernelOp("gemv", Region(kernel_block))


def _lower_reduceop(func: FuncOp) -> KernelOp:
    """Real ReduceOp (AsmLoopExtractor/output/reduceop_uint32_t_mainloop.txt)
    DMAs two chunks (A and B) per iteration and, per element, does
    `lw, lw, add, add` -- load A[c], load B[c], result += A[c],
    result += B[c] -- not a single-array reduction. Matches
    frontend/linalg_kernels.py's build_reduceop.

    The inner (per-element) loop is marked `addressing="pointer"` with
    `fuse_branch=True`: real ReduceOp's inner loop bumps two pointers
    directly and fuses its trip-count decrement into the closing branch,
    unlike VectorOp's inner loop (index-scaled, unfused), which additionally
    writes a 3rd, differently-based array."""
    (N,) = _shape(func, 0)
    num_chunks = N // CHUNK
    chunk_t = MemRefType(i32, [CHUNK])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    a_op = kb.insert(OperandOp("A", MemRefType(i32, [N])))
    b_op = kb.insert(OperandOp("B", MemRefType(i32, [N])))
    acc_op = kb.insert(OutputOp("acc", MemRefType(i32, [])))

    cache_a = kb.insert(AllocOp(chunk_t))
    cache_b = kb.insert(AllocOp(chunk_t))

    zero_i32 = _const(kb, 0, i32)
    chunks_ub = _const(kb, num_chunks)
    chunk_size_const = _const(kb, CHUNK)
    elem_ub = _const(kb, CHUNK)

    chunk_block = Block(arg_types=[index_t, i32])
    cbb = Builder(InsertPoint.at_end(chunk_block))
    chunk_idx, total_in = chunk_block.args

    offset = cbb.insert(ComputeOp("mul", [chunk_idx, chunk_size_const], index_t)).res
    cbb.insert(DmaLoadOp(a_op, offset, cache_a, size=CHUNK))
    cbb.insert(DmaLoadOp(b_op, offset, cache_b, size=CHUNK))

    elem_block = Block(arg_types=[index_t, i32])
    eb = Builder(InsertPoint.at_end(elem_block))
    j, running = elem_block.args
    a_val = eb.insert(LoadOp(cache_a, [j], level="WRAM", result_type=i32)).res
    b_val = eb.insert(LoadOp(cache_b, [j], level="WRAM", result_type=i32)).res
    running_plus_a = eb.insert(ComputeOp("add", [running, a_val], i32)).res
    running_plus_ab = eb.insert(ComputeOp("add", [running_plus_a, b_val], i32)).res
    eb.insert(YieldOp([running_plus_ab]))
    elem_loop = cbb.insert(
        ForOp(
            ub=elem_ub,
            iter_args=[total_in],
            body=Region(elem_block),
            addressing="pointer",
            fuse_branch=True,
        )
    )

    cbb.insert(YieldOp([elem_loop.res[0]]))
    chunk_loop = kb.insert(ForOp(ub=chunks_ub, iter_args=[zero_i32], body=Region(chunk_block)))

    kb.insert(StoreOp(chunk_loop.res[0], acc_op, [], level="MRAM"))
    kb.insert(ReturnOp([acc_op]))

    return KernelOp("reduceop", Region(kernel_block))


def _lower_scan(func: FuncOp) -> KernelOp:
    """Real Scan (Scan/dpu/dpu.c) is a three-phase per-tasklet algorithm:
      1. Reduce this tasklet's own chunk to a single sum -- same shape as
         ReduceOp's inner loop but one array, not two (`pointer` addressing,
         a single read-only array with no indexed output).
      2. Publish that sum into a DPU-wide shared WRAM array
         (`partial_sums[tasklet_id]`, modeled with the same `cnm.alloc`
         mechanism as a per-tasklet-local buffer), `cnm.barrier`, then
         compute this tasklet's starting offset from every *other*
         tasklet's published sum.
      3. The actual scan, in place (index-scaled addressing, CSE'd to one
         address computation per element), starting the running sum from
         the computed offset instead of a hardcoded 0.

    Phase 2's predicate depends on `tasklet_id`, which nothing in this
    single, tasklet-uniform kernel represents (the Simulator dispatches the
    same instruction sequence to every simulated thread) -- so the offset
    loop's condition here is a placeholder (`t < THREADS // 2`), immaterial
    for cost purposes since a taken vs. not-taken branch reads/skips the
    same one `partial_sums[t]` load either way in this lowering (real
    hardware's two paths actually cost different amounts -- see
    Predictor/kernels/Scan.py's comment on exactly this). This lowering
    keeps the phase structurally literal (a real `cnm.if` inside a real
    loop, matching the C); Predictor/kernels/Scan_xdsl.py's hand
    transcription applies Scan.py's skip/add-pair averaging technique when
    converting this phase to an actual predicted cost."""
    THREADS = 16
    (N,) = _shape(func, 0)
    num_chunks = N // CHUNK
    chunk_t = MemRefType(i32, [CHUNK])
    partial_sums_t = MemRefType(i32, [THREADS])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    x_op = kb.insert(OperandOp("x", MemRefType(i32, [N])))
    y_op = kb.insert(OutputOp("y", MemRefType(i32, [N])))

    cache = kb.insert(AllocOp(chunk_t))
    partial_sums = kb.insert(AllocOp(partial_sums_t))

    zero_idx = _const(kb, 0)
    zero_i32 = _const(kb, 0, i32)
    chunks_ub = _const(kb, num_chunks)
    chunk_size_const = _const(kb, CHUNK)
    elem_ub = _const(kb, CHUNK)
    threads_ub = _const(kb, THREADS)
    tasklet_id_placeholder = _const(kb, THREADS // 2)

    # --- Phase 1: reduce this tasklet's own chunk ---
    reduce_chunk_block = Block(arg_types=[index_t, i32])
    rcb = Builder(InsertPoint.at_end(reduce_chunk_block))
    r_chunk_idx, r_total_in = reduce_chunk_block.args

    r_offset = rcb.insert(ComputeOp("mul", [r_chunk_idx, chunk_size_const], index_t)).res
    rcb.insert(DmaLoadOp(x_op, r_offset, cache, size=CHUNK))

    reduce_elem_block = Block(arg_types=[index_t, i32])
    reb = Builder(InsertPoint.at_end(reduce_elem_block))
    rj, r_running = reduce_elem_block.args
    r_val = reb.insert(LoadOp(cache, [rj], level="WRAM", result_type=i32)).res
    r_new_running = reb.insert(ComputeOp("add", [r_running, r_val], i32)).res
    reb.insert(YieldOp([r_new_running]))
    reduce_elem_loop = rcb.insert(
        ForOp(
            ub=elem_ub,
            iter_args=[r_total_in],
            body=Region(reduce_elem_block),
            addressing="pointer",
            fuse_branch=True,
        )
    )
    rcb.insert(YieldOp([reduce_elem_loop.res[0]]))
    reduce_loop = kb.insert(
        ForOp(ub=chunks_ub, iter_args=[zero_i32], body=Region(reduce_chunk_block))
    )

    # --- Phase 2: publish + barrier + cross-tasklet offset ---
    kb.insert(StoreOp(reduce_loop.res[0], partial_sums, [zero_idx], level="WRAM"))
    kb.insert(BarrierOp())

    offset_block = Block(arg_types=[index_t, i32])
    ob = Builder(InsertPoint.at_end(offset_block))
    t, offset_in = offset_block.args
    cond = ob.insert(ComputeOp("lt", [t, tasklet_id_placeholder], i1)).res

    then_block = Block()
    tb = Builder(InsertPoint.at_end(then_block))
    slot_val = tb.insert(LoadOp(partial_sums, [t], level="WRAM", result_type=i32)).res
    new_offset = tb.insert(ComputeOp("add", [offset_in, slot_val], i32)).res
    tb.insert(YieldOp([new_offset]))

    else_block = Block()
    elb = Builder(InsertPoint.at_end(else_block))
    elb.insert(YieldOp([offset_in]))

    if_op = ob.insert(IfOp(cond, [i32], Region(then_block), Region(else_block)))
    ob.insert(YieldOp([if_op.res[0]]))
    offset_loop = kb.insert(ForOp(ub=threads_ub, iter_args=[zero_i32], body=Region(offset_block)))

    # --- Phase 3: scan, in place, starting from the computed offset ---
    scan_chunk_block = Block(arg_types=[index_t, i32])
    scb = Builder(InsertPoint.at_end(scan_chunk_block))
    s_chunk_idx, s_running_in = scan_chunk_block.args

    s_offset = scb.insert(ComputeOp("mul", [s_chunk_idx, chunk_size_const], index_t)).res
    scb.insert(DmaLoadOp(x_op, s_offset, cache, size=CHUNK))

    scan_elem_block = Block(arg_types=[index_t, i32])
    seb = Builder(InsertPoint.at_end(scan_elem_block))
    sj, s_running = scan_elem_block.args
    s_val = seb.insert(LoadOp(cache, [sj], level="WRAM", result_type=i32)).res
    s_new_running = seb.insert(ComputeOp("add", [s_running, s_val], i32)).res
    seb.insert(StoreOp(s_new_running, cache, [sj], level="WRAM"))
    seb.insert(YieldOp([s_new_running]))
    scan_elem_loop = scb.insert(
        ForOp(ub=elem_ub, iter_args=[s_running_in], body=Region(scan_elem_block))
    )

    scb.insert(DmaStoreOp(cache, y_op, s_offset, size=CHUNK))
    scb.insert(YieldOp([scan_elem_loop.res[0]]))
    kb.insert(
        ForOp(ub=chunks_ub, iter_args=[offset_loop.res[0]], body=Region(scan_chunk_block))
    )

    kb.insert(ReturnOp([y_op]))

    return KernelOp("scan", Region(kernel_block))


def _lower_sel(func: FuncOp) -> KernelOp:
    """Predicate: select elements strictly greater than SEL_THRESHOLD --
    arbitrary, illustrative (real Sel's predicate is configured by the host,
    see Sel/support/pred.h; not modeled here).

    Matches Sel/dpu/dpu.c: (1) each chunk's compacted output is written at
    that chunk's own INPUT-aligned offset, same stride as the input,
    restarting at the front of that chunk's own reserved region --
    compaction only happens *within* one chunk, chunks don't pack together.
    The full CHUNK-sized `cache_out` is written regardless of how many were
    actually selected, matching the real kernel's unconditional
    `mram_write(..., BUFFER_SIZE)`. (2) the real kernel also DMAs out a
    separate, per-chunk COUNT value, modeled here as its own small `count`
    output, one element per chunk -- sized 4 elements (16 bytes for
    uint32_t), matching dpu.c's actual `mem_alloc(4 * sizeof(T))` (MRAM
    writes must be a multiple of 8 bytes, so real Sel pads a 1-element count
    to a 4-element block -- the same alignment reason Gemv/dpu/dpu.c pads
    its own final row batch)."""
    SEL_THRESHOLD = 0
    COUNT_BLOCK = 4
    (N,) = _shape(func, 0)
    num_chunks = N // CHUNK
    chunk_t = MemRefType(i32, [CHUNK])
    count_cache_t = MemRefType(i32, [COUNT_BLOCK])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    x_op = kb.insert(OperandOp("x", MemRefType(i32, [N])))
    y_op = kb.insert(OutputOp("y", MemRefType(i32, [N])))
    count_op = kb.insert(OutputOp("count", MemRefType(i32, [num_chunks * COUNT_BLOCK])))

    cache_in = kb.insert(AllocOp(chunk_t))
    cache_out = kb.insert(AllocOp(chunk_t))
    cache_count = kb.insert(AllocOp(count_cache_t))

    zero_idx = _const(kb, 0)
    chunks_ub = _const(kb, num_chunks)
    chunk_size_const = _const(kb, CHUNK)
    elem_ub = _const(kb, CHUNK)
    threshold = _const(kb, SEL_THRESHOLD, i32)
    one_idx = _const(kb, 1)
    count_block_const = _const(kb, COUNT_BLOCK)

    # Nothing threads across chunks: real Sel resets `count = 0` fresh at
    # the top of every chunk (not carried from the previous one), so this
    # loop carries no iter_args at all -- unlike ReduceOp/Scan's running
    # accumulator.
    chunk_block = Block(arg_types=[index_t])
    cbb = Builder(InsertPoint.at_end(chunk_block))
    chunk_idx = chunk_block.args[0]

    offset = cbb.insert(ComputeOp("mul", [chunk_idx, chunk_size_const], index_t)).res
    cbb.insert(DmaLoadOp(x_op, offset, cache_in, size=CHUNK))

    elem_block = Block(arg_types=[index_t, index_t])
    eb = Builder(InsertPoint.at_end(elem_block))
    j, local_count = elem_block.args
    val = eb.insert(LoadOp(cache_in, [j], level="WRAM", result_type=i32)).res
    cond = eb.insert(ComputeOp("gt", [val, threshold], i1)).res

    then_block = Block()
    tb = Builder(InsertPoint.at_end(then_block))
    tb.insert(StoreOp(val, cache_out, [local_count], level="WRAM"))
    incremented = tb.insert(ComputeOp("add", [local_count, one_idx], index_t)).res
    tb.insert(YieldOp([incremented]))

    else_block = Block()
    elb = Builder(InsertPoint.at_end(else_block))
    elb.insert(YieldOp([local_count]))

    if_op = eb.insert(IfOp(cond, [index_t], Region(then_block), Region(else_block)))
    eb.insert(YieldOp([if_op.res[0]]))
    # addressing="pointer": Sel.py's real trace shows no separate scale
    # instruction for the input load and an unfused ADD+J close, same as
    # Hst's element loop. cache_out's access uses `local_count`, not this
    # loop's own indvar `j`, so only cache_in[j] is affected.
    elem_loop = cbb.insert(
        ForOp(ub=elem_ub, iter_args=[zero_idx], body=Region(elem_block), addressing="pointer")
    )

    count_offset = cbb.insert(ComputeOp("mul", [chunk_idx, count_block_const], index_t)).res
    cbb.insert(DmaStoreOp(cache_out, y_op, offset, size=CHUNK))
    cbb.insert(StoreOp(elem_loop.res[0], cache_count, [zero_idx], level="WRAM"))
    cbb.insert(DmaStoreOp(cache_count, count_op, count_offset, size=COUNT_BLOCK))
    cbb.insert(YieldOp())
    kb.insert(ForOp(ub=chunks_ub, iter_args=[], body=Region(chunk_block)))

    kb.insert(ReturnOp([y_op, count_op]))

    return KernelOp("sel", Region(kernel_block))


def _lower_hst(func: FuncOp) -> KernelOp:
    """Bin index matches Hst/dpu/dpu.c exactly: `(value * BINS) >> DEPTH`
    (DEPTH=12, fixed by Hst/run_script/configuration.py) -- a real
    multiply-shift, not a modulo.

    The element loop is marked `addressing="pointer"` for its input array
    (a pointer bumped directly, no per-access scale), but `fuse_branch` is
    left at its default False, unlike ReduceOp/Scan's pointer loops: Hst's
    real trace lists the trip check as a separate ADD then J, not one fused
    instruction -- pointer addressing and branch fusion are independent
    properties of a compiled loop, which is why ForOp tracks them
    separately."""
    DEPTH = 12
    (N,) = _shape(func, 0)
    (BINS,) = _shape(func, 1)
    num_chunks = N // CHUNK
    chunk_t = MemRefType(i32, [CHUNK])
    bins_t = MemRefType(i32, [BINS])

    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    x_op = kb.insert(OperandOp("x", MemRefType(i32, [N])))
    bins_op = kb.insert(OutputOp("bins", bins_t))

    cache_in = kb.insert(AllocOp(chunk_t))
    cache_bins = kb.insert(AllocOp(bins_t))

    # Zero-init the local histogram (real hardware needs an explicit
    # zeroing pass too -- there's no implicit-zero WRAM allocation).
    bins_ub = _const(kb, BINS)
    zero_i32 = _const(kb, 0, i32)
    init_block = Block(arg_types=[index_t])
    ib = Builder(InsertPoint.at_end(init_block))
    ib.insert(StoreOp(zero_i32, cache_bins, [init_block.args[0]], level="WRAM"))
    ib.insert(YieldOp())
    kb.insert(ForOp(ub=bins_ub, iter_args=[], body=Region(init_block)))

    chunks_ub = _const(kb, num_chunks)
    chunk_size_const = _const(kb, CHUNK)
    elem_ub = _const(kb, CHUNK)
    bins_const = _const(kb, BINS, i32)
    depth_const = _const(kb, DEPTH, i32)
    one_i32 = _const(kb, 1, i32)

    chunk_block = Block(arg_types=[index_t])
    cbb = Builder(InsertPoint.at_end(chunk_block))
    chunk_idx = chunk_block.args[0]

    offset = cbb.insert(ComputeOp("mul", [chunk_idx, chunk_size_const], index_t)).res
    cbb.insert(DmaLoadOp(x_op, offset, cache_in, size=CHUNK))

    elem_block = Block(arg_types=[index_t])
    eb = Builder(InsertPoint.at_end(elem_block))
    j = elem_block.args[0]
    val = eb.insert(LoadOp(cache_in, [j], level="WRAM", result_type=i32)).res
    scaled = eb.insert(ComputeOp("mul", [val, bins_const], i32)).res
    bin_idx_i32 = eb.insert(ComputeOp("shr", [scaled, depth_const], i32)).res
    bin_idx = eb.insert(ComputeOp("index_cast", [bin_idx_i32], index_t)).res
    cur = eb.insert(LoadOp(cache_bins, [bin_idx], level="WRAM", result_type=i32)).res
    incremented = eb.insert(ComputeOp("add", [cur, one_i32], i32)).res
    eb.insert(StoreOp(incremented, cache_bins, [bin_idx], level="WRAM"))
    eb.insert(YieldOp())
    cbb.insert(
        ForOp(ub=elem_ub, iter_args=[], body=Region(elem_block), addressing="pointer")
    )
    cbb.insert(YieldOp())
    kb.insert(ForOp(ub=chunks_ub, iter_args=[], body=Region(chunk_block)))

    zero_off = _const(kb, 0)
    kb.insert(DmaStoreOp(cache_bins, bins_op, zero_off, size=BINS))
    kb.insert(ReturnOp([bins_op]))

    return KernelOp("hst", Region(kernel_block))
