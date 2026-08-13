import sys

sys.path.append("../")

from instructions import *
from support import *
from predictor import *

# Fixed by Hst/run_script/configuration.py (not swept): BINS=256, DEPTH=12.
BINS = 256


def TestOp(luts, thread_count, data_type, iter_per_thread, buffer_size):
    # Path B: hand-transcribed from CnmGen/output/llvcnm/hst.llvcnm, the
    # same way Hst.py's ins_list was hand-transcribed from the real
    # compiled -O3 assembly. NOT auto-converted. Same signature and same
    # two-call structure (chunk loop, then the explicit final histogram
    # write) as Hst.py's TestOp.
    #
    # Real Hst computes `(value*BINS)>>DEPTH` for the bin index (not a
    # modulo). The element loop's input access is `addressing="pointer"`
    # (Hst.py's real trace bumps cache_INPUT's pointer directly, no
    # per-access scale) -- but the trip-count check is a separate ADD+J,
    # not fused, unlike ReduceOp/Scan's pointer loops. This is why ForOp
    # tracks `addressing` and `fuse_branch` as two independent properties
    # (see dialects/cnm.py's ForOp docstring).
    #
    # hst.llvcnm's element loop, once the input access is pointer-mode, is
    # an almost exact structural match to Hst.py's own real element_loop --
    # both are: load input, [multiply-related bin-index work], shift,
    # [mask], address-calc, load histogram slot, increment, store histogram
    # slot, pointer bump, branch. The one piece the generated code's
    # MUL_I32 pseudo-op can't show: `value * BINS` is a RUNTIME multiply
    # (BINS isn't a compile-time constant), so it's a full
    # `__mulsi3`/`__muldi3` library call on real hardware, not one
    # instruction -- reused verbatim from Hst.py's own mulsi3_call/
    # muldi3-step-count logic below. Also reused: the mask op (Hst.py's
    # "AND (aliases ADD's cost)"), which this lowering's SHR_I32/
    # INDEX_CAST_IDX decomposition doesn't emit but real hardware needs.
    #
    # One more manual correction, not a literal transcription of
    # hst.llvcnm's own closing sequence: once the input access is
    # pointer-mode, the loop's own index register `j` has no remaining use
    # (it only ever indexed the now-pointer-based input access) -- true
    # strength reduction eliminates it entirely and tests the loop bound
    # against the bumped pointer itself, matching Hst.py's real trace (one
    # `ADD` pointer bump + one `J` loop test, no separate index increment).
    # This lowering's `_lower_for` can't yet prove a trip counter is fully
    # redundant and drop it, so its generated llvcnm still shows a
    # redundant `INC(j)`; this transcription omits it to match what real
    # optimized code actually does.
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]

    BUFFER_SIZE = buffer_size * data_type_size(data_type)
    local_iter_count = iter_per_thread // buffer_size

    # Reused verbatim from Hst.py -- see its own long comment for the full
    # microbenchmark story (__mulsi3 ~176 cycles / 9 mul_steps since BINS is
    # pinned to a 9-bit-length constant; __muldi3 ~1421 cycles, split into
    # per-instruction-cost-sized J steps so the round-robin dispatcher's
    # thread-contention modeling stays correct instead of hiding behind one
    # opaque atomic block).
    if data_type == DataType.INT64:
        effective_j_cost = max(11, baseInsLUT[InsType.J].latency)
        muldi3_steps = round(1421.6397476196289 / effective_j_cost)
        mulsi3_call = [baseInsLUT[InsType.J]] * muldi3_steps
    else:
        mulsi3_call = [
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.MOV],
            *([baseInsLUT[InsType.J]] * 9),
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.J],
        ]

    element_loop = [
        baseInsLUT[(InsType.L, data_type)],  # load cache_INPUT[c], pointer mode
        baseInsLUT[InsType.MOV],  # arg setup (move r1, BINS)
        *mulsi3_call,
        baseInsLUT[InsType.LSR],
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # mask fold (AND, aliases ADD's cost)
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # address calc for cache_HST[d]
        baseInsLUT[(InsType.L, DataType.INT32)],  # load cache_HST[d]
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # increment
        baseInsLUT[(InsType.S, DataType.INT32)],  # store cache_HST[d]
        baseInsLUT[(InsType.ADD, DataType.INT32)],  # cache_INPUT pointer++
        baseInsLUT[InsType.J],  # loop test (no separate index INC -- see note above)
    ]

    chunk_body = [
        baseInsLUT[InsType.LSL],
        dmaLUT[(InsType.LDMA, BUFFER_SIZE)],
        baseInsLUT[InsType.MOV],
        element_loop,
        baseInsLUT[(InsType.ADD, DataType.INT32)],
        baseInsLUT[InsType.J],
    ]

    final_write = [
        baseInsLUT[InsType.MOV],
        dmaLUT[(InsType.SDMA, BINS * data_type_size(DataType.INT32))],
    ]

    p = Simulator(thread_count)
    sim_iterations = [local_iter_count, buffer_size]
    p.putInstructionsNested(sim_iterations, chunk_body)
    return p.runSegment(Leaf(final_write))
