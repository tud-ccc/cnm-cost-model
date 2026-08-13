from support import *
from configuration import *
from load_lut import *


# ---------------------------------------------------------------------------
# Nested fast-forwarding segment tree (Leaf/Repeat/Seq) + bytecode engine.
#
# putInstructionsFast/runRepeating (below, in Simulator) can only
# fast-forward the OUTERMOST repeat of a nested kernel program -- every level
# below it still gets fully unrolled into one flat `unit_program`, which is
# exactly the case that blows up for kernels like Gemv: num_row_batches is
# often small (so there's no room to detect a steady completion cadence),
# while num_col_chunks/buffer_size (the INNER levels) can be huge, making
# even one "unit" impossibly expensive to materialize.
#
# This engine compiles a nested loop structure (Leaf/Repeat/Seq) into a flat
# bytecode array with explicit loop_start/loop_end markers instead of
# unrolling repeats, so thread.pc stays a small index regardless of how many
# times any level repeats. Each thread tracks its own per-loop iteration
# counter, and convergence/extrapolation is detected independently AT EVERY
# LOOP, per thread:
#   - "within" convergence: this loop's own per-repeat completion cadence
#     (for this thread) has stabilized -- skip its remaining repeats.
#   - "invocation" convergence: across repeated VISITS to this same loop
#     (e.g. once per row), the total cost per visit has stabilized -- skip
#     the whole loop instantly next time, without dispatching anything.
# The second case is what makes small-but-frequently-visited inner loops
# (Gemv's chunk_loop/c_loop, visited once per row) cheap even when no single
# visit has enough repeats to converge on its own.
#
# Loops containing a BARRIER are never extrapolated (contains_barrier check
# at compile time) -- skipping a thread past a barrier it hasn't physically
# reached would break the other threads waiting on it.
# ---------------------------------------------------------------------------


class Leaf:
    __slots__ = ("instructions",)

    def __init__(self, instructions):
        self.instructions = instructions


class Repeat:
    __slots__ = ("count", "body")

    def __init__(self, count, body):
        self.count = count
        self.body = body


class Seq:
    __slots__ = ("parts",)

    def __init__(self, parts):
        self.parts = parts


def _segment_from_legacy(iterations, instructions, level=0):
    # Auto-converts the existing (iterations, instructions) convention
    # (a flat list, repeating iterations[level] times, containing at most
    # one nested list which repeats iterations[level+1] times, and so on)
    # into a Leaf/Repeat/Seq tree -- lets existing single-chain kernels
    # (ReduceOp, VectorOp, etc.) use the new engine with no code changes.
    prefix = []
    nested = None
    suffix = []
    for ins in instructions:
        if isinstance(ins, list):
            nested = ins
        elif nested is None:
            prefix.append(ins)
        else:
            suffix.append(ins)
    if nested is not None:
        inner = _segment_from_legacy(iterations, nested, level + 1)
        body = Seq([Leaf(prefix), inner, Leaf(suffix)])
    else:
        body = Leaf(prefix)
    return Repeat(iterations[level], body)


def bodyFromLegacy(instructions, iterations, level=0):
    # Like _segment_from_legacy, but returns just the BODY (Leaf/Seq),
    # without wrapping `instructions` itself in an outer Repeat -- for
    # kernels where a repeat isn't uniform (e.g. Gemv's "buffer_size-1
    # ordinary rows then one different flush row": each row's own body has
    # the same nested chunk_loop/c_loop structure, but the row-level
    # repeat itself needs a Repeat+one-off Seq built by hand, not a plain
    # iterations[level] count). Use this to get one row's body as a
    # Leaf/Seq, then wrap the (buffer_size - 1) uniform rows in your own
    # Repeat and append the special last row as a Seq part.
    prefix = []
    nested = None
    suffix = []
    for ins in instructions:
        if isinstance(ins, list):
            nested = ins
        elif nested is None:
            prefix.append(ins)
        else:
            suffix.append(ins)
    if nested is not None:
        inner = _segment_from_legacy(iterations, nested, level)
        return Seq([Leaf(prefix), inner, Leaf(suffix)])
    return Leaf(prefix)


def _segment_contains_barrier(segment):
    if isinstance(segment, Leaf):
        return any(ins.op_type == InsType.BARRIER for ins in segment.instructions)
    if isinstance(segment, Seq):
        return any(_segment_contains_barrier(part) for part in segment.parts)
    if isinstance(segment, Repeat):
        return _segment_contains_barrier(segment.body)
    return False


def _segment_contains_dma(segment):
    # A loop containing DMA traffic shares self.next_available_dma (a
    # single global queue) with every other thread -- extrapolating past
    # it without also advancing next_available_dma would leave that queue
    # stale, corrupting the timing of every OTHER thread's subsequent real
    # DMA dispatch. See runNested's has_dma handling for how this is
    # tracked and applied.
    if isinstance(segment, Leaf):
        return any(
            ins.op_type in (InsType.LDMA, InsType.SDMA) for ins in segment.instructions
        )
    if isinstance(segment, Seq):
        return any(_segment_contains_dma(part) for part in segment.parts)
    if isinstance(segment, Repeat):
        return _segment_contains_dma(segment.body)
    return False


def _segment_dma_service_sum(segment):
    # Total DMA queue service time (sum of ins.service_interval) incurred
    # by ONE pass through `segment` -- used as an analytical (not
    # statistically detected) per-repeat next_available_dma advancement
    # for multi-threaded DMA-carrying loops. In a saturated queue (the
    # normal case once more than one thread shares it -- aggregate demand
    # against a fixed-capacity resource), next_available_dma advances by
    # EXACTLY service_interval per request, deterministically; there's
    # nothing to detect. Recurses into nested Repeats, multiplying by
    # their own count, so a loop containing further nested DMA-carrying
    # loops still gets the right total.
    if isinstance(segment, Leaf):
        return sum(
            ins.service_interval
            for ins in segment.instructions
            if ins.op_type in (InsType.LDMA, InsType.SDMA)
        )
    if isinstance(segment, Seq):
        return sum(_segment_dma_service_sum(part) for part in segment.parts)
    if isinstance(segment, Repeat):
        return segment.count * _segment_dma_service_sum(segment.body)
    return 0


