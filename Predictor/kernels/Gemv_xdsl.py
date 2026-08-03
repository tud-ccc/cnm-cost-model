import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iteration, vec_length, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/gemv.llvcnm, the
    # same way gemv.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly. NOT auto-converted. Same signature as gemv.py's
    # TestOp.
    #
    # The linalg/cnm source this was transcribed from needed real
    # structural fixes first, all found by reading Gemv/dpu/dpu.c directly
    # (its compiled assembly alone under-determines this) -- an earlier
    # version of this lowering assumed the whole vector fit in one WRAM
    # chunk and cached it once for the whole kernel. Real Gemv (1) chunks
    # over the vector/K dimension in general, (2) re-fetches BOTH the row's
    # A-chunk AND the shared vector x fresh every column-chunk of every row
    # (never caches x), and (3) its dot-product loop uses pointer
    # addressing with a fused branch -- same 2-read/no-indexed-write shape
    # as ReduceOp's validated pointer+fused loop (this is that hypothesis,
    # noted but untested in ReduceOp_xdsl.py, actually being checked here
    # against real hardware). Fixed in lowering/linalg_to_cnm.py's
    # _lower_gemv, which also had its own bug caught by inspecting the
    # regenerated gemv.llvcnm directly: the per-row accumulator reset had
    # been hoisted to kernel scope (reset once for the whole kernel instead
    # of once per row) by the same kind of over-hoisting mistake the
    # general constant-hoisting fix was supposed to prevent -- fixed by
    # moving it back inside the row loop, matching ReduceOp/Scan/Hst's own
    # established convention for per-iteration resets.
    #
    # gemv.llvcnm's row body, once these are applied:
    #   MOVE_I32(sum, 0)                    -- per-row reset
    #   MUL_IDX(row_base, row, N)            -- per row
    #   MOVE_IDX(col_chunk_idx, 0)            -- per row
    #   .Lloop3 (repeated num_col_chunks times):
    #     MUL_IDX(col_offset), ADD_IDX(a_offset)
    #     DMA_LOAD_I32(A chunk), DMA_LOAD_I32(x chunk)
    #     MOVE_IDX(c, 0)
    #     .Lloop5 (repeated buffer_size times):
    #       LOAD_I32(A[c]); LOAD_I32(x[c])    -- pointer mode, no scale
    #       MUL_I32(product); ADD_I32(sum += product)
    #       ADD_IDX(A ptr++); ADD_IDX(x ptr++)
    #       INCJNEQ(...)                      -- fused
    #     INC(col_chunk_idx), JNEQ(...)
    #   STORE_I32(y[row], sum)
    #
    # MUL_I32 (the dot-product multiply) is NOT one instruction on real
    # hardware -- gemv.py's own comment explains why: the benchmark pins
    # the vector to all-1s specifically so `__mulsi3`'s operand-swap early
    # exit fires after exactly 1 of 32 mul_step copies, giving a fixed
    # 8-instruction call regardless of data_type (traced disassembly:
    # call, jgtu, 2x move (swap), move r1,zero, 1x mul_step, move result,
    # return). Reused verbatim from gemv.py below, the same way
    # ReduceOp_xdsl.py/Scan_xdsl.py/Hst_xdsl.py reuse their own respective
    # opaque-library-call corrections rather than re-deriving a worse one.
    #
    # NOT modeled here (same as gemv.py's own docstring reference, and
    # _lower_gemv's docstring): real Gemv batches multiple rows' results
    # into one `sdma` instead of a per-row scalar MRAM store, and spills
    # several per-row/per-chunk values to the stack that this lowering's
    # simpler index-based addressing never needs -- both bigger than a
    # mechanical fix, left as flagged follow-up work.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    num_col_chunks = vec_length // buffer_size

    # Reused verbatim from gemv.py.
    mulsi3_call = [
        baseInsLUT[InsType.J],  # call
        baseInsLUT[InsType.J],  # jgtu (compare-branch)
        baseInsLUT[InsType.MOV],  # move (swap setup)
        baseInsLUT[InsType.MOV],  # move (swap setup)
        baseInsLUT[InsType.MOV],  # move r1, zero
        baseInsLUT[InsType.J],  # mul_step (compare-branch class, 1 executes)
        baseInsLUT[InsType.MOV],  # move result
        baseInsLUT[InsType.J],  # return jump
    ]

    c_loop = [
        baseInsLUT[(InsType.L, data_type)],  # load cache_A[c], pointer mode
        baseInsLUT[(InsType.L, data_type)],  # load cache_x[c], pointer mode
        *mulsi3_call,
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # sum += product
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_x pointer++
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_A pointer++
        baseInsLUT[InsType.J],  # fused trip-decrement+branch
    ]

    col_chunk_body = (
        [
            baseInsLUT[InsType.LSL],  # col_offset
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # a_offset
            dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
            dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
            baseInsLUT[InsType.MOV],  # c init
        ]
        + [c_loop]
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # col_chunk_idx++
            baseInsLUT[InsType.J],
        ]
    )

    row_body = (
        [
            baseInsLUT[InsType.MOV],  # sum reset
            baseInsLUT[InsType.LSL],  # row_base = row * N
            baseInsLUT[InsType.MOV],  # col_chunk_idx init
        ]
        + [col_chunk_body]
        + [
            baseInsLUT[InsType.LSL],  # SCALE_IDX for y[row]
            baseInsLUT[(InsType.S, data_type)],
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # row++
            baseInsLUT[InsType.J],
        ]
    )

    sim_iterations = [iteration, num_col_chunks, buffer_size]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, row_body)
