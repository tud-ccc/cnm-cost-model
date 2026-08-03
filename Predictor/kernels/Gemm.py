import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iteration, vec_length, n_length, buffer_size):
    # Traced from the real -O3 assembly (dpu.s). All the "BUFFER_COUNT >
    # dimension" guards (BB0_5's N check, BB0_7's K check, BB0_8/10's
    # buffer-count-vs-K degenerate path) are dead code for real sweeps,
    # since run_benchmark.py already skips any config where buffer_size
    # exceeds K or N -- so the nested row/n_tile/k_chunk/col/c structure
    # below is the only path that matters.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    num_k_chunks = vec_length // buffer_size
    num_n_tiles = n_length // buffer_size

    # __mulsi3(A[c], B=1): identical situation to Gemv's dot product -- B_T
    # is pinned to INIT_DATA=1, so the swap-to-smaller-operand puts 1 in the
    # bit-scanning register and only 1 of 32 unrolled mul_step copies ever
    # executes (data-dependent early exit). Traced disassembly gives exactly
    # 8 dispatched instructions per call, same as Gemv's.
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

    # BB0_14: innermost dot-product loop, once per element within a K-chunk.
    # The compiler can't prove cache_C doesn't alias cache_A/cache_B, so it
    # stores the running sum back to cache_C[col] every iteration instead of
    # keeping it purely in a register until the end.
    c_loop = [
        baseInsLUT[InsType.LSL],  # address of cache_A[c]
        baseInsLUT[(InsType.L, data_type)],  # load cache_A[c]
        baseInsLUT[InsType.LSL],  # address of cache_B[c]
        baseInsLUT[(InsType.L, data_type)],  # load cache_B[c]
        *mulsi3_call,
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # accumulate
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # c++
        baseInsLUT[(InsType.S, DataType.INT32)],  # store cache_C[col]
        baseInsLUT[InsType.J],  # loop continue check
    ]

    # BB0_13 entry (mram_read of this col's K-chunk of B, plus loading the
    # running cache_C[col] value) + bb.15 tail (col++, advance row_addr_B by
    # K*sizeof(T)): once per column within the current N-tile.
    col_entry = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
    ]
    col_tail = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    col_body = col_entry + [c_loop] + col_tail

    # BB0_12 entry (mram_read of this row's K-chunk of A, once -- reused
    # against every column in the current N-tile below) + bb.16 tail
    # (advance chunk_addr_A/k_chunk_addr_B by BUFFER_SIZE, k_chunk++): once
    # per K-chunk within the current N-tile.
    k_chunk_entry = [
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.LSL],
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.S, DataType.INT32)],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
    ]
    k_chunk_tail = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    k_chunk_body = k_chunk_entry + [col_body] + k_chunk_tail

    # BB0_7 entry: once per N-tile. cache_C[0..BUFFER_COUNT) is zeroed via a
    # real memset() call (BUFFER_COUNT isn't a compile-time constant, so the
    # compiler can't inline/unroll it) -- disassembled librt's memset.c.obj
    # to trace its real cost: with cache_C's mem_alloc'd pointer 4-aligned
    # and BUFFER_SIZE always a multiple of 4, it always takes the aligned
    # fast path (no byte-at-a-time prefix/suffix), which is a fixed 15-
    # instruction setup/teardown plus a 3-instruction word-store loop
    # (store, pointer++, decrement-and-branch) repeated BUFFER_COUNT times.
    memset_word_loop = [
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ] * buffer_size
    memset_call = (
        [
            baseInsLUT[InsType.J],  # call r23, memset -- previously uncounted
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and (aliases ADD)
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.J],  # jltu (not taken)
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and
            baseInsLUT[InsType.LSL],
            baseInsLUT[InsType.LSL],
            baseInsLUT[InsType.LSL],
            baseInsLUT[InsType.LSR],
            baseInsLUT[InsType.MOV],
        ]
        + memset_word_loop
        + [
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[InsType.J],  # jnz (not taken)
            baseInsLUT[InsType.J],  # unconditional jump
            baseInsLUT[InsType.J],  # return
        ]
    )
    n_tile_entry = (
        [
            baseInsLUT[(InsType.S, DataType.INT32)],
            baseInsLUT[(InsType.S, DataType.INT32)],
            baseInsLUT[InsType.MOV],
            baseInsLUT[(InsType.L, DataType.INT32)],
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.MOV],
        ]
        + memset_call
        + [
            baseInsLUT[(InsType.L, DataType.INT32)],
            baseInsLUT[(InsType.L, DataType.INT32)],
        ]
    )

    # BB0_18: once per N-tile -- mram_write of this tile's BUFFER_COUNT
    # results, plus advancing write_addr_C/ntile_row0_addr_B and n_tile++.
    n_tile_tail = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.L, DataType.INT32)],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    n_tile_body = n_tile_entry + [k_chunk_body] + n_tile_tail

    # bb.6: once per row -- reset n_tile=0 and ntile_row0_addr_B to B's
    # start. BB0_19: once per row -- row_addr_A += K*sizeof(T), row++.
    row_entry = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    row_tail = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]
    row_body = row_entry + [n_tile_body] + row_tail

    sim_iterations = [iteration, num_n_tiles, num_k_chunks, buffer_size, buffer_size]

    p = Simulator(thread_count)
    return p.putInstructionsNested(sim_iterations, row_body)