def _deepest_leaf_instructions(segment, depth=0):
    if isinstance(segment, Leaf):
        return depth, segment.instructions
    if isinstance(segment, Repeat):
        return _deepest_leaf_instructions(segment.body, depth + 1)
    if isinstance(segment, Seq):
        best = (depth, [])
        for part in segment.parts:
            d, ins = _deepest_leaf_instructions(part, depth + 1)
            if d > best[0] and ins:
                best = (d, ins)
        return best
    return depth, []


def compileSegment(segment):
    # Flattens a Leaf/Repeat/Seq tree into a small bytecode array: real
    # instructions become ("ins", instruction); each Repeat becomes a
    # ("loop_start", loop_id) / ("loop_end", loop_id, body_start_pc) pair
    # wrapping its body, contributing O(1) ops regardless of `count`.
    ops = []
    loop_meta = {}
    next_loop_id = [0]

    def emit(seg):
        if isinstance(seg, Leaf):
            for ins in seg.instructions:
                ops.append(("ins", ins))
        elif isinstance(seg, Seq):
            for part in seg.parts:
                emit(part)
        elif isinstance(seg, Repeat):
            if seg.count <= 0:
                return
            loop_id = next_loop_id[0]
            next_loop_id[0] += 1
            has_barrier = _segment_contains_barrier(seg.body)
            has_dma = _segment_contains_dma(seg.body)
            dma_service_per_repeat = _segment_dma_service_sum(seg.body) if has_dma else 0
            body_start = len(ops)
            ops.append(("loop_start", loop_id))
            emit(seg.body)
            ops.append(("loop_end", loop_id, body_start + 1))
            loop_meta[loop_id] = {
                "count": seg.count,
                # Barrier-carrying loops are never extrapolated: skipping a
                # thread past a barrier it hasn't physically reached would
                # desync it from other threads still waiting there.
                #
                # DMA-carrying loops CAN be extrapolated, but need extra
                # care: self.next_available_dma is a single queue shared by
                # every thread, so skipping DMA dispatches must ALSO
                # advance that shared counter by the right amount, not
                # just this thread's own next_available. runNested uses
                # TWO different strategies for this depending on
                # thread_count (see has_dma handling there):
                #   - thread_count == 1: per-thread empirical moving-average
                #     detection (like completion time) -- validated correct
                #     since there's no other thread to confound it.
                #   - thread_count > 1: the ANALYTICAL dma_service_per_repeat
                #     computed here, not detected at all. A per-thread
                #     empirical measurement of next_available_dma's
                #     advancement is fundamentally confounded once other
                #     threads share the queue: between one thread's own
                #     consecutive repeats, however many OTHER threads'
                #     DMA requests happen to get interleaved (via
                #     round-robin dispatch) also land in that thread's
                #     "delta" -- and that interleaving count isn't stable
                #     over the run, since different threads converge and
                #     start skipping their own real dispatches at
                #     different times. No amount of averaging fixes a
                #     non-stationary quantity. The analytical value
                #     sidesteps this entirely: in a saturated queue (the
                #     expected case once more than one thread shares it),
                #     next_available_dma advances by EXACTLY
                #     service_interval per request, deterministically -- see
                #     runSegment's own copy of this comment for the same
                #     reasoning applied there.
                "extrapolatable": not has_barrier,
                "has_dma": has_dma,
                "dma_service_per_repeat": dma_service_per_repeat,
                "end_pc": len(ops) - 1,
            }
        else:
            raise TypeError(f"unknown segment type {type(seg)}")

    emit(segment)
    ops.append(("halt",))
    return ops, loop_meta


def _detect_steady_rate(history, count_so_far, max_period_multiple, base_period=1, TOL=1e-6):
    # Tries candidate periods P=base_period*k for k=1..max_period_multiple,
    # requiring the last two P-sized groups of a population-wide history to
    # match (twice, not just once, to avoid an early coincidental match)
    # before trusting a steady rate. Returns cycles-per-UNIT-STEP (s2/P) --
    # correct when `history` is a single running total and the caller will
    # multiply by a population-wide remaining-unit COUNT (e.g. runRepeating:
    # total remaining completions across every thread). NOT correct for a
    # per-thread quantity -- runNested's within/invocation tracking uses
    # _detect_average_rate for that case instead.
    def close(a, b):
        return abs(a - b) <= TOL * max(1.0, abs(a), abs(b))

    def shift(a, b):
        # Uniform per-index shift needed to advance window `a` to the
        # immediately-following window `b` (same length) -- None if the
        # shift isn't consistent across every index (catches any sub-period
        # wobble within one candidate P). NOTE: for P=1, each window is a
        # single value, so this alone is *always* "consistent" (nothing to
        # cross-check within one pair) -- callers MUST additionally compare
        # the shift from (g1,g2) against the shift from (g2,g3) themselves,
        # not just trust either pair in isolation, or a P=1 candidate would
        # "converge" off of any 3 arbitrary values.
        deltas = [b[i] - a[i] for i in range(len(a))]
        if not all(close(d, deltas[0]) for d in deltas):
            return None
        return deltas[0]

    for k in range(1, max_period_multiple + 1):
        P = base_period * k
        if count_so_far % P != 0 or len(history) < 3 * P:
            continue
        g1 = history[-3 * P : -2 * P]
        g2 = history[-2 * P : -P]
        g3 = history[-P:]

        s1 = shift(g1, g2)
        s2 = shift(g2, g3)
        if s1 is not None and s2 is not None and close(s1, s2):
            return s2 / P
    return None


