import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Traced from the real -O3 assembly (dpu.s). The BUFFER_COUNT==0 guards
    # (BB0_23/24, BB0_25/26's alternate path) are dead code for real sweeps,
    # so the three real phases below -- reduce (BB0_8/9), the offset loop
    # (BB0_12/14), and scan (BB0_20/21) -- are the only path that matters.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]
    barrier_lut = luts[LUTType.BarrierLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # Reduce phase: identical structure to ReduceOp's own kernel (single
    # array, ADD only). Real assembly per element: ld, then add+addc (the
    # data accumulation `result += cache_INPUT[c]`), then a separate add for
    # pointer advancement -- the model's two ADD units below correspond to
    # exactly those two, not two copies of the same thing.
    #
    # For INT64, `result += X` is a loop-carried accumulator (each
    # iteration's addc depends on the previous iteration's resolved carry),
    # which costs much more than an isolated add+addc pair (~11 cycles):
    # ~22 cycles for INT64 vs ~11 for INT32/INT16 (no chain penalty), i.e.
    # 2 ADD(INT32)-cost units instead of 1. Same root cause as ReduceOp's
    # INT64 fix (see Predictor/kernels/ReduceOp.py), but measured separately
    # since Scan's accumulator is single-term (`result += X`) rather than
    # ReduceOp's two-term (`result += A + B`) -- the per-term chain cost
    # doesn't scale linearly between the two shapes, so ReduceOp's constant
    # doesn't transfer here.
    data_add_cost = [baseInsLUT[(InsType.ADD, DataType.INT32)]] * (
        2 if data_type == DataType.INT64 else 1
    )
    reduce_c_loop = (
        [baseInsLUT[(InsType.L, data_type)]]
        + data_add_cost
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # pointer advance, not data-type-dependent
            baseInsLUT[InsType.J],
        ]
    )
    reduce_chunk_entry = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
    ]
    reduce_chunk_tail = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    reduce_body = reduce_chunk_entry + [reduce_c_loop] + reduce_chunk_tail

    # Once, after the reduce loop finishes: publish this tasklet's partial
    # sum to the shared partial_sums[] slot, then the real barrier_wait()
    # call. The barrier's own cost is NOT a fixed per-instruction latency --
    # it's empirically calibrated as a function of thread_count in
    # BarrierExperiment/ (real hardware shows contention on its internal
    # spinlock makes the cost scale worse than linearly with thread count,
    # which neither a static disassembly nor a naive O(thread_count) formula
    # would catch -- see load_lut.py's loadBarrierCostModel).
    barrier_ins = Instruction(InsType.BARRIER, getBarrierCost(barrier_lut, thread_count))
    after_reduce = [
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.MOV],
        barrier_ins,
    ]

    # Offset loop: iterates all THREADS partial_sums slots unconditionally
    # for every tasklet (uniform trip count, only the add-or-skip branch
    # depends on tasklet_id) -- but unlike Sel's predicate check or Gemv's
    # boolean-replacement pattern, the two branch outcomes here do NOT cost
    # the same: disassembly shows the "skip" path (t >= tasklet_id) is 3
    # instructions (J, ADD, J) while the "add" path (t < tasklet_id) is 6
    # (J, LSL, L, ADD, ADD, J). Since the Simulator runs one identical
    # program for every simulated thread, there's no way to give tasklet 0
    # (all skips) and tasklet THREADS-1 (all adds) their own distinct costs
    # -- modeled here as alternating skip/add pairs, which gives the same
    # *average* total cost per thread as the real 0..THREADS-1 spread of
    # tasklet_ids, without claiming to reproduce any single tasklet's exact
    # count. Slightly undercounts thread_count==1 (real cost is one skip
    # iteration, 3 instructions; this rounds it to 0 pairs) -- negligible in
    # absolute terms.
    skip_iter = [
        baseInsLUT[InsType.J],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    add_iter = [
        baseInsLUT[InsType.J],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    offset_pair = skip_iter + add_iter

    # Scan phase: running-sum scan, stored in place into cache_INPUT before
    # the mram_write (matches dpu.c's real in-place update). Same
    # loop-carried accumulator pattern as reduce_c_loop above
    # (`running += cache_INPUT[c]`) -- reuses the same data_add_cost.
    scan_c_loop = (
        [
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
        ]
        + data_add_cost
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # index advance, not data-type-dependent
            baseInsLUT[(InsType.S, data_type)],
            baseInsLUT[InsType.J],
        ]
    )
    scan_chunk_entry = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
    ]
    scan_chunk_tail = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    scan_body = scan_chunk_entry + [scan_c_loop] + scan_chunk_tail

    p = Simulator(thread_count)
    sim_iterations = [local_iter_count, buffer_size]

    # Nested fast-forwarding engine (predictor.py's Leaf/Repeat/Seq): each
    # phase's own chunk-level repeat (local_iter_count) carries the DMA
    # read/write, so it's never extrapolated (see compileSegment's
    # has_dma check) -- but reduce_c_loop/scan_c_loop, nested inside each
    # chunk, are pure WRAM/ALU with no DMA and DO get fast-forwarded, same
    # role as Gemv's innermost MAC loop. after_reduce's barrier similarly
    # blocks extrapolation of anything that would need to cross it, but it
    # runs once (not inside a loop) so that's moot here.
    result = p.putInstructionsNested(sim_iterations, reduce_body)
    result = p.runSegment(Leaf(after_reduce))
    if thread_count > 1:
        result = p.putInstructionsNested([thread_count // 2], offset_pair)
    result = p.putInstructionsNested(sim_iterations, scan_body)

    return result
