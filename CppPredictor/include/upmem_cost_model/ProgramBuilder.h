#pragma once
#include "upmem_cost_model/Types.h"
#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace upmem_cm {

class ProgramBuilderImpl;

/// High-level builder for UPMEM DPU programs that produces a flat low-level IR
/// suitable for cycle-accurate simulation.
///
/// Callers describe a DPU kernel using high-level primitives (buffers,
/// transfers, loads, stores, loops) without worrying about the underlying
/// instruction representation. Internally, each call lowers directly to a
/// sequence of SimInsn entries — the LL IR — including address-generation
/// instructions, DMA entries, and loop prologue/epilogue bookkeeping. The LL IR
/// is then fed to a round-robin tasklet scheduler over a shared DMA queue,
/// which is the Python reference model's scheduler ported (see
/// Simulator.cpp).
///
/// Typical usage:
///   ProgramBuilder b;
///   auto A = b.addBuffer("A", MemSpace::MRAM, DType::F32);
///   auto a = b.addBuffer("a_wram", MemSpace::WRAM, DType::F32);
///   b.createTransfer(A, a, 64);          // MRAM → WRAM DMA
///   b.beginLoop(0, 64);
///     auto v = b.createLoad(a, /*iv_indexed=*/true);
///     b.createStore(a, v, /*iv_indexed=*/true);
///   b.endLoop();
///   double seconds = b.simulate(16);
///
/// The builder is not thread-safe; create one instance per thread.
class ProgramBuilder {
public:
  /// Opaque handle to a declared buffer.
  using BufId = uint32_t;
  /// Opaque handle to a computed value (result of load, arith, or const).
  using ValId = uint32_t;

  ProgramBuilder();
  ~ProgramBuilder();
  ProgramBuilder(const ProgramBuilder &) = delete;
  ProgramBuilder &operator=(const ProgramBuilder &) = delete;

  /// Declare a buffer in the given memory space with element type `dtype`.
  /// Returns a handle used by subsequent load/store/transfer calls.
  BufId addBuffer(std::string_view name, MemSpace space, DType dtype);

  /// Emit a DMA transfer of `n_elems` elements between `src` and `dst`.
  /// Exactly one of the two buffers must be in MRAM; the other is in WRAM.
  /// `src_iv_indexed` / `dst_iv_indexed`: set true when the MRAM-side address
  /// advances by `n_elems` each iteration of the enclosing loop. The builder
  /// then emits address-bump instructions in the loop epilogue automatically.
  void createTransfer(BufId src, BufId dst, int64_t n_elems,
                      bool src_iv_indexed = false, bool dst_iv_indexed = false);

  /// Emit a WRAM load from `buf`. `iv_indexed` indicates the address steps
  /// by one element per iteration of the innermost enclosing loop (triggers
  /// offset-register allocation and an epilogue bump).
  /// Returns a ValId representing the loaded value.
  ValId createLoad(BufId buf, bool iv_indexed = false);

  /// Emit a WRAM store of `val` to `buf`. `iv_indexed` works as in createLoad.
  void createStore(BufId buf, ValId val, bool iv_indexed = false);

  /// Emit a load-modify-store reduction into `buf` using binary operator `op`.
  /// Lowers to: load(buf) → op(loaded, val) → store(buf).
  /// Used to model accumulator patterns such as `acc += compute`.
  void createReduceStore(BufId buf, ArithOp op, ValId val);

  void createReductionLoop(int bufSize, BufId in, BufId out, ArithOp op) {
    beginLoop(0, bufSize);
    auto a_val = createLoad(in, /*iv_indexed=*/true);
    createReduceStore(out, op, a_val);
    endLoop();
  }

  /// Emit an arithmetic operation on two values. For MUL on 32-bit integer
  /// types with a power-of-2 constant operand, the faster immediate-multiply
  /// latency is used automatically.
  /// Returns a ValId for the result.
  ValId createArith(ArithOp op, DType dtype, ValId lhs, ValId rhs);

  /// Emit a MOV-immediate to materialise a constant. Returns the ValId.
  /// The constant value is tracked so power-of-2 MUL optimisation can fire.
  ValId createConst(int64_t value, DType dtype);

  /// Begin a counted loop with bounds [lb, ub) and the given step.
  /// All builder calls between beginLoop() and the matching endLoop() are
  /// recorded as the loop body. Loops may be nested.
  /// Emit a barrier: all tasklets must reach this point before any proceeds,
  /// and all of them resume together one barrier cost after the last arrival
  /// (see lookupBarrierLatency, which grows as a power of the tasklet count).
  void createBarrier();

  /// Begin a conditional block executed only by tasklets whose ID is in
  /// `allowedTids`. Other tasklets skip the block entirely (zero cost).
  /// Allowed tasklets pay an 11-cycle branch check on entry.
  void beginIfThread(std::vector<int> allowedTids);
  void endIfThread();

  void beginLoop(int64_t lb, int64_t ub, int64_t step = 1);

  /// Close the innermost open loop. Synthesises the full loop sequence:
  ///   prologue  — offset-register initialisations for iv-indexed accesses
  ///   body      — instructions recorded since beginLoop()
  ///   epilogue  — offset bumps, IV increment, back-edge JUMP
  /// and flushes the result into the enclosing scope (or the top-level
  /// program).
  void endLoop();

  /// Run the simulator on the built program and return the estimated
  /// wall-clock time in seconds.
  /// `nTasklets` is the number of concurrent tasklets (hardware threads) on
  /// one DPU. `freqHz` is the DPU clock frequency (default: 350 MHz).
  ///
  /// `fast` lets a loop whose cost per repeat has stopped moving be skipped
  /// at that rate instead of dispatched repeat by repeat. It is the same
  /// scheduler either way, so it answers the same question: on the
  /// benchmarked programs it lands within 0.01% of the exhaustive run, and
  /// on programs whose loops never repeat enough for a rate to be
  /// established it simply does the exhaustive run. It cannot time out.
  std::optional<double>
  simulate(int nTasklets,
           std::chrono::milliseconds timeoutMs = std::chrono::milliseconds(0),
           bool fast = false, uint64_t freqHz = 350'000'000);

  /// Serialise the built program as JSON for the Python reference model's
  /// `cnmprog` reader, to cross-check this simulator against it.
  ///
  /// The dump is *symbolic* — opcodes, dtypes, DMA byte counts and loop trip
  /// counts, with no latencies — so the reader prices it from its own
  /// calibrated tables and the comparison isolates scheduling differences from
  /// latency-table differences.
  ///
  /// Programs built with beginIfThread() serialise to an `ifthread` node the
  /// reference model has no equivalent for; its reader rejects them rather
  /// than silently mispricing a predicated region.
  std::string emitJson(int nTasklets, std::string_view kernelName) const;

private:
  std::unique_ptr<ProgramBuilderImpl> impl_;
};

} // namespace upmem_cm
