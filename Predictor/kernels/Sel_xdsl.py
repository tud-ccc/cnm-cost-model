import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/sel.llvcnm, the
    # same way Sel.py's ins_list was hand-transcribed from the real compiled
    # -O3 assembly. NOT auto-converted. Same signature and same
    # element-pairing trick as Sel.py's TestOp.
    #
    # Real Sel allocates and writes a 4-element (16-byte for uint32_t)
    # per-chunk COUNT block (`mem_alloc(4 * sizeof(T))`,
    # `mram_write(cache_COUNT, ..., 4*sizeof(T))` -- MRAM writes must be a
    # multiple of 8 bytes, same alignment reason Gemv pads its own final row
    # batch). Real Sel's own element loop (Sel.py's always_head/always_tail)
    # uses pointer addressing for the input array without a fused branch
    # (same as Hst's element loop) -- _lower_sel's elem loop is marked
    # accordingly. sel.llvcnm's chunk-level structure, once that's applied:
    #   MUL_IDX(offset), DMA_LOAD_I32(chunk)         -- chunk prefix
    #   MOVE_IDX(local_count, 0), MOVE_IDX(j, 0)      -- chunk prefix
    #   .Lloop3 (repeated buffer_size times, but SEE BELOW):
    #     LOAD_I32(cache_in[j])                       -- no scale, pointer mode
    #     GT_I1(...); JEQ(...)                         -- predicate branch
    #     [then: SCALE_IDX, STORE_I32(cache_out[local_count]), local_count++]
    #     ADD_IDX(input ptr++); INC(j); JNEQ(...)      -- unfused close
    #   MUL_IDX(count_offset), DMA_STORE_I32(y, cache_out)
    #   SCALE_IDX, STORE_I32(cache_count[0], final count)
    #   DMA_STORE_I32(count, cache_count, size=4)
    #   INC(chunk_idx), JNEQ(...)
    #
    # NOT transcribed from this lowering's generic `cnm.if` (a fixed,
    # illustrative `value > 0` predicate that can't show real per-datatype
    # instruction-count differences anyway): the per-datatype predicate
    # fusion (extra_pred_ops) and the buffer_size//2 element-pairing trick
    # are both reused VERBATIM from Sel.py, exactly like ReduceOp/Scan/Hst's
    # Path B files reuse their own respective opaque-cost corrections
    # (extra_carry_adds, the skip/add-pair average, mulsi3_call) rather than
    # re-deriving a worse version of an already-validated measurement.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size
    COUNT_BLOCK = 4  # fixed at i32 regardless of data_type -- matches the
    # cnm dialect's own i32-fixed count buffer (see linalg_to_cnm.py's
    # module docstring: "Element type is fixed to i32 throughout"), not a
    # per-data-type real quantity like the input/output arrays are.

    extra_pred_ops = []
    if data_type == DataType.INT16:
        extra_pred_ops = [baseInsLUT[(InsType.ADD, DataType.INT32)]]  # the separate `and`
    elif data_type == DataType.INT64:
        extra_pred_ops = [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and (lo word)
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and (hi word)
            baseInsLUT[InsType.J],  # extra branch (hi word)
        ]
    always_head = [
        baseInsLUT[(InsType.L, data_type)],
        *extra_pred_ops,
        baseInsLUT[InsType.J],
    ]
    always_tail = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_INPUT pointer++
        baseInsLUT[InsType.J],  # loop test -- unfused, matches fuse_branch=False
    ]
    pass_extra = [
        baseInsLUT[InsType.LSL],  # SCALE_IDX for cache_out[local_count]
        baseInsLUT[(InsType.S, data_type)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # local_count++
    ]

    fail_element = always_head + always_tail
    pass_element = always_head + pass_extra + always_tail
    element_pair = fail_element + pass_element

    chunk_entry = [
        baseInsLUT[InsType.LSL],  # MUL_IDX(offset)
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],  # local_count init (fresh reg from zero_idx)
        baseInsLUT[InsType.MOV],  # j init
    ]
    chunk_tail = [
        baseInsLUT[InsType.LSL],  # MUL_IDX(count_offset)
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.LSL],  # SCALE_IDX(cache_count[0] address)
        baseInsLUT[(InsType.S, DataType.INT32)],  # store count into WRAM
        dmaLUT[(InsType.SDMA, COUNT_BLOCK * data_type_size(DataType.INT32))],
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # chunk_idx++
        baseInsLUT[InsType.J],
    ]

    chunk_body = chunk_entry + [element_pair] + chunk_tail
    sim_iterations = [local_iter_count, buffer_size // 2]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, chunk_body)
