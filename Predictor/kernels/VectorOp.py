import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, operation, thread_count, data_type, iterations):
    # Traced from the real -O3 assembly (uint32_t, ADD): move, lsr_add,
    # lsl_add, ldma, lsl_add, ldma, move, [lsl_add, lw, lsl_add, lw, add,
    # lsl_add, add, sw, branch], move, lsr_add, lsl_add, sdma, add, add,
    # add, add, branch. `iterations` is [num_chunks, buffer_size] (chunk
    # count outer, element-per-chunk count inner), matching how
    # run_benchmark.py sweeps iteration*buffer_size as ITER_PER_THREAD.
    BUFFER_SIZE = iterations[1] * data_type_size(data_type)
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    ins_list = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        [
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[(operation, data_type)],
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[(InsType.S, data_type)],
            baseInsLUT[InsType.J],
        ],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    p = Simulator(thread_count)
    return p.putInstructionsNested(iterations, ins_list)
