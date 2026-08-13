import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/reduceop.llvcnm,
    # the same way ReduceOp.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly (dpu.s) / AsmLoopExtractor's
    # reduceop_uint32_t_mainloop.txt. NOT auto-converted. Same signature as
    # ReduceOp.py's TestOp so benchmarks_reduceop.py's calling convention
    # carries over unchanged.
    #
    # The real kernel sums TWO input arrays elementwise (result += A[c];
    # result += B[c]), not a single-array sum. The inner loop uses
    # `cnm.for`'s `addressing="pointer"` mode (see dialects/cnm.py's ForOp
    # docstring): real ReduceOp's inner loop uses pointer-increment
    # addressing with a fused trip-decrement branch, unlike VectorOp's.
    #
    # reduceop.llvcnm, per chunk (num_chunks = local_iter_count, CHUNK = 128
    # standing in for buffer_size):
    #   MUL_IDX(V10, V9, V7)            -- offset = chunk_idx * CHUNK
    #   DMA_LOAD_I32(V3, V0[V10], 128)  -- A chunk
    #   DMA_LOAD_I32(V4, V1[V10], 128)  -- B chunk
    #   MOVE_IDX(V11, 0)                -- element index init
    #   .Lloop3 (repeated buffer_size times):
    #     LOAD_I32(V12, V3[V11])        -- no SCALE_IDX: pointer-mode access
    #     LOAD_I32(V13, V4[V11])
    #     ADD_I32(V14, V5, V12)         -- running += A[c]
    #     ADD_I32(V5, V14, V13)         -- running += B[c], targets V5 directly
    #     ADD_IDX(V3, V3, 1)            -- A pointer bump
    #     ADD_IDX(V4, V4, 1)            -- B pointer bump
    #     INCJNEQ(V11, V8, .Lloop3)     -- fused trip-decrement+branch
    #   INC(V9)
    #   JNEQ(V9, V6, .Lloop1)
    #
    # An almost exact instruction-for-instruction match to ReduceOp.py's own
    # real c_loop (L, L, ADD, ADD, ADD, ADD, J -- 7 instructions), except
    # for one real hardware effect the generated llvcnm text has no way to
    # show: for INT64, each 32-bit `add` on the low word is immediately
    # followed by an `addc` on the high word, and because `result` is a
    # loop-carried accumulator, each iteration's addc genuinely depends on
    # the previous iteration's resolved carry, forcing real serialization
    # (see ReduceOp.py's own extra_carry_adds comment: ~22 cycles/iteration
    # for INT32 vs ~55 for INT64, a ~33-cycle gap = 3 extra ADD(INT32)-cost
    # units). llvcnm has no addc/carry-chain concept -- it's a
    # target-agnostic virtual ISA -- so this is manually carried over from
    # Path A's own measurement.
    #
    # Mapping decisions: ADD_I32 (both accumulation and pointer-bump) ->
    # (ADD, INT32), matching ReduceOp.py's finding that a plain logical add
    # on this 32-bit ALU always costs the same regardless of the LUT's
    # data_type key. INCJNEQ (the fused trip-decrement+branch) -> InsType.J
    # alone (one dispatched instruction, not ADD+J as two). MOVE_IDX ->
    # InsType.MOV; MUL_IDX -> InsType.LSL (compile-time power-of-two CHUNK).
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # See the addc-chain-serialization note above -- manually mirrored from
    # ReduceOp.py's own extra_carry_adds, same position (right after the two
    # logical accumulate adds, before the pointer bumps and the branch).
    extra_carry_adds = (
        [baseInsLUT[(InsType.ADD, DataType.INT32)]] * 3 if data_type == DataType.INT64 else []
    )

    ins_list = [
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        (
            [
                baseInsLUT[(InsType.L, data_type)],
                baseInsLUT[(InsType.L, data_type)],
                baseInsLUT[(InsType.ADD, DataType.INT32)],
                baseInsLUT[(InsType.ADD, DataType.INT32)],
            ]
            + extra_carry_adds
            + [
                baseInsLUT[(InsType.ADD, DataType.INT32)],  # A pointer bump
                baseInsLUT[(InsType.ADD, DataType.INT32)],  # B pointer bump
                baseInsLUT[InsType.J],
            ]
        ),
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    sim_iterations = [local_iter_count, buffer_size]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, ins_list)
