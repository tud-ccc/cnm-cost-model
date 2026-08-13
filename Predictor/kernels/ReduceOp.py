import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # ADD only for now. Traced from the real -O3 assembly (dpu.s): the
    # BUFFER_COUNT==0 branch (BB0_13/14) is dead code for real sweeps
    # (buffer_size is always >= 4), so the modeled path below (BB0_8/BB0_9)
    # is the only one that matters. One-time setup and the single final
    # mram_write (once per tasklet, not per iteration) are left unmodeled,
    # same as Gemv's precedent -- negligible, absorbed into base_time.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # BB0_9: inner reduction loop, once per element within a chunk --
    # result += cache_A[c] + cache_B[c], pointer-increment addressing.
    # For a 64-bit data type (uint64_t/INT64), the two logical adds (A[c]+B[c]
    # and result+=...) each lower to a pair of real instructions on this
    # 32-bit ALU -- add (low word) then addc (high word, propagating carry) --
    # instead of one (`ld`/`add`/`addc` for INT64 vs `lw`/`add` for INT32).
    #
    # The extra cost here is NOT just "one more instruction": an isolated,
    # non-chained 64-bit add+addc pair costs about the same as a single
    # 32-bit add (addc alone is nearly free when nothing depends on the
    # previous iteration's result). This loop is different: `result` is a
    # loop-carried accumulator, so each iteration's addc genuinely depends
    # on the previous iteration's fully resolved carry, forcing real
    # serialization that an isolated add/addc pair never pays. The two
    # chained logical additions cost ~22 cycles for INT32 (2x the calibrated
    # ADD(INT32) cost, no penalty) vs ~55 cycles for INT64 -- a ~33 cycle
    # gap, i.e. 3 extra ADD(INT32)-cost units, not 2. There's no separate
    # ADDC/chained-accumulator entry in the instruction LUT (and it
    # shouldn't go there -- this penalty is specific to this kernel's
    # accumulator pattern, not a general property of INT64 addition;
    # VectorOp's independent, non-accumulating elementwise add doesn't pay
    # it and is already accurate using the LUT's plain ADD(INT64) value).
    extra_carry_adds = [baseInsLUT[(InsType.ADD, DataType.INT32)]] * 3 if data_type == DataType.INT64 else []
    c_loop = (
        [
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
        ]
        + extra_carry_adds
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[InsType.J],
        ]
    )

    # BB0_8 entry + bb.10 tail: once per chunk. The two mram_reads (A, B)
    # share a single size-based shift computation, same pattern as Gemv's
    # chunk_loop.
    chunk_loop = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        c_loop,
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    sim_iterations = [local_iter_count, buffer_size]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, chunk_loop)
