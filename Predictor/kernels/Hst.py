import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *

# Fixed by Hst/run_script/configuration.py (not swept): BINS=256, DEPTH=12.
BINS = 256


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Traced from the real -O3 assembly (dpu.s). The BUFFER_COUNT==0 and
    # local_iter_count<=1 guards (BB0_6/7, BB0_15/16) are dead code for real
    # sweeps, so the modeled path below (BB0_10/11 + the one-time final
    # write) is the only one that matters.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # `d = (value*BINS)>>DEPTH` lowers to a real __mulsi3 call, since BINS is
    # a runtime dpu_arguments_t field rather than a compile-time constant
    # the compiler could fold into a shift. __mulsi3 swaps its operands so
    # the smaller of the two ends up in the bit-scanning register, then
    # unrolls 32 mul_step instructions with a data-dependent early exit once
    # that register is exhausted -- so the number of mul_steps dispatched
    # equals that operand's bit-length. host/app.c's init_data() keeps every
    # input value >= BIN_COUNT specifically so that operand is always the
    # constant BINS=256 (9-bit-length), pinning this to exactly 9 mul_steps
    # for every element -- the same "pin the data" trick Gemv uses with B=1.
    #
    # For INT64, `value` is promoted to 64-bit before the multiply, so the
    # compiler emits a call to __muldi3 instead -- a different routine, not
    # just a bigger version of __mulsi3. __mulsi3 costs ~176.4 cycles,
    # __muldi3 costs ~1421.6 cycles (~8x more, not merely 2x).
    #
    # __muldi3's cost is split into per-instruction-cost-sized J steps
    # rather than modeled as one opaque atomic Instruction: the Simulator's
    # round-robin dispatcher issues exactly one instruction per global
    # cycle-tick across all threads (predictor.py's runNested), so a single
    # atomic 1421-cycle Instruction would tell the dispatcher this thread is
    # unconditionally busy with no arbitration points, hiding the real
    # contention with the other 15 tasklets' own concurrent multiplies.
    # Splitting into J-sized steps (matching mulsi3_call's own granularity)
    # lets the existing round-robin machinery capture that contention.
    if data_type == DataType.INT64:
        # The Simulator's dispatcher floors each instruction's effective
        # thread-occupancy at 11 cycles regardless of its own declared
        # latency (predictor.py: `next_available = max(issue_cycle + 11,
        # completion_cycle)`) -- baseInsLUT[InsType.J]'s own latency (10)
        # is actually below that floor, so the real per-step cost here is
        # 11, not 10.
        effective_j_cost = max(11, baseInsLUT[InsType.J].latency)
        muldi3_steps = round(1421.6397476196289 / effective_j_cost)
        mulsi3_call = [baseInsLUT[InsType.J]] * muldi3_steps
    else:
        mulsi3_call = [
            baseInsLUT[InsType.J],  # call
            baseInsLUT[InsType.J],  # jgtu (swap-direction compare-branch)
            baseInsLUT[InsType.MOV],  # move (swap setup)
            baseInsLUT[InsType.MOV],  # move (swap setup)
            baseInsLUT[InsType.MOV],  # move r1, zero
            *([baseInsLUT[InsType.J]] * 9),  # 9 mul_step (compare-branch class)
            baseInsLUT[InsType.MOV],  # move result
            baseInsLUT[InsType.J],  # return jump
        ]

    # BB0_11: inner per-element loop, once per element within a chunk --
    # bin the value and increment cache_HST[d]. The shift+mask below
    # (lsr 10, and 0x3FFFFC) is the compiler folding ">>DEPTH then *4" into
    # a single shift-and-mask, specific to DEPTH=12.
    element_loop = [
        baseInsLUT[(InsType.L, data_type)],  # load cache_INPUT[c]
        baseInsLUT[InsType.MOV],  # move r1, BINS (arg setup)
        *mulsi3_call,
        baseInsLUT[InsType.LSR],
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # and (aliases ADD's cost)
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # address calc
        baseInsLUT[(InsType.L, DataType.INT32)],  # load cache_HST[d]
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # increment
        baseInsLUT[(InsType.S, DataType.INT32)],  # store cache_HST[d]
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_INPUT pointer++
        baseInsLUT[InsType.J],  # loop continue check
    ]

    # BB0_10 entry: once per chunk. mram_read of this chunk's INPUT slice.
    chunk_entry = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.L, DataType.INT32)],  # reload BUFFER_COUNT
    ]

    # bb.12: once per chunk. Advance addresses, check the chunk loop bound.
    chunk_tail = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],  # reload local_iter_count
        baseInsLUT[InsType.J],
    ]

    chunk_body = chunk_entry + [element_loop] + chunk_tail
    sim_iterations = [local_iter_count, buffer_size]

    # BB0_13: once per tasklet (not per chunk) -- the single mram_write of
    # this tasklet's full BINS-sized histogram, after the chunk loop exits.
    # Unlike ReduceOp/Gemv's tiny one-time tail writes, BINS*sizeof(T) is a
    # real, sizable transfer (1024 bytes for BINS=256/uint32_t), so it's
    # modeled explicitly rather than folded into base_time.
    final_write = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.L, DataType.INT32)],
        dmaLUT[(InsType.SDMA, BINS * data_type_size(DataType.INT32))],
    ]

    p = Simulator(thread_count)
    p.putInstructionsNested(sim_iterations, chunk_body)
    return p.runSegment(Leaf(final_write))
