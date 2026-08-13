import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, operation, thread_count, data_type, iterations):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/vecadd.llvcnm (the
    # xDSL linalg->cnm->llvcnm pipeline's generated code for VecAdd), the
    # same way VectorOp.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly. NOT auto-converted -- there is no llvcnm parser
    # here, this is a line-by-line manual translation, kept in its own file
    # so the same benchmark can be run through either instruction sequence
    # and produce two separate predictions/CSVs for comparison.
    #
    # vecadd.llvcnm, per chunk (num_chunks = iterations[0], CHUNK = 128 in
    # the generated file, standing in for buffer_size = iterations[1] here):
    #
    #   MOVE_IDX(V8, 128)              -- chunk-size constant (re-emitted
    #                                      every chunk in the generated code;
    #                                      real -O3 would hoist this, but
    #                                      it's genuinely in the generated
    #                                      listing, so it's counted)
    #   MUL_IDX(V9, V7, V8)            -- offset = chunk_idx * CHUNK
    #   DMA_LOAD_I32(V3, V0[V9], 128)
    #   DMA_LOAD_I32(V4, V1[V9], 128)
    #   MOVE_IDX(V10, 128)             -- element-loop upper bound constant
    #   MOVE_IDX(V11, 0)               -- element index init
    #   .Lloop3 (repeated buffer_size times):
    #     SCALE_IDX(V12, V11)           -- index -> byte address, before EVERY
    #                                      load/store (see mapping decisions)
    #     LOAD_I32(V13, V3[V11])
    #     SCALE_IDX(V14, V11)
    #     LOAD_I32(V15, V4[V11])
    #     ADD_I32(V16, V13, V15)
    #     SCALE_IDX(V17, V11)
    #     STORE_I32(V16, V5[V11])
    #     INC(V11)
    #     JNEQ(V11, V10, .Lloop3)
    #   DMA_STORE_I32(V5, V2[V9], 128)
    #   INC(V7)
    #   JNEQ(V7, V6, .Lloop1)
    #
    # Mapping decisions:
    #   - MOVE_IDX/MOV -> InsType.MOV. LDMA/SDMA/LOAD_I32/STORE_I32 map
    #     directly to their InsType equivalents, keyed by data_type same as
    #     VectorOp.py.
    #   - MUL_IDX(idx, CHUNK) -> InsType.LSL, not InsType.MUL: CHUNK is a
    #     compile-time power-of-two constant, so a real backend lowering
    #     this multiply would emit a shift, the same idiom real VectorOp
    #     assembly already uses for its own address arithmetic (`lsl_add`).
    #     Using MUL here would overstate the cost of something that's really
    #     free-form virtual-ISA index arithmetic, not an actual runtime
    #     multiply.
    #   - ADD_I32 (the compute op) -> (operation, data_type), same
    #     substitution VectorOp.py itself makes: the generated file only
    #     ever models "add" (build_vecadd.py's ComputeOp("add", ...)), but
    #     this stands in for whatever operation/data_type is being swept, so
    #     the two paths stay comparable across the full sweep rather than
    #     just the one "add" row the generated file literally shows.
    #   - INC(idx) -> (ADD, INT32): index-register bumps are always 32-bit
    #     regardless of the kernel's data_type, same convention VectorOp.py
    #     uses for its own pointer-increment ADDs.
    #   - JNEQ -> InsType.J.
    #   - SCALE_IDX -> InsType.LSL, same reasoning as MUL_IDX above: this is
    #     cnm_to_llvcnm.py's `_emit_addr_scale` -- real UPMEM needs an
    #     explicit index-to-byte-address computation before every
    #     load/store, which LOAD_I32/STORE_I32 don't otherwise account for.
    #
    # Remaining structural difference from VectorOp.py's real trace, not
    # hidden: this ins_list still has 1 chunk-level increment (chunk_idx)
    # where the real trace has 4 (one per bumped pointer register), since
    # llvcnm's index-based addressing never introduces separate pointer
    # registers to bump in the first place -- a real, smaller remaining gap.
    BUFFER_SIZE = iterations[1] * data_type_size(data_type)
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    ins_list = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        [
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.L, data_type)],
            baseInsLUT[(operation, data_type)],
            baseInsLUT[InsType.LSL],
            baseInsLUT[(InsType.S, data_type)],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[InsType.J],
        ],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    p = Simulator(thread_count)
    return p.putInstructionsNested(iterations, ins_list)
