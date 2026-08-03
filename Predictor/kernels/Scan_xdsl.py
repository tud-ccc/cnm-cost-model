import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/scan.llvcnm, the
    # same way Scan.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly. NOT auto-converted. Same signature and same
    # phase-chaining calls to the Simulator as Scan.py's TestOp (three
    # separate calls carrying Simulator state across them -- see
    # putInstructionsNested/runSegment's own "only the LAST call's return
    # value matters" convention), so benchmarks_scan_xdsl.py's calling
    # convention carries over unchanged.
    #
    # Real Scan (Scan/dpu/dpu.c) does THREE phases per tasklet -- reduce
    # this tasklet's own chunk, publish+barrier+compute a cross-tasklet
    # offset, then scan starting from that offset. `cnm.barrier` (see
    # dialects/cnm.py) represents the synchronization point.
    #
    # Phase 1 (reduce) -- scan.llvcnm, per chunk (num_chunks =
    # local_iter_count, CHUNK = 128 standing in for buffer_size), inner loop
    # marked `addressing="pointer"` (single read-only array, no indexed
    # output, same reasoning as ReduceOp's validated fix):
    #   MUL_IDX(offset), DMA_LOAD_I32(chunk)      -- prefix
    #   MOVE_IDX(j, 0)                            -- prefix
    #   .Lloop3 (repeated buffer_size times):
    #     LOAD_I32(cache[j])                      -- no SCALE_IDX: pointer mode
    #     ADD_I32(running, running, val)          -- targets running directly
    #     ADD_IDX(ptr, ptr, 1)                    -- pointer bump
    #     INCJNEQ(j, elem_ub, .Lloop3)             -- fused
    #   INC(chunk_idx), JNEQ(...)                 -- suffix
    #
    # Phase 2 (publish + barrier + cross-tasklet offset) is NOT transcribed
    # from the generated llvcnm's literal LT_I1/JEQ encoding: the offset
    # loop's branch outcome depends on `tasklet_id`, which nothing in this
    # single, tasklet-uniform kernel represents (see _lower_scan's own
    # docstring), and separately, this lowering's `cnm.if` always
    # materializes its condition as a separate compare instruction before
    # the branch -- real hardware fuses a comparison directly into a
    # conditional branch, so a literal transcription would overcount by one
    # instruction per offset-loop iteration on top of not being able to
    # track tasklet_id at all. Reuses Scan.py's own skip_iter/add_iter
    # averaging technique verbatim instead.
    #
    # Phase 3 (scan) -- unchanged in shape from before (in-place update,
    # index-scaled addressing CSE'd to one SCALE_IDX per element, matching
    # AsmLoopExtractor/output/scan_uint32_t_mainloop.txt), except the
    # running sum now starts from Phase 2's computed offset instead of 0
    # (handled by chaining the Simulator calls in order, exactly like
    # Scan.py does).
    #
    # Mapping decisions: MOVE_IDX/MOVE_I32 -> InsType.MOV; MUL_IDX ->
    # InsType.LSL (compile-time power-of-two CHUNK); SCALE_IDX -> InsType.LSL;
    # INC/pointer-bump ADD_IDX -> (ADD, INT32); INCJNEQ (fused) -> InsType.J
    # alone; JNEQ -> InsType.J. The accumulate ADD_I32 in both phase 1 and
    # phase 3 uses `data_add_cost` below, mirroring Scan.py's own
    # hardware-measured finding that Scan's single-term accumulator
    # (`result += X`) pays a DIFFERENT INT64 carry-chain penalty than
    # ReduceOp's two-term one (`result += A + B`) -- 2 extra ADD(INT32)
    # units, not 3 -- manually carried over from that already-validated
    # measurement, the same way ReduceOp_xdsl.py carries over
    # extra_carry_adds.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]
    barrier_lut = luts[LUTType.BarrierLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    data_add_cost = [baseInsLUT[(InsType.ADD, DataType.INT32)]] * (
        2 if data_type == DataType.INT64 else 1
    )

    # --- Phase 1: reduce ---
    reduce_c_loop = (
        [baseInsLUT[(InsType.L, data_type)]]
        + data_add_cost
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # pointer bump
            baseInsLUT[InsType.J],  # fused trip-decrement+branch
        ]
    )
    reduce_body = [
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        reduce_c_loop,
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    # --- Once, after the reduce loop: publish partial sum + barrier ---
    barrier_ins = Instruction(InsType.BARRIER, getBarrierCost(barrier_lut, thread_count))
    after_reduce = [
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.S, data_type)],
        barrier_ins,
    ]

    # --- Phase 2: offset loop -- reused verbatim from Scan.py, see the
    # comment above for why (untrackable per-tasklet branch outcome, not a
    # gap in this lowering specifically).
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

    # --- Phase 3: scan, in place, starting from Phase 2's offset ---
    scan_c_loop = (
        [
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
        ]
        + data_add_cost
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # index advance
            baseInsLUT[(InsType.S, data_type)],
            baseInsLUT[InsType.J],
        ]
    )
    scan_body = [
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        scan_c_loop,
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    p = Simulator(thread_count)
    sim_iterations = [local_iter_count, buffer_size]

    result = p.putInstructionsNested(sim_iterations, reduce_body)
    result = p.runSegment(Leaf(after_reduce))
    if thread_count > 1:
        result = p.putInstructionsNested([thread_count // 2], offset_pair)
    result = p.putInstructionsNested(sim_iterations, scan_body)

    return result
