import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Traced from the real -O3 assembly (dpu.s). The BUFFER_COUNT==0 branch
    # is dead code for real sweeps (buffer_size is always >= 4), so BB0_8/9
    # is the only path modeled. The inner predicate check is a genuine
    # data-dependent branch (pred(x) = x%2==0): with init_data's A[r]=r,
    # each chunk alternates fail(odd)/pass(even) perfectly, so this models
    # that alternation literally (deterministic for this exact,
    # deliberately-chosen data pattern) rather than as a statistical
    # average.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # BB0_9 entry (load + predicate branch) and BB0_11 tail (pointer++,
    # loop-continue check) execute for every element regardless of outcome.
    #
    # pred(x) = (x%2)==0 lowers very differently depending on how many
    # 32-bit registers x occupies:
    #   - uint32_t: `and r8, r7, 1, nz, .LBB0_11` -- a single FUSED
    #     and+conditional-branch instruction. This is what the 2-instruction
    #     [L, J] estimate below actually models (load + this one fused op).
    #   - uint16_t: `and r8, r7, 1` then a SEPARATE `jnz r8, .LBB0_11` --
    #     the fusion doesn't happen for a zero-extended half-word value, so
    #     this is 1 extra instruction (a plain AND) beyond the uint32_t
    #     shape.
    #   - uint64_t: the value spans two registers (hi/lo), so pred() needs
    #     to check ONE relevant bit but the compiler still emits an AND +
    #     branch for BOTH halves (`and`/`jneq` on the lo word, then another
    #     `and`/`jneq` on the hi word) -- 3 extra instructions (2 ANDs + 1
    #     extra branch) beyond the uint32_t shape.
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
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    # bb.10: only reached when pred() passes -- pack the value into
    # cache_OUTPUT and bump count.
    pass_extra = [
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.S, data_type)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
    ]

    fail_element = always_head + always_tail
    pass_element = always_head + pass_extra + always_tail
    element_pair = fail_element + pass_element

    # BB0_8 entry: once per chunk, before the element loop. mram_read of
    # this chunk's INPUT slice.
    chunk_entry = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
    ]
    # BB0_12 tail: once per chunk, after the element loop. Stores this
    # chunk's count, writes both cache_OUTPUT (BUFFER_SIZE) and cache_COUNT
    # (a fixed 4-element/16-byte block -- small enough that the compiler
    # bakes its size into the sdma immediate directly, no separate shift
    # calc), then advances all three addresses and the chunk counter.
    chunk_tail = [
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        dmaLUT[(InsType.SDMA, 16)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    chunk_body = chunk_entry + [element_pair] + chunk_tail

    sim_iterations = [local_iter_count, buffer_size // 2]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, chunk_body)