def _detect_average_rate(samples, base_window, tol=0.02, max_scale=1024):
    # Shared convergence check for runNested's within-tracking (per-thread
    # repeat-to-repeat deltas) and invocation-tracking (pooled per-
    # invocation durations): rather than demanding an EXACT repeating
    # pattern (a fixed small period with every delta matching precisely),
    # this asks whether the MOVING AVERAGE has stabilized -- tolerating
    # noisy-but-stationary contention that never settles into any clean
    # small-integer period at all (e.g. a loop nested inside a
    # non-extrapolated DMA loop, where threads drift in and out of phase
    # with each other depending on where each currently sits in ITS OWN
    # DMA wait, producing per-repeat costs that swing 20-30% call to call
    # with no exact repeating pattern -- but a stable mean, which is all a
    # linear extrapolation needs).
    #
    # This doubles the window (starting from `base_window`, which callers
    # scale generously with thread_count -- see runNested) and asks a
    # scale-invariance question at each step: does avg(last W) still equal
    # avg(last 2W)? If the process has truly reached steady state,
    # widening the lookback shouldn't move the average (the additional
    # older samples have the same mean as the recent ones). If it's still
    # drifting, the older half pulls the combined average away from the
    # recent one, correctly failing this scale and trying a larger one.
    #
    # A "require several consecutive passing scales, starting from a small
    # base" version was tried (to let easy, already-stable loops converge
    # sooner instead of every loop paying for a large starting window) and
    # reverted: it can be WORSE than plain exact simulation for genuinely
    # hard cases, since a borderline quantity that occasionally fails one
    # intermediate scale check resets the whole streak, forcing repeated,
    # expensive O(window) resummation over and over with the streak never
    # reaching its threshold. A single large starting window costs more
    # for easy cases but degrades predictably, not catastrophically, for
    # hard ones.
    def close(a, b):
        return abs(a - b) <= tol * max(1.0, abs(a), abs(b))

    scale = 1
    while True:
        w = base_window * scale
        if 2 * w > len(samples) or scale > max_scale:
            return None
        avg_small = sum(samples[-w:]) / w
        avg_large = sum(samples[-2 * w :]) / (2 * w)
        if close(avg_small, avg_large):
            return avg_small
        scale *= 2


def process_block(instructions, additional_latency, thread_count):
    latency = 0
    for ins in instructions:
        if ins.op_type == InsType.LDMA or ins.op_type == InsType.SDMA:
            if thread_count > 1:
                additional_latency = (thread_count - 1) * ins.latency
            else:
                latency += ins.latency
        else:
            latency += ins.latency
            additional_latency = max(0, additional_latency - ins.latency)
    return additional_latency, latency


def predict(iterations, instructions, thread_count):
    curr_iter = 1
    additional_latency = 0
    latency = 0
    for i in range(len(iterations)):
        curr_iter *= iterations[i]
        curr_loop_ins = instructions[i]

        additional_latency, curr_loop_latency = process_block(
            curr_loop_ins, additional_latency, thread_count
        )
        latency += curr_loop_latency * curr_iter
        if i == (len(iterations) - 1) and additional_latency != 0:
            latency += additional_latency * curr_iter

    return latency


class Pipeline_ins:
    def __init__(self, ins, thread):
        self.ins = ins
        self.thread = thread


class ThreadState:
    def __init__(self, id):
        self.last_cycle = -1
        self.blocked_until = -1
        self.id = id

    def exec(self, placed_cycle, instruction_latency):
        self.last_cycle = placed_cycle + instruction_latency

    def available(self, cycle):
        return self.last_cycle < cycle


class DPU:
    def __init__(self, thread_count):
        self.thread_count = thread_count
        self.stage_count = 14
        self.instructions_to_execute = []
        self.threads_states = [ThreadState(x) for x in range(thread_count)]
        self.current_cycle = 0
        self.waiting_queue = []
        self.dma_free = True
        self.dma_blocked_until = -1

    def printStatus(self):
        for thread in self.threads_states:
            print(thread.id, thread.last_cycle)
        print("last cycle ", self.current_cycle)

    def putInstructions(self, iterations, instructions):
        pipeline_status_per_instruction = 14
        latency_per_instruction = 0
        self.current_cycle = 1
        level = 1
        for ins in instructions[level]:
            done_nothing = True
            for element in self.waiting_queue:
                if element[1].op_type not in [InsType.LDMA, InsType.SDMA]:
                    if element[0].available(self.current_cycle):
                        element[0].exec(self.current_cycle, element[1].latency)
                        self.current_cycle += 1
                        done_nothing = False
                        self.waiting_queue.remove(element)
                else:
                    if element[0].available(self.current_cycle):
                        if self.dma_blocked_until < self.current_cycle:
                            self.dma_blocked_until = self.current_cycle + ins.latency
                            self.current_cycle += 1
                            done_nothing = False
                            self.waiting_queue.remove(element)

            if ins.op_type not in [InsType.LDMA, InsType.SDMA]:
                for thread in self.threads_states:
                    if thread.available(self.current_cycle):
                        thread.exec(self.current_cycle, ins.latency)
                        self.current_cycle += 1
                        done_nothing = False
                    else:
                        self.waiting_queue.append((thread, ins))
            else:
                for thread in self.threads_states:
                    if thread.available(self.current_cycle):
                        if self.dma_blocked_until < self.current_cycle:
                            self.dma_blocked_until = self.current_cycle + ins.latency
                            self.current_cycle += 1
                            done_nothing = False
                    else:
                        self.waiting_queue.append((thread, ins))

            if done_nothing:
                self.current_cycle += 1
        while len(self.waiting_queue) != 0:
            done_nothing = True
            for element in self.waiting_queue:
                if element[0].available(self.current_cycle):
                    element[0].exec(self.current_cycle, element[1].latency)
                    self.current_cycle += 1
                    done_nothing = False
                    self.waiting_queue.remove(element)
            if done_nothing:
                self.current_cycle += 1

        self.printStatus()


