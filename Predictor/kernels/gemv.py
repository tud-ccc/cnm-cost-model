import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *
from configuration import buffer_sizes


def _nearest_dma_instruction(dmaLUT, op_type, size_bytes):
    # The final partial-batch write (see remainder handling below) writes
    # however many bytes are left over, which won't generally land on one of
    # the calibrated buffer_sizes. Real DMA latency scales continuously with
    # size, so rounding up to the nearest calibrated entry is a reasonable
    # stand-in for an exact calibration point we don't have.
    for size in sorted(buffer_sizes):
        if size >= size_bytes:
            return dmaLUT[(op_type, size)]
    return dmaLUT[(op_type, max(buffer_sizes))]


def TestOp(luts, thread_count, data_type, iteration, vec_length, buffer_size):
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    num_col_chunks = vec_length // buffer_size
    num_row_batches = iteration // buffer_size
    remainder = iteration % buffer_size

    # __mulsi3(A, 1): call always lowers to a runtime routine call (never
    # inlined -- DPUInstrFormats.td marks MUL_STEP with Pattern=[]). With B
    # pinned to 1, the swap-to-smaller-operand puts 1 in the scan register,
    # so the first of 32 unrolled mul_step copies already shifts it to zero
    # and takes the data-dependent early exit (COND_Z) -- only 1 mul_step of
    # 32 ever executes. The real disassembly (librt_v1A.a) gives exactly 8
    # dispatched instructions per call: call, jgtu + 2x move (swap), move
    # r1,zero, 1x mul_step, move result, return jump.
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

    # Traced from the actual -O3 assembly (dpu.s) after decoupling the
    # output-write batching from the K-chunk size in dpu.c. That refactor
    # changed the compiler's codegen for the whole kernel (not just the
    # output path): the MAC loop now uses pointer-increment addressing
    # instead of indexed LSL+L, and per-row/per-chunk state gets spilled to
    # the stack and reloaded (higher register pressure from the new control
    # flow), both of which are real, measured differences from the old
    # 4-level batch/rr/chunk/c model. Every list below is named after the
    # basic block it was traced from.

    # BB0_15: MAC loop, once per column-chunk element
    c_loop = [
        baseInsLUT[(InsType.L, data_type)],  # load cache_A[c] (pointer)
        baseInsLUT[(InsType.L, data_type)],  # load cache_B[c] (pointer)
        *mulsi3_call,
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # sum += product
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_B pointer++
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_A pointer++
        baseInsLUT[InsType.J],  # loop branch
    ]

    # BB0_14 entry + BB0_16 continue path: once per column chunk. The two
    # mram_reads (A, B) share a single size-based shift computation.
    chunk_loop = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
        c_loop,
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    # BB0_12 entry (per-row register-spill housekeeping) + BB0_13 (chunk
    # loop init): once per row, before the chunk loop.
    row_head = [
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.J],
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.MOV],
    ]

    # BB0_21: store this row's sum into cache_C, bump pending_rows, check
    # whether this batch just became full. Shared by every row.
    row_tail_common = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.S, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    # bb.22: batch not yet full -- just reload two spilled values.
    non_flush_reload = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    # BB0_17: batch just became full -- the actual mram_write of cache_C,
    # plus write-pointer bump and pending_rows reset.
    flush_write = [
        baseInsLUT[InsType.MOV],
        baseInsLUT[InsType.LSR],
        baseInsLUT[InsType.LSL],
        baseInsLUT[(InsType.L, DataType.INT32)],
        dmaLUT[(InsType.SDMA, BUFFER_SIZE)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.MOV],
        baseInsLUT[(InsType.L, DataType.INT32)],
    ]

    # BB0_18: row loop increment / exit check. Shared by every row.
    row_loop_tail = [
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[(InsType.L, DataType.INT32)],
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    non_flush_row = (
        row_head + [chunk_loop] + row_tail_common + non_flush_reload + row_loop_tail
    )
    flush_row = (
        row_head + [chunk_loop] + row_tail_common + flush_write + row_loop_tail
    )

    # Nested fast-forwarding engine (see predictor.py's Leaf/Repeat/Seq):
    # non_flush_row/flush_row each have the SAME nested chunk_loop/c_loop
    # structure (num_col_chunks reps of chunk_loop, each with buffer_size
    # reps of c_loop) -- bodyFromLegacy converts that legacy nested-list
    # shape into a Leaf/Seq once, reused for both a row's own repeat level
    # and the exact same structure inside the one-off flush/final_write
    # row, letting the engine fast-forward EVERY level (num_row_batches,
    # rows-per-batch, num_col_chunks, buffer_size) independently instead of
    # only the outermost -- needed since num_row_batches is often small in
    # real sweeps while the inner dimensions can be huge (see gemv.csv:
    # nearly half the real sweep has num_row_batches <= 4).
    row_iterations = [num_col_chunks, buffer_size]
    non_flush_body = bodyFromLegacy(non_flush_row, row_iterations, level=0)
    flush_body = bodyFromLegacy(flush_row, row_iterations, level=0)

    p = Simulator(thread_count)
    result = None

    if num_row_batches > 0:
        # One batch = (buffer_size - 1) non-flush rows followed by the row
        # that triggers the mram_write flush -- matches dpu.c's real
        # control flow exactly (pending_rows counts up to BUFFER_COUNT,
        # then flushes).
        batch_body = Seq([Repeat(buffer_size - 1, non_flush_body), flush_body])
        result = p.runSegment(Repeat(num_row_batches, batch_body))

    if remainder > 0:
        # BB0_8/BB0_9: once the row loop exits, `remainder` rows are still
        # unflushed (dpu.c's pending_rows never reached BUFFER_COUNT for
        # these leftover rows, since fewer than BUFFER_COUNT rows remained).
        # Round up to an even element count for MRAM's 8-byte alignment
        # (mirrors dpu.c's write_count = pending_rows + pending_rows % 2),
        # then do one final mram_write of that size.
        write_count_elems = remainder + (remainder % 2)
        write_bytes = write_count_elems * data_type_size(data_type)
        final_write = [
            baseInsLUT[InsType.J],  # jeq pending_rows,0 (not taken)
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # and (aliases ADD's cost)
            baseInsLUT[(InsType.ADD, DataType.INT32)],  # add (write_count)
            baseInsLUT[InsType.LSL],  # lsl (bytes)
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.LSR],
            baseInsLUT[InsType.LSL],
            _nearest_dma_instruction(dmaLUT, InsType.SDMA, write_bytes),
        ]
        remainder_body = Seq([Repeat(remainder, non_flush_body), Leaf(final_write)])
        result = p.runSegment(remainder_body)

    return result
