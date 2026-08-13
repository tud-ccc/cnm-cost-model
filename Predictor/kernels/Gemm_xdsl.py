import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *


def TestOp(luts, thread_count, data_type, iteration, vec_length, n_length, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/gemm.llvcnm, the
    # same way Gemm.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly. NOT auto-converted. Same signature as Gemm.py's
    # TestOp.
    #
    # Built with Leaf/Repeat/Seq directly (not the simpler nested-list
    # convention every other *_xdsl.py file uses) because Gemm's real
    # structure has a zero-init loop that's a SIBLING of the k-chunk loop
    # inside the n-tile level, not one nested inside the other -- the flat
    # nested-list convention can only express one nested repeat per level.
    #
    # Real Gemm (Gemm/dpu/dpu.c) is a 4-level tiled algorithm
    # (row -> n_tile -> k_chunk -> col -> c), not a simpler 3-level
    # row/col/k structure, and B is stored TRANSPOSED on real hardware so
    # each logical column reads contiguously.
    #
    # The innermost c_loop is left at cnm.for's default (index-scaled,
    # unfused) addressing, not pointer mode like ReduceOp/Gemv's
    # 2-read/no-indexed-output loops: Gemm.py's real c_loop shows `LSL`
    # before every load and an unfused `ADD`+`J` close, because
    # cache_C[col] is written every iteration and the compiler can't prove
    # non-aliasing with cache_A/cache_B -- same shape as VectorOp's
    # index-scaled/unfused loop.
    #
    # Gemm.py's own c_loop is close enough to gemm.llvcnm's own c-level
    # loop to reuse directly (both: LSL, L, LSL, L, [multiply], ADD, ADD,
    # S, J) -- reused verbatim below, including its mulsi3_call (identical
    # B_T=1-pinning situation to Gemv's dot product: only 1 of 32 mul_step
    # copies ever executes, 8 dispatched instructions total regardless of
    # data_type).
    #
    # NOT modeled: real cache_C zeroing is a `memset()` library call; this
    # transcription uses a plain per-element zero-store loop instead,
    # matching what lowering/linalg_to_cnm.py's _lower_gemm actually
    # generates. Also not modeled: real Gemm spills far more
    # per-row/per-tile/per-chunk state to the stack than this lowering's
    # simpler index-based addressing ever needs.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    num_k_chunks = vec_length // buffer_size
    num_n_tiles = n_length // buffer_size

    # Reused verbatim from Gemm.py (identical to Gemv's -- same B_T=1
    # pinning situation).
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

    # --- c level: dot-product accumulation, once per element within a K-chunk ---
    c_step = Leaf(
        [
            baseInsLUT[InsType.LSL],  # address of cache_A[c]
            baseInsLUT[(InsType.L, data_type)],  # load cache_A[c]
            baseInsLUT[InsType.LSL],  # address of cache_B[c]
            baseInsLUT[(InsType.L, data_type)],  # load cache_B[c]
            *mulsi3_call,
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # accumulate
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # c++
            baseInsLUT[(InsType.S, DataType.INT32)],  # store cache_C[col]
            baseInsLUT[InsType.J],
        ]
    )
    c_loop = Repeat(buffer_size, c_step)

    # --- col level: once per column within the current N-tile ---
    col_prefix = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # col_idx = n_tile_base + col
        baseInsLUT[InsType.LSL],  # b_row_base = col_idx * K
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # b_offset
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],  # cache_B chunk
        baseInsLUT[InsType.LSL],  # address of cache_C[col]
        baseInsLUT[(InsType.L, DataType.INT32)],  # load cache_C[col] (running sum)
        baseInsLUT[InsType.MOV],  # c init
    ]
    col_suffix = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # col++
        baseInsLUT[InsType.J],
    ]
    col_body = Seq([Leaf(col_prefix), c_loop, Leaf(col_suffix)])
    col_loop = Repeat(buffer_size, col_body)

    # --- k-chunk level: once per K-chunk within the current N-tile ---
    k_chunk_prefix = [
        baseInsLUT[InsType.LSL],  # k_offset = k_chunk * CHUNK
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # a_offset = row_a_base + k_offset
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],  # cache_A chunk (once, reused across columns)
        baseInsLUT[InsType.MOV],  # col init
    ]
    k_chunk_suffix = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # k_chunk++
        baseInsLUT[InsType.J],
    ]
    k_chunk_body = Seq([Leaf(k_chunk_prefix), col_loop, Leaf(k_chunk_suffix)])
    k_chunk_loop = Repeat(num_k_chunks, k_chunk_body)

    # --- N-tile level: zero cache_C (sibling of the k-chunk loop, not
    # nested inside it), then the k-chunk loop, then flush cache_C out ---
    zero_step = Leaf(
        [
            baseInsLUT[InsType.LSL],  # address of cache_C[b]
            baseInsLUT[(InsType.S, DataType.INT32)],  # store 0
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # b++
            baseInsLUT[InsType.J],
        ]
    )
    zero_loop = Repeat(buffer_size, zero_step)

    n_tile_prefix_1 = [baseInsLUT[InsType.MOV]]  # zero constant init, once per tile
    n_tile_prefix_2 = [
        baseInsLUT[InsType.LSL],  # n_tile_base = n_tile * CHUNK
        baseInsLUT[InsType.MOV],  # k_chunk init
    ]
    n_tile_suffix = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # c_offset = row_c_base + n_tile_base
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],  # flush cache_C
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # n_tile++
        baseInsLUT[InsType.J],
    ]
    n_tile_body = Seq(
        [
            Leaf(n_tile_prefix_1),
            zero_loop,
            Leaf(n_tile_prefix_2),
            k_chunk_loop,
            Leaf(n_tile_suffix),
        ]
    )
    n_tile_loop = Repeat(num_n_tiles, n_tile_body)

    # --- row level: once per row ---
    row_prefix = [
        baseInsLUT[InsType.LSL],  # row_a_base = row * K
        baseInsLUT[InsType.LSL],  # row_c_base = row * N
        baseInsLUT[InsType.MOV],  # n_tile init
    ]
    row_suffix = [
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # row++
        baseInsLUT[InsType.J],
    ]
    row_body = Seq([Leaf(row_prefix), n_tile_loop, Leaf(row_suffix)])
    row_loop = Repeat(iteration, row_body)

    p = Simulator(thread_count)
    return p.runSegment(row_loop)