class Simulator:
    class Thread:
        def __init__(self, id):
            self.id = id
            self.pc = 0
            self.next_available = 1

    def __init__(self, thread_count):
        self.thread_count = thread_count
        self.threads = [self.Thread(x) for x in range(thread_count)]
        self.next_available_dma = 0
        # Carried state for runRepeating, so multiple calls on the same
        # Simulator compose correctly (e.g. Gemv's batch + remainder,
        # Scan's reduce + offset-loop + scan): the phase-offset stagger
        # must happen only once (at the very first segment), and the
        # cycle count must keep accumulating rather than restart.
        self.last_completion_cycle = 0
        self._phase_offset_done = False

    def expandProgram(self, iterations, instructions, level=0):
        program = []

        for _ in range(iterations[level]):
            for ins in instructions:
                if isinstance(ins, list):
                    program.extend(
                        self.expandProgram(iterations, ins, level + 1)
                    )
                else:
                    program.append(ins)

        return program

    def deepestInstructionList(self, instructions):
        # Find the innermost repeating instruction list (the deepest nested
        # loop body). Used to estimate one thread's natural cycle time for a
        # single pass through the hottest loop.
        for ins in instructions:
            if isinstance(ins, list):
                return self.deepestInstructionList(ins)
        return instructions

    def putInstructions(self, iterations, instructions):
        program = self.expandProgram(iterations, instructions)
        innermost = self.deepestInstructionList(instructions)
        return self.runProgram(program, innermost)

    def _expandOneUnit(self, iterations, instructions, level):
        # Like expandProgram, but expands `instructions` a single time at
        # `level` instead of repeating it iterations[level] times -- nested
        # lists within it still expand fully according to
        # iterations[level+1:]. This is "one pass through the outermost
        # repeat", used by putInstructionsFast to avoid ever materializing
        # all `iterations[0]` copies.
        unit = []
        for ins in instructions:
            if isinstance(ins, list):
                unit.extend(self.expandProgram(iterations, ins, level + 1))
            else:
                unit.append(ins)
        return unit

    def putInstructionsFast(self, iterations, instructions):
        # Same result as putInstructions, but simulates the outermost
        # repeat (iterations[0]) round by round and extrapolates once the
        # per-round cost stabilizes, instead of materializing and
        # dispatching all iterations[0] copies -- the fully-expanded
        # program is what makes putInstructions slow for large sweeps
        # (e.g. Gemv), since its cost is O(total dispatched instructions).
        # Deeper nested levels (iterations[1:], typically small -- K-chunks,
        # buffer_size) are still expanded in full within each round.
        if not iterations:
            return self.runProgram(
                [ins for ins in instructions if not isinstance(ins, list)],
                instructions,
            )
        outer_count = iterations[0]
        unit_program = self._expandOneUnit(iterations, instructions, 0)
        innermost = self.deepestInstructionList(instructions)
        return self.runRepeating(unit_program, outer_count, innermost)

    def runRepeating(self, unit_program, outer_count, innermost, max_period_multiple=8, max_prefix_windows=200):
        # Simulates `unit_program` repeated `outer_count` times per thread,
        # with the exact same dispatch semantics as runProgram (thread
        # pipeline, shared DMA queue, barriers) and NO artificial
        # synchronization between repetitions -- a fast thread starts its
        # next repetition while a slower thread is still finishing an
        # earlier one, exactly as in the fully-expanded exact simulation
        # (an earlier version of this forced all threads to finish before
        # starting the next "round", which is an artificial barrier that
        # doesn't exist in reality and measurably over-predicted latency,
        # worse at higher thread counts -- this version fixes that).
        # `unit_program` is accessed via modulo indexing instead of being
        # materialized `outer_count` times.
        #
        # Convergence is detected by watching "unit-completion" events (any
        # thread finishing one more repetition of unit_program). The
        # completion cadence's true period isn't always exactly
        # thread_count (observed empirically: e.g. 2x thread_count for one
        # ReduceOp config), so candidate periods P = k*thread_count are
        # tried for k = 1..max_period_multiple, using the smallest P whose
        # last P completions match the P before them (within a small float
        # tolerance -- service_interval values like 55.73 aren't integers,
        # so exact equality is too strict even in true steady state).
        #
        # Carries self.next_available_dma, self.last_completion_cycle and
        # the one-time phase-offset stagger across calls (not reset here
        # except the stagger, which only ever happens once), so this
        # composes correctly when a kernel calls it more than once in
        # sequence for structurally distinct segments (Gemv's batch +
        # remainder, Scan's reduce + offset-loop + scan) -- each call's
        # return value is the running total-so-far converted to ms; when
        # chaining calls, only the LAST call's return value should be used
        # (base_time must only be counted once, and each call's return
        # already includes it).
        if outer_count <= 0 or len(unit_program) == 0:
            return (self.last_completion_cycle / freq) + base_time

        unit_len = len(unit_program)
        total_len = outer_count * unit_len
        base_period = self.thread_count
        max_candidate_period = base_period * max_period_multiple
        total_completions_target = outer_count * self.thread_count
        max_prefix_completions = max_prefix_windows * max_candidate_period

        current_cycle = 1
        round_robin = 0

        if not self._phase_offset_done:
            period = sum(max(11, ins.latency) for ins in innermost) if innermost else 1
            period = max(period, 1)
            for i, thread in enumerate(self.threads):
                thread.next_available = 1 + (i * period) // self.thread_count
            self._phase_offset_done = True
        for thread in self.threads:
            thread.pc = 0

        barrier_waiting = {}
        parked = set()
        unfinished_threads = self.thread_count

        completions = 0
        # Last 2*max_candidate_period unit-completion cycles, in order.
        completion_cycles = []
        extrapolating = True

        while unfinished_threads > 0:
            current_thread = None

            for offset in range(self.thread_count):
                index = (round_robin + offset) % self.thread_count
                thread = self.threads[index]

                if thread.id in parked:
                    continue

                if thread.pc < total_len and thread.next_available <= current_cycle:
                    current_thread = thread
                    break

            if current_thread is None:
                candidates = [
                    thread.next_available
                    for thread in self.threads
                    if thread.pc < total_len and thread.id not in parked
                ]
                if not candidates:
                    break
                current_cycle = min(candidates)
                continue

            ins = unit_program[current_thread.pc % unit_len]
            issue_cycle = current_cycle

            if ins.op_type == InsType.BARRIER:
                pos = current_thread.pc
                bucket = barrier_waiting.setdefault(pos, {})
                bucket[current_thread.id] = issue_cycle
                parked.add(current_thread.id)
                current_thread.pc += 1

                if len(bucket) == self.thread_count:
                    release_cycle = max(bucket.values()) + ins.latency
                    for thread in self.threads:
                        thread.next_available = release_cycle
                        parked.discard(thread.id)
                    self.last_completion_cycle = max(self.last_completion_cycle, release_cycle)
                    del barrier_waiting[pos]

                round_robin = (current_thread.id + 1) % self.thread_count
                current_cycle += 1
                continue

            if ins.op_type in [InsType.LDMA, InsType.SDMA] and not ins.pre_contended:
                dma_start = max(issue_cycle, self.next_available_dma)
                completion_cycle = dma_start + ins.latency
                self.next_available_dma = dma_start + ins.service_interval
            else:
                completion_cycle = issue_cycle + ins.latency

            current_thread.next_available = max(issue_cycle + 11, completion_cycle)
            current_thread.pc += 1

            self.last_completion_cycle = max(self.last_completion_cycle, completion_cycle)

            if current_thread.pc == total_len:
                unfinished_threads -= 1

            if current_thread.pc % unit_len == 0:
                completions += 1
                if extrapolating:
                    completion_cycles.append(self.last_completion_cycle)
                    if len(completion_cycles) > 3 * max_candidate_period:
                        completion_cycles.pop(0)

                    converged = False
                    if completions < total_completions_target:
                        # Extrapolate ALL remaining completions in one shot
                        # rather than jumping only full periods and
                        # resuming the exact loop for a partial remainder:
                        # per-thread pc/next_available only reflect real
                        # dispatched instructions, not an extrapolated
                        # jump, so resuming the exact loop after a partial
                        # jump would read stale thread state. Treating the
                        # last (<P) completions at the same average
                        # per-completion rate is a negligible extra
                        # approximation on top of the one already being
                        # made.
                        rate = _detect_steady_rate(
                            completion_cycles,
                            completions,
                            max_period_multiple,
                            base_period=base_period,
                        )
                        if rate is not None:
                            remaining = total_completions_target - completions
                            self.last_completion_cycle += remaining * rate
                            for thread in self.threads:
                                thread.pc = total_len
                                # Extrapolation skips straight to the end
                                # without dispatching the remaining
                                # instructions, so next_available is never
                                # updated to match -- harmless within this
                                # call (nothing reads it again), but a
                                # follow-up runRepeating call for the next
                                # segment relies on it to know when each
                                # thread is free. Sync it to the
                                # extrapolated end time.
                                thread.next_available = self.last_completion_cycle
                            unfinished_threads = 0
                            extrapolating = False
                            converged = True

                    if (
                        not converged
                        and extrapolating
                        and completions >= max_prefix_completions
                    ):
                        # Safety net: never settled into a stable
                        # completion cadence within the prefix cap --
                        # simulate the rest exactly. Should be rare.
                        extrapolating = False

            round_robin = (current_thread.id + 1) % self.thread_count
            current_cycle += 1

        return (self.last_completion_cycle / freq) + base_time

    def runProgram(self, program, innermost):
        # `program` is an already-flattened instruction list (e.g. from
        # expandProgram, or several such calls concatenated -- useful when a
        # kernel's structure doesn't fit a single uniform nested-loop shape,
        # like a fixed number of full batches followed by a one-off partial
        # remainder). `innermost` is the hottest repeating instruction list,
        # used below to estimate one thread's natural loop period.
        current_cycle = 1
        last_completion_cycle = 0
        unfinished_threads = self.thread_count
        round_robin = 0

        # Every tasklet here is identical and deterministic, so without any
        # desynchronizing force they permanently lock-step onto shared
        # bottleneck instructions (e.g. all N tasklets hitting the same slow
        # MUL at the exact same simulated cycle, every iteration), producing
        # large periodic pile-ups real hardware doesn't have (real tasklets
        # drift apart from ordinary timing variance). Spreading tasklets'
        # starting phase evenly across one natural period of the innermost
        # loop breaks that artificial synchronization independently of
        # whatever DMA queueing happens to do.
        period = sum(max(11, ins.latency) for ins in innermost) if innermost else 1
        period = max(period, 1)
        for i, thread in enumerate(self.threads):
            thread.next_available = 1 + (i * period) // self.thread_count

        # Every thread runs the identical flat `program`, so a BARRIER
        # instruction sits at the same index for every thread -- that index
        # doubles as the barrier's identity. barrier_waiting[pc] collects
        # {thread_id: arrival_cycle} for threads that have reached it; a
        # thread is excluded from dispatch (via `parked`) from the moment it
        # arrives until every thread has arrived at that same pc, at which
        # point all threads are released together at
        # max(arrival cycles) + this barrier instruction's calibrated cost
        # (from BarrierExperiment; see load_lut.py/support.py InsType.BARRIER).
        barrier_waiting = {}
        parked = set()

        while unfinished_threads > 0:
            current_thread = None

            for offset in range(self.thread_count):
                index = (round_robin + offset) % self.thread_count
                thread = self.threads[index]

                if thread.id in parked:
                    continue

                if (
                    thread.pc < len(program)
                    and thread.next_available <= current_cycle
                ):
                    current_thread = thread
                    break

            if current_thread is None:
                candidates = [
                    thread.next_available
                    for thread in self.threads
                    if thread.pc < len(program) and thread.id not in parked
                ]
                if not candidates:
                    # Every remaining active thread is parked at the same
                    # barrier, waiting on each other -- shouldn't happen
                    # since the barrier releases once all have arrived, but
                    # bail rather than spin forever if it ever does.
                    break
                current_cycle = min(candidates)
                continue

            ins = program[current_thread.pc]
            issue_cycle = current_cycle

            if ins.op_type == InsType.BARRIER:
                pos = current_thread.pc
                bucket = barrier_waiting.setdefault(pos, {})
                bucket[current_thread.id] = issue_cycle
                parked.add(current_thread.id)
                current_thread.pc += 1

                if len(bucket) == self.thread_count:
                    release_cycle = max(bucket.values()) + ins.latency
                    for thread in self.threads:
                        thread.next_available = release_cycle
                        parked.discard(thread.id)
                    last_completion_cycle = max(last_completion_cycle, release_cycle)
                    del barrier_waiting[pos]

                round_robin = (current_thread.id + 1) % self.thread_count
                current_cycle += 1
                continue

            if ins.op_type in [InsType.LDMA, InsType.SDMA] and not ins.pre_contended:
                dma_start = max(issue_cycle, self.next_available_dma)
                completion_cycle = dma_start + ins.latency
                self.next_available_dma = dma_start + ins.service_interval
            else:
                completion_cycle = issue_cycle + ins.latency

            current_thread.next_available = max(
                issue_cycle + 11,
                completion_cycle,
            )
            current_thread.pc += 1

            last_completion_cycle = max(
                last_completion_cycle,
                completion_cycle,
            )

            if current_thread.pc == len(program):
                unfinished_threads -= 1

            round_robin = (current_thread.id + 1) % self.thread_count
            current_cycle += 1

        return (last_completion_cycle / freq) + base_time

    def putInstructionsNested(self, iterations, instructions):
        # Drop-in replacement for putInstructions/putInstructionsFast using
        # the multi-level nested engine below -- lets existing single-chain
        # kernels (built with the legacy (iterations, instructions)
        # convention) opt into fast-forwarding at EVERY nesting level, not
        # just the outermost, with no changes to their own code.
        if not iterations:
            return self.runProgram(
                [ins for ins in instructions if not isinstance(ins, list)],
                instructions,
            )
        segment = _segment_from_legacy(iterations, instructions, 0)
        return self.runSegment(segment)

    def runSegment(self, segment, window=None, tol=0.02):
        # Entry point for kernels built directly with Leaf/Repeat/Seq (e.g.
        # Gemv's "buffer_size-1 uniform rows + 1 special flush row" pattern,
        # which the legacy (iterations, instructions) convention can't
        # express at all, since it assumes every repeat of a level is
        # identical).
        ops, loop_meta = compileSegment(segment)
        innermost = _deepest_leaf_instructions(segment)[1]
        return self.runNested(ops, loop_meta, innermost, window, tol)

    def runNested(self, ops, loop_meta, innermost, window=None, tol=0.02):
        # Dispatch engine for bytecode compiled by compileSegment. Same
        # per-thread round-robin scheduling, shared DMA queue, and BARRIER
        # semantics as runProgram/runRepeating -- the difference is
        # thread.pc indexes a small, loop-aware bytecode array instead of a
        # fully-unrolled instruction list, and loop_start/loop_end are
        # zero-cost bookkeeping ops (resolved immediately, not treated as a
        # dispatch turn) that can trigger per-(thread, loop) extrapolation.
        #
        # Carries self.next_available_dma, self.last_completion_cycle and
        # the one-time phase-offset stagger across calls, same convention
        # as runRepeating -- only the LAST call's return value should be
        # used when chaining multiple runSegment/runNested calls for
        # structurally distinct segments.
        n = self.thread_count
        # DMA-extrapolation uses two different strategies depending on
        # thread_count (see the has_dma branches below and loop_meta's
        # dma_service_per_repeat comment):
        #   - thread_count == 1: per-thread empirical moving-average
        #     detection of next_available_dma's advancement, identical in
        #     spirit to completion-time tracking. Validated correct here,
        #     since there's no other thread to confound the measurement.
        #   - thread_count > 1: a per-thread empirical measurement is
        #     fundamentally confounded once other threads share the queue
        #     -- between one thread's own consecutive repeats, however
        #     many OTHER threads' DMA requests happen to get interleaved
        #     (via round-robin dispatch) also land in that thread's
        #     "delta", and that interleaving count isn't stable over the
        #     run. Using the ANALYTICAL dma_service_per_repeat value
        #     instead sidesteps this: in a saturated queue (the expected
        #     case once more than one thread shares it), next_available_dma
        #     advances by exactly service_interval per request,
        #     deterministically -- nothing to detect.
        if window is None:
            # How many samples a moving average needs before it can be
            # trusted scales with contention, not just a fixed constant:
            # thread_count=2/4/8 converge correctly even at window=24, but
            # thread_count=16 needs ~160-192 -- with more threads sharing
            # the DMA queue, it takes longer for system-wide contention to
            # actually ramp up to its true steady level, and a small window
            # can "converge" on an early, not-yet-representative average,
            # then get locked in and reused for the rest of the run,
            # systematically under-predicting. A "start small, require several
            # consecutive doublings to agree" alternative was tried (to
            # avoid making every loop pay for a large window regardless of
            # whether its own contention pattern needs it) and reverted:
            # for a genuinely hard case, it can be WORSE than plain exact
            # simulation, since a borderline quantity that occasionally
            # fails one intermediate check resets the whole streak,
            # forcing repeated expensive resummation with the streak never
            # reaching its threshold. Scaling the starting window with
            # thread_count (tuned against the worst case found so far: 12x
            # thread_count still under-predicted by 8.9%, 48x got it to
            # -0.56%) costs more for loops that didn't need the caution,
            # but degrades predictably rather than catastrophically.
            window = max(24, 48 * n)
        MAX_SCALE = 256
        # Largest sample count _detect_average_rate could ever need (its
        # search gives up once the doubling scale exceeds MAX_SCALE) --
        # caps how much history each (thread, loop) or pooled invocation
        # list retains, and doubles as the signal that a loop has
        # genuinely exhausted the search (not just "hasn't converged yet,
        # keep collecting"): once retained history hits this cap and
        # detection still returns None, no amount of additional data would
        # change that, so give up and fall back to exact dispatch for that
        # loop -- the same safety-net principle runRepeating's
        # max_prefix_completions already uses.
        HISTORY_CAP = 2 * window * MAX_SCALE
        # loop_iter/invocation_start are inherently PER-THREAD: each thread
        # has its own position within its own current invocation of a loop,
        # and its own start-of-invocation timestamp (needed to isolate that
        # invocation's own duration from whatever runs before/after it).
        loop_iter = [dict() for _ in range(n)]
        invocation_start = [dict() for _ in range(n)]

        # within_deltas/within_last_value/within_rate are PER-THREAD: each
        # thread tracks its OWN repeat-to-repeat delta sequence for each
        # extrapolatable loop, accumulated across every invocation of that
        # loop this thread ever makes (not reset per invocation -- more
        # invocations means more data for the moving average to settle on).
        # within_last_value is the snapshot the NEXT delta is measured
        # from; it IS reset at loop_start (one invocation's repeats must
        # not be diffed against a value from a totally different
        # invocation, or a prior thread's own progress).
        #
        # An earlier design aggregated repeat-completion events from every
        # thread into one POPULATION-WIDE history and searched for an
        # exact repeating period (up to thread_count) in it. That worked
        # for cleanly periodic cases but broke down whenever a loop's
        # timing didn't settle into a small exact period at all -- e.g. a
        # loop nested inside a non-extrapolated DMA loop, where threads
        # drift in and out of phase with each other depending on where
        # each currently sits in ITS OWN DMA wait, producing per-repeat
        # costs that swing 20-30% with no clean period to find. Tracking
        # each thread's own sequence and asking only "has the MOVING
        # AVERAGE stabilized" (see _detect_average_rate) tolerates that
        # noise directly, without needing an exact period to exist.
        within_deltas = [dict() for _ in range(n)]
        within_last_value = [dict() for _ in range(n)]
        within_rate = [dict() for _ in range(n)]
        # within_dma_* mirror within_* exactly, but track how much
        # self.next_available_dma (a single queue shared by every thread)
        # advances per repeat, from THIS thread's own point of view,
        # instead of how much this thread's own completion time advances.
        # A DMA-carrying loop is only extrapolated once BOTH rates have
        # independently converged (see the loop_end handling below):
        # skipping a thread past DMA dispatches without also advancing the
        # shared queue by a matching amount would leave it stale for
        # whichever thread reads it next.
        within_dma_deltas = [dict() for _ in range(n)]
        within_dma_last_value = [dict() for _ in range(n)]
        within_dma_rate = [dict() for _ in range(n)]
        # If within-convergence hasn't fired after several full attempts,
        # it isn't going to: giving up after a bounded number of attempts
        # (per thread, per loop) falls back to plain exact dispatch for
        # that loop, the same safety-net principle runRepeating's
        # max_prefix_completions already uses -- without this, a loop
        # whose average never actually stabilizes would keep paying the
        # (cheap, but nonzero) convergence-check cost on every repeat for
        # the entire rest of the run.
        within_gave_up = [set() for _ in range(n)]
        # invocation_history/invocation_rate stay POPULATION-WIDE (keyed
        # only by loop_id): each entry is one invocation's own RAW DURATION
        # (loop_start to loop_end for whichever thread completed it,
        # already isolated from surrounding code), and since every thread
        # runs the identical kernel structure, pooling across threads just
        # means more data for the same moving-average check to work with,
        # not a different quantity. invocation_dma_* is the same idea
        # applied to next_available_dma's advancement over one invocation.
        invocation_start_dma = [dict() for _ in range(n)]
        invocation_history = {}
        invocation_rate = {}
        invocation_dma_history = {}
        invocation_dma_rate = {}

        for thread in self.threads:
            thread.pc = 0

        if not self._phase_offset_done:
            period = sum(max(11, ins.latency) for ins in innermost) if innermost else 1
            period = max(period, 1)
            for i, thread in enumerate(self.threads):
                thread.next_available = 1 + (i * period) // self.thread_count
            self._phase_offset_done = True

        def resolve(thread):
            tid = thread.id
            while True:
                op = ops[thread.pc]
                kind = op[0]
                if kind == "loop_start":
                    loop_id = op[1]
                    meta = loop_meta[loop_id]
                    rate = invocation_rate.get(loop_id)
                    has_dma = meta["has_dma"]
                    if has_dma and n > 1:
                        # Analytical: no detection needed, see loop_meta's
                        # dma_service_per_repeat comment.
                        dma_rate = meta["count"] * meta["dma_service_per_repeat"]
                    elif has_dma:
                        dma_rate = invocation_dma_rate.get(loop_id)
                    else:
                        dma_rate = None
                    dma_ready = dma_rate is not None if has_dma else True
                    if meta["extrapolatable"] and rate is not None and dma_ready:
                        # `rate` is already the cost of ONE full invocation
                        # (all of this loop's repeats combined) -- add it
                        # once, not multiplied by count again. Same for
                        # dma_rate against next_available_dma, when this
                        # loop carries DMA traffic.
                        thread.next_available += rate
                        if has_dma and n == 1:
                            self.next_available_dma += dma_rate
                        self.last_completion_cycle = max(
                            self.last_completion_cycle, thread.next_available
                        )
                        thread.pc = meta["end_pc"] + 1
                        continue
                    loop_iter[tid][loop_id] = 0
                    invocation_start[tid][loop_id] = thread.next_available
                    within_last_value[tid][loop_id] = thread.next_available
                    if has_dma and n == 1:
                        invocation_start_dma[tid][loop_id] = self.next_available_dma
                        within_dma_last_value[tid][loop_id] = self.next_available_dma
                    thread.pc += 1
                elif kind == "loop_end":
                    loop_id, body_start = op[1], op[2]
                    meta = loop_meta[loop_id]
                    count = meta["count"]
                    it = loop_iter[tid][loop_id] + 1
                    loop_iter[tid][loop_id] = it

                    if meta["extrapolatable"] and loop_id not in within_gave_up[tid]:
                        has_dma = meta["has_dma"]
                        analytical_dma = has_dma and n > 1
                        rate = within_rate[tid].get(loop_id)
                        if analytical_dma:
                            dma_rate = meta["dma_service_per_repeat"]
                        elif has_dma:
                            dma_rate = within_dma_rate[tid].get(loop_id)
                        else:
                            dma_rate = None

                        need_rate = rate is None
                        need_dma = has_dma and not analytical_dma and dma_rate is None
                        if need_rate or need_dma:
                            deltas = within_deltas[tid].setdefault(loop_id, [])
                            deltas.append(thread.next_available - within_last_value[tid][loop_id])
                            if len(deltas) > HISTORY_CAP:
                                del deltas[0]
                            if need_rate:
                                rate = _detect_average_rate(deltas, window, tol, MAX_SCALE)
                                if rate is not None:
                                    within_rate[tid][loop_id] = rate

                            if need_dma:
                                dma_deltas = within_dma_deltas[tid].setdefault(loop_id, [])
                                dma_deltas.append(
                                    self.next_available_dma - within_dma_last_value[tid][loop_id]
                                )
                                if len(dma_deltas) > HISTORY_CAP:
                                    del dma_deltas[0]
                                dma_rate = _detect_average_rate(dma_deltas, window, tol, MAX_SCALE)
                                if dma_rate is not None:
                                    within_dma_rate[tid][loop_id] = dma_rate

                            ready = rate is not None and (not has_dma or dma_rate is not None)
                            if not ready and len(deltas) >= HISTORY_CAP:
                                within_gave_up[tid].add(loop_id)
                                within_deltas[tid].pop(loop_id, None)
                                within_dma_deltas[tid].pop(loop_id, None)
                            if not ready:
                                rate = None
                        within_last_value[tid][loop_id] = thread.next_available
                        if has_dma and not analytical_dma:
                            within_dma_last_value[tid][loop_id] = self.next_available_dma
                        if rate is not None and (not has_dma or dma_rate is not None):
                            remaining = count - it
                            thread.next_available += remaining * rate
                            if has_dma and n == 1:
                                self.next_available_dma += remaining * dma_rate
                            self.last_completion_cycle = max(
                                self.last_completion_cycle, thread.next_available
                            )
                            it = count
                            loop_iter[tid][loop_id] = count

                    if it >= count:
                        duration = thread.next_available - invocation_start[tid][loop_id]
                        if meta["extrapolatable"] and loop_id not in invocation_rate:
                            inv_hist = invocation_history.setdefault(loop_id, [])
                            inv_hist.append(duration)
                            if len(inv_hist) > HISTORY_CAP:
                                del inv_hist[0]
                            rate2 = _detect_average_rate(inv_hist, window, tol, MAX_SCALE)
                            if rate2 is not None:
                                invocation_rate[loop_id] = rate2
                        if meta["has_dma"] and n == 1 and loop_id not in invocation_dma_rate:
                            dma_duration = (
                                self.next_available_dma - invocation_start_dma[tid][loop_id]
                            )
                            inv_dma_hist = invocation_dma_history.setdefault(loop_id, [])
                            inv_dma_hist.append(dma_duration)
                            if len(inv_dma_hist) > HISTORY_CAP:
                                del inv_dma_hist[0]
                            dma_rate2 = _detect_average_rate(inv_dma_hist, window, tol, MAX_SCALE)
                            if dma_rate2 is not None:
                                invocation_dma_rate[loop_id] = dma_rate2
                        thread.pc = meta["end_pc"] + 1
                    else:
                        thread.pc = body_start
                else:
                    return

        for thread in self.threads:
            resolve(thread)

        current_cycle = 1
        round_robin = 0
        barrier_waiting = {}
        parked = set()
        unfinished_threads = self.thread_count

        while unfinished_threads > 0:
            current_thread = None

            for offset in range(self.thread_count):
                index = (round_robin + offset) % self.thread_count
                thread = self.threads[index]

                if thread.id in parked:
                    continue

                if ops[thread.pc][0] != "halt" and thread.next_available <= current_cycle:
                    current_thread = thread
                    break

            if current_thread is None:
                candidates = [
                    thread.next_available
                    for thread in self.threads
                    if ops[thread.pc][0] != "halt" and thread.id not in parked
                ]
                if not candidates:
                    break
                current_cycle = min(candidates)
                continue

            ins = ops[current_thread.pc][1]
            issue_cycle = current_cycle

            if ins.op_type == InsType.BARRIER:
                pos = current_thread.pc
                bucket = barrier_waiting.setdefault(pos, {})
                bucket[current_thread.id] = issue_cycle
                parked.add(current_thread.id)
                # Advance past the barrier op itself, but do NOT resolve()
                # yet: this thread is parked until every thread arrives, so
                # its next_available doesn't reflect the barrier's release
                # cycle yet. Resolving now (if the very next op happens to
                # be a loop_start/loop_end) would let a zero-cost
                # loop-completion/extrapolation check fire using that
                # stale, pre-release timestamp -- resolve every thread only
                # once release_cycle is known and applied below.
                current_thread.pc += 1

                if len(bucket) == self.thread_count:
                    release_cycle = max(bucket.values()) + ins.latency
                    for thread in self.threads:
                        thread.next_available = release_cycle
                        parked.discard(thread.id)
                    self.last_completion_cycle = max(self.last_completion_cycle, release_cycle)
                    del barrier_waiting[pos]
                    for thread in self.threads:
                        resolve(thread)

                round_robin = (current_thread.id + 1) % self.thread_count
                current_cycle += 1
                continue

            if ins.op_type in [InsType.LDMA, InsType.SDMA] and not ins.pre_contended:
                dma_start = max(issue_cycle, self.next_available_dma)
                completion_cycle = dma_start + ins.latency
                self.next_available_dma = dma_start + ins.service_interval
            else:
                completion_cycle = issue_cycle + ins.latency

            current_thread.next_available = max(issue_cycle + 11, completion_cycle)
            current_thread.pc += 1
            resolve(current_thread)

            self.last_completion_cycle = max(self.last_completion_cycle, completion_cycle)

            if ops[current_thread.pc][0] == "halt":
                unfinished_threads -= 1

            round_robin = (current_thread.id + 1) % self.thread_count
            current_cycle += 1

        return (self.last_completion_cycle / freq) + base_time
