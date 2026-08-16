#include "ProgramBuilderImpl.h"
#include "upmem_cost_model/Simulation.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <vector>

namespace upmem_cm {
namespace {

// ─────────────────────────────────────────────────────────────────────────────
// Bytecode
// ─────────────────────────────────────────────────────────────────────────────
//
// The flat instruction array plus its LoopInfo records, re-expressed as the
// op array the reference model's runNested walks (predictor.py, via
// compileSegment): real instructions interleaved with zero-cost LoopStart /
// LoopEnd markers. A loop then costs O(1) ops however many times it repeats,
// which is what lets a converged loop be skipped whole -- the flat array can
// only be walked one repeat at a time.

enum class OpKind : uint8_t { Ins, LoopStart, LoopEnd, Halt };

struct Op {
  OpKind kind;
  uint32_t a = 0; // Ins: instruction index. LoopStart/LoopEnd: loop id.
  uint32_t b = 0; // LoopEnd: pc of the first op of the body.
};

struct LoopMeta {
  uint32_t count = 0;
  /// A loop with a barrier anywhere inside is never extrapolated: skipping a
  /// thread past a barrier it has not physically reached would desynchronise
  /// it from the threads still waiting there.
  bool extrapolatable = true;
  bool has_dma = false;
  /// Sum of the service intervals of the DMA requests in one repeat -- what
  /// the shared queue provably advances by per repeat once it is saturated,
  /// which is the expected case as soon as more than one thread shares it.
  double dma_service_per_repeat = 0;
  uint32_t end_pc = 0; // pc of this loop's LoopEnd op
};

struct Bytecode {
  std::vector<Op> ops;
  std::vector<LoopMeta> loops;
  /// Instruction index → the pc of its Ins op, for translating ifthread
  /// guard ranges (which are recorded in instruction space) into op space.
  std::vector<uint32_t> op_of_ins;
};

Bytecode compileProgram(const std::vector<SimInsn> &insns,
                        const std::vector<LoopInfo> &loops) {
  std::unordered_map<uint32_t, const LoopInfo *> heads;
  for (const auto &li : loops)
    heads[li.head_idx] = &li;

  Bytecode bc;
  bc.op_of_ins.assign(insns.size(), 0);

  // Emits [lo, hi). `ignore_head` suppresses the loop-head lookup at exactly
  // one index -- used when recursing into a loop body, whose first
  // instruction is that loop's own head and would otherwise re-enter it.
  auto emit = [&](auto &&self, uint32_t lo, uint32_t hi,
                  std::optional<uint32_t> ignore_head) -> void {
    for (uint32_t pc = lo; pc < hi;) {
      auto it =
          (ignore_head && *ignore_head == pc) ? heads.end() : heads.find(pc);
      if (it == heads.end()) {
        bc.op_of_ins[pc] = static_cast<uint32_t>(bc.ops.size());
        bc.ops.push_back({OpKind::Ins, pc, 0});
        pc++;
        continue;
      }

      const LoopInfo &li = *it->second;
      uint32_t loop_id = static_cast<uint32_t>(bc.loops.size());
      bc.loops.push_back({});
      bc.ops.push_back({OpKind::LoopStart, loop_id, 0});
      uint32_t body_start = static_cast<uint32_t>(bc.ops.size());

      self(self, li.head_idx, li.jump_idx + 1, li.head_idx);

      LoopMeta &meta = bc.loops[loop_id];
      meta.count = li.trips;
      meta.end_pc = static_cast<uint32_t>(bc.ops.size());
      // Walk the body's ops rather than the instruction range: a nested
      // loop's barrier or DMA counts for the loop that encloses it, and the
      // nested loop's own repeats multiply its DMA service.
      for (uint32_t p = body_start; p < meta.end_pc; p++) {
        const Op &op = bc.ops[p];
        if (op.kind == OpKind::Ins) {
          const SimInsn &in = insns[op.a];
          if (in.kind == SimKind::BARRIER)
            meta.extrapolatable = false;
          else if (in.kind == SimKind::DMA_LOAD ||
                   in.kind == SimKind::DMA_STORE) {
            meta.has_dma = true;
            meta.dma_service_per_repeat += lookupDmaServiceInterval(
                in.kind == SimKind::DMA_LOAD, in.dma_bytes);
          }
        } else if (op.kind == OpKind::LoopEnd) {
          const LoopMeta &inner = bc.loops[op.a];
          if (!inner.extrapolatable)
            meta.extrapolatable = false;
          if (inner.has_dma) {
            meta.has_dma = true;
            meta.dma_service_per_repeat +=
                inner.count * inner.dma_service_per_repeat;
          }
        }
      }
      bc.ops.push_back({OpKind::LoopEnd, loop_id, body_start});
      pc = li.jump_idx + 1;
    }
  };

  emit(emit, 0, static_cast<uint32_t>(insns.size()), std::nullopt);
  bc.ops.push_back({OpKind::Halt, 0, 0});
  return bc;
}

// ─────────────────────────────────────────────────────────────────────────────
// Convergence
// ─────────────────────────────────────────────────────────────────────────────

/// The reference model's _detect_average_rate: has the moving average of
/// `samples` stabilised? Doubling the lookback from `base_window`, it asks at
/// each scale whether avg(last w) still equals avg(last 2w) -- if the process
/// has reached steady state, widening cannot move the average, and if it is
/// still drifting the older half pulls it away. Returns the average, or
/// nothing if no scale passes.
///
/// This tolerates a noisy-but-stationary rate, which an exact-period search
/// cannot: threads sharing a DMA queue drift in and out of phase and their
/// per-repeat costs swing by tens of percent without ever settling into a
/// clean period, while their mean is perfectly stable.
std::optional<double> detectAverageRate(const std::vector<double> &samples,
                                        size_t base_window, double tol,
                                        size_t max_scale) {
  for (size_t scale = 1; scale <= max_scale; scale *= 2) {
    size_t w = base_window * scale;
    if (2 * w > samples.size())
      return std::nullopt;
    double small = 0, large = 0;
    for (size_t i = samples.size() - w; i < samples.size(); i++)
      small += samples[i];
    for (size_t i = samples.size() - 2 * w; i < samples.size(); i++)
      large += samples[i];
    small /= static_cast<double>(w);
    large /= static_cast<double>(2 * w);
    if (std::abs(small - large) <=
        tol * std::max({1.0, std::abs(small), std::abs(large)}))
      return small;
  }
  return std::nullopt;
}

/// A per-(thread, loop) or pooled sequence of durations, and the rate it has
/// converged to.
struct RateTracker {
  std::vector<double> samples;
  double last_value = 0;
  std::optional<double> rate;
  bool gave_up = false;
};

// ─────────────────────────────────────────────────────────────────────────────
// Thread
// ─────────────────────────────────────────────────────────────────────────────
//
// The scheduling is the reference model's, instruction for instruction, so
// that a program priced by both engines differs only in their tables and not
// in how either one issues it. What that model does, and what this one
// therefore does now:
//
//   * A thread is a cursor and a `next_available` cycle. Nothing else
//     constrains issue -- no pipeline occupancy, no in-flight slot count.
//     Issuing an instruction at cycle c makes its thread unavailable until
//     max(c + 11, completion): 11 cycles is the pipeline depth a thread must
//     wait before its next issue whatever the instruction was, and a long
//     instruction extends that to its own completion.
//   * DMA requests queue against ONE shared queue for the whole DPU, loads
//     and stores together. A request starts when the queue frees, completes
//     `latency` cycles later, and holds the queue for `service_interval`,
//     which is shorter -- so a saturated queue issues one request per
//     service interval while each individual one still takes its full
//     latency.
//   * A barrier parks its thread and is identified by its op index. When
//     every thread has arrived, all of them resume together at max(arrival)
//     + the barrier's cost, charged once and not per thread.

struct SimThread {
  uint32_t tid = 0;
  uint32_t pc = 0;
  double next_available = 1;
  bool parked = false;
  /// Guard op pc → the op pc to jump to, for ifthread regions this thread is
  /// not part of.
  std::unordered_map<uint32_t, uint32_t> cond_jumps;
  // Per-loop state, indexed by loop id.
  std::vector<uint32_t> loop_iter;
  std::vector<double> invocation_start;
  std::vector<double> invocation_start_dma;
  std::vector<RateTracker> within;     // repeat-to-repeat completion deltas
  std::vector<RateTracker> within_dma; // ... and shared-queue deltas
};

/// One pass of the innermost loop body, in cycles, counting each instruction
/// as the longer of its latency and the 11-cycle issue interval. This is the
/// period the tasklets' start cycles are spread over; the reference model
/// takes it from the deepest leaf of its segment tree, and the deepest
/// nesting of the op array is the same thing.
uint64_t innermostPeriodOf(const Bytecode &bc,
                           const std::vector<SimInsn> &insns) {
  int depth = 0, deepest = 0;
  std::vector<int> op_depth(bc.ops.size(), 0);
  for (size_t pc = 0; pc < bc.ops.size(); pc++) {
    if (bc.ops[pc].kind == OpKind::LoopStart)
      depth++;
    op_depth[pc] = depth;
    if (bc.ops[pc].kind == OpKind::LoopEnd)
      depth--;
    deepest = std::max(deepest, op_depth[pc]);
  }

  uint64_t period = 0;
  for (size_t pc = 0; pc < bc.ops.size(); pc++)
    if (op_depth[pc] == deepest && bc.ops[pc].kind == OpKind::Ins)
      period += std::max<uint64_t>(11, insns[bc.ops[pc].a].latency);
  return std::max<uint64_t>(period, 1);
}

} // anonymous namespace

// ─────────────────────────────────────────────────────────────────────────────
// ProgramBuilderImpl::simulateFast
// ─────────────────────────────────────────────────────────────────────────────

double ProgramBuilderImpl::simulateFast(int nTasklets, uint64_t freqHz) {
  // Kept as a thin wrapper so the two modes cannot drift apart: fast mode is
  // the same scheduler, allowed to extrapolate a loop once its cost per
  // repeat has stopped moving. It answers the same question, only sooner.
  // The timeout is what separates them at the call site, so this one cannot
  // fail; an unconverged loop simply runs exactly.
  return simulateScheduled(nTasklets, freqHz, std::chrono::milliseconds::zero(),
                           /*extrapolate=*/true)
      .value();
}

// ─────────────────────────────────────────────────────────────────────────────
// ProgramBuilderImpl::simulateScheduled
// ─────────────────────────────────────────────────────────────────────────────

std::optional<double>
ProgramBuilderImpl::simulateScheduled(int nTasklets, uint64_t freqHz,
                                      std::chrono::milliseconds timeoutMs,
                                      bool extrapolate) {
  const int n = nTasklets;
  Bytecode bc = compileProgram(main_, main_loops_);
  const uint32_t halt_pc = static_cast<uint32_t>(bc.ops.size()) - 1;
  const size_t n_loops = bc.loops.size();

  // How many samples a moving average needs before it can be trusted scales
  // with contention: with more threads sharing the DMA queue it takes longer
  // for system-wide contention to ramp to its steady level, and a small
  // window can converge on an early, unrepresentative average and then be
  // reused for the rest of the run. These are the reference model's values.
  const size_t window = std::max<size_t>(24, 48 * static_cast<size_t>(n));
  const double kTol = 0.02;
  const size_t kMaxScale = 256;
  // Retaining more than the largest window the search could ever use buys
  // nothing, and hitting the cap without converging is the signal that no
  // amount of further data would help: give up and run that loop exactly.
  const size_t history_cap = 2 * window * kMaxScale;

  std::vector<SimThread> threads(n);
  for (int i = 0; i < n; i++) {
    SimThread &th = threads[i];
    th.tid = static_cast<uint32_t>(i);
    th.loop_iter.assign(n_loops, 0);
    th.invocation_start.assign(n_loops, 0);
    th.invocation_start_dma.assign(n_loops, 0);
    th.within.assign(n_loops, {});
    th.within_dma.assign(n_loops, {});
    for (const auto &ci : main_conds_) {
      bool allowed =
          std::count(ci.allowed_tids.begin(), ci.allowed_tids.end(), i) > 0;
      if (!allowed)
        th.cond_jumps[bc.op_of_ins[ci.guard_idx]] =
            ci.end_idx < bc.op_of_ins.size() ? bc.op_of_ins[ci.end_idx]
                                             : halt_pc;
    }
  }

  // Identical deterministic tasklets would otherwise stay in lock-step for
  // the whole kernel, all of them reaching the same slow instruction on the
  // same cycle and queueing for the same DMA at once -- a pile-up real
  // tasklets do not have, since ordinary timing variance spreads them out.
  // The reference model breaks that by spreading their start cycles evenly
  // over one period of the innermost loop, and so does this.
  const uint64_t period = innermostPeriodOf(bc, main_);
  for (int i = 0; i < n; i++) {
    // Integer division, as the reference model's `//` is: the offsets are
    // whole cycles.
    uint64_t offset =
        static_cast<uint64_t>(i) * period / static_cast<uint64_t>(n);
    threads[i].next_available = static_cast<double>(1 + offset);
  }

  double last_completion = 0;
  double next_available_dma = 0;
  // Pooled across threads, keyed by loop id: every thread runs the same
  // kernel, so one invocation's duration is the same quantity whoever
  // completed it, and pooling just gives the same check more data.
  std::vector<RateTracker> invocation(n_loops);
  std::vector<RateTracker> invocation_dma(n_loops);

  // Resolves the zero-cost loop markers at a thread's cursor, which is also
  // where a converged loop or invocation gets skipped.
  auto resolve = [&](SimThread &th) {
    while (true) {
      if (auto jump = th.cond_jumps.find(th.pc); jump != th.cond_jumps.end()) {
        th.pc = jump->second;
        continue;
      }
      const Op &op = bc.ops[th.pc];
      if (op.kind == OpKind::LoopStart) {
        const LoopMeta &meta = bc.loops[op.a];
        std::optional<double> rate =
            extrapolate ? invocation[op.a].rate : std::nullopt;
        std::optional<double> dma_rate;
        if (meta.has_dma && n > 1)
          // Analytical, not detected: see the note on the loop_end branch.
          dma_rate = meta.count * meta.dma_service_per_repeat;
        else if (meta.has_dma)
          dma_rate = extrapolate ? invocation_dma[op.a].rate : std::nullopt;
        bool dma_ready = meta.has_dma ? dma_rate.has_value() : true;

        if (meta.extrapolatable && rate && dma_ready) {
          // `rate` is the cost of one whole invocation, every repeat
          // included, so it is added once and not multiplied by the count.
          th.next_available += *rate;
          if (meta.has_dma && n == 1)
            next_available_dma += *dma_rate;
          last_completion = std::max(last_completion, th.next_available);
          th.pc = meta.end_pc + 1;
          continue;
        }
        th.loop_iter[op.a] = 0;
        th.invocation_start[op.a] = th.next_available;
        th.within[op.a].last_value = th.next_available;
        if (meta.has_dma && n == 1) {
          th.invocation_start_dma[op.a] = next_available_dma;
          th.within_dma[op.a].last_value = next_available_dma;
        }
        th.pc++;
        continue;
      }
      if (op.kind == OpKind::LoopEnd) {
        const LoopMeta &meta = bc.loops[op.a];
        RateTracker &within = th.within[op.a];
        RateTracker &within_dma = th.within_dma[op.a];
        uint32_t it = ++th.loop_iter[op.a];

        if (extrapolate && meta.extrapolatable && !within.gave_up) {
          // With one thread the shared queue's advance per repeat can be
          // measured like anything else. With more, it cannot: between one
          // thread's consecutive repeats, however many other threads'
          // requests happen to interleave lands in its delta too, and that
          // count is not stationary -- threads start skipping their own
          // dispatches at different times. The saturated queue advances by
          // exactly one service interval per request, so for n > 1 that sum
          // is used directly instead of being detected.
          bool analytical_dma = meta.has_dma && n > 1;
          std::optional<double> rate = within.rate;
          std::optional<double> dma_rate;
          if (analytical_dma)
            dma_rate = meta.dma_service_per_repeat;
          else if (meta.has_dma)
            dma_rate = within_dma.rate;

          bool need_rate = !rate.has_value();
          bool need_dma = meta.has_dma && !analytical_dma && !dma_rate;
          if (need_rate || need_dma) {
            within.samples.push_back(th.next_available - within.last_value);
            if (within.samples.size() > history_cap)
              within.samples.erase(within.samples.begin());
            if (need_rate) {
              rate = detectAverageRate(within.samples, window, kTol, kMaxScale);
              within.rate = rate;
            }
            if (need_dma) {
              within_dma.samples.push_back(next_available_dma -
                                           within_dma.last_value);
              if (within_dma.samples.size() > history_cap)
                within_dma.samples.erase(within_dma.samples.begin());
              dma_rate = detectAverageRate(within_dma.samples, window, kTol,
                                           kMaxScale);
              within_dma.rate = dma_rate;
            }
            bool ready = rate && (!meta.has_dma || dma_rate);
            if (!ready && within.samples.size() >= history_cap) {
              within.gave_up = true;
              within.samples.clear();
              within_dma.samples.clear();
            }
            if (!ready)
              rate.reset();
          }

          within.last_value = th.next_available;
          if (meta.has_dma && !analytical_dma)
            within_dma.last_value = next_available_dma;

          if (rate && (!meta.has_dma || dma_rate)) {
            uint32_t remaining = meta.count - it;
            th.next_available += remaining * *rate;
            if (meta.has_dma && n == 1)
              next_available_dma += remaining * *dma_rate;
            last_completion = std::max(last_completion, th.next_available);
            it = meta.count;
            th.loop_iter[op.a] = meta.count;
          }
        } else {
          within.last_value = th.next_available;
        }

        if (it >= meta.count) {
          double duration = th.next_available - th.invocation_start[op.a];
          if (extrapolate && meta.extrapolatable && !invocation[op.a].rate) {
            RateTracker &inv = invocation[op.a];
            inv.samples.push_back(duration);
            if (inv.samples.size() > history_cap)
              inv.samples.erase(inv.samples.begin());
            inv.rate = detectAverageRate(inv.samples, window, kTol, kMaxScale);
          }
          if (extrapolate && meta.has_dma && n == 1 &&
              !invocation_dma[op.a].rate) {
            RateTracker &inv = invocation_dma[op.a];
            inv.samples.push_back(next_available_dma -
                                  th.invocation_start_dma[op.a]);
            if (inv.samples.size() > history_cap)
              inv.samples.erase(inv.samples.begin());
            inv.rate = detectAverageRate(inv.samples, window, kTol, kMaxScale);
          }
          th.pc = meta.end_pc + 1;
        } else {
          th.pc = op.b;
        }
        continue;
      }
      return; // Ins or Halt: a real dispatch, or nothing left to do
    }
  };

  for (SimThread &th : threads)
    resolve(th);

  double current_cycle = 1;
  size_t round_robin = 0;
  size_t unfinished = threads.size();
  std::unordered_map<uint32_t, std::unordered_map<uint32_t, double>>
      barrier_waiting;

  auto deadline = (timeoutMs != timeoutMs.zero())
                      ? std::chrono::steady_clock::now() + timeoutMs
                      : std::chrono::steady_clock::time_point::max();
  uint64_t dispatches = 0;

  while (unfinished > 0) {
    SimThread *current = nullptr;
    for (size_t offset = 0; offset < threads.size(); offset++) {
      SimThread &th = threads[(round_robin + offset) % threads.size()];
      if (th.parked || bc.ops[th.pc].kind == OpKind::Halt)
        continue;
      if (th.next_available <= current_cycle) {
        current = &th;
        break;
      }
    }

    if (!current) {
      // Nobody can issue at this cycle: jump to the earliest one who can,
      // rather than stepping cycle by cycle.
      double soonest = std::numeric_limits<double>::infinity();
      for (const SimThread &th : threads)
        if (!th.parked && bc.ops[th.pc].kind != OpKind::Halt)
          soonest = std::min(soonest, th.next_available);
      if (!std::isfinite(soonest))
        break; // every unfinished thread is parked: a barrier nobody reaches
      current_cycle = soonest;
      continue;
    }

    if (++dispatches % 4096 == 0 &&
        std::chrono::steady_clock::now() >= deadline)
      return std::nullopt;

    const SimInsn &insn = main_[bc.ops[current->pc].a];
    double issue_cycle = current_cycle;

    if (insn.kind == SimKind::BARRIER) {
      uint32_t pos = current->pc;
      barrier_waiting[pos][current->tid] = issue_cycle;
      current->parked = true;
      // Past the barrier op, but not resolved yet: this thread's
      // next_available does not reflect the release cycle until every
      // thread has arrived, and resolving now would let a loop marker
      // sitting right after the barrier extrapolate off that stale value.
      current->pc++;

      if (barrier_waiting[pos].size() == threads.size()) {
        double arrival = 0;
        for (const auto &[_, cycle] : barrier_waiting[pos])
          arrival = std::max(arrival, cycle);
        // Not insn.latency: a barrier's cost depends on how many tasklets
        // are in it, which the builder does not know, so createBarrier
        // leaves the instruction at zero and it is priced here. The
        // reference model resolves it the same way, at the point where its
        // reader knows the tasklet count.
        double release = arrival + lookupBarrierLatency(n);
        for (SimThread &th : threads) {
          th.next_available = release;
          th.parked = false;
        }
        last_completion = std::max(last_completion, release);
        barrier_waiting.erase(pos);
        for (SimThread &th : threads)
          resolve(th);
      }

      round_robin = (current->tid + 1) % threads.size();
      current_cycle++;
      continue;
    }

    double completion;
    if (insn.kind == SimKind::DMA_LOAD || insn.kind == SimKind::DMA_STORE) {
      bool is_load = insn.kind == SimKind::DMA_LOAD;
      double start = std::max(issue_cycle, next_available_dma);
      completion = start + lookupDmaLatency(is_load, insn.dma_bytes);
      next_available_dma =
          start + lookupDmaServiceInterval(is_load, insn.dma_bytes);
    } else {
      completion = issue_cycle + insn.latency;
    }

    current->next_available = std::max(issue_cycle + 11, completion);
    current->pc++;
    resolve(*current);
    last_completion = std::max(last_completion, completion);
    if (bc.ops[current->pc].kind == OpKind::Halt)
      unfinished--;

    round_robin = (current->tid + 1) % threads.size();
    current_cycle++;
  }

  // The reference model adds a constant base_time to this before returning,
  // for the host-side cost of getting a kernel started. Not added here: the
  // caller charges its own launch overhead, measured per working-group size
  // (see UpmemPythonSimulator.cpp), and would otherwise count both.
  return last_completion / static_cast<double>(freqHz);
}

// ─────────────────────────────────────────────────────────────────────────────
// ProgramBuilderImpl::simulate
// ─────────────────────────────────────────────────────────────────────────────

std::optional<double>
ProgramBuilderImpl::simulate(int nTasklets, uint64_t freqHz,
                             std::chrono::milliseconds timeoutMs, bool fast) {
  if (fast)
    return simulateFast(nTasklets, freqHz);
  return simulateScheduled(nTasklets, freqHz, timeoutMs,
                           /*extrapolate=*/false);
}

} // namespace upmem_cm
