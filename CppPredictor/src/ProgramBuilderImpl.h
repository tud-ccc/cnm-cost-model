#pragma once

#include "upmem_cost_model/Simulation.h"
#include "upmem_cost_model/Types.h"

#include <chrono>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace upmem_cm {

// ─────────────────────────────────────────────────────────────────────────────
// Shared low-level IR types (used by both builder and simulator)
// ─────────────────────────────────────────────────────────────────────────────

enum class SimKind : uint8_t {
  PIPE = 0,
  DMA_LOAD = 1,
  DMA_STORE = 2,
  BARRIER = 3
};

/// What an instruction *is*, as opposed to how it is timed. Purely
/// descriptive: the simulator dispatches on `kind` alone and never reads this.
/// It exists so a built program can be re-emitted symbolically (see
/// emitJson) and re-costed by a different cost model against that model's own
/// latency tables — which is how this engine gets validated against the Python
/// reference implementation. Without it, `latency` is all that survives and a
/// dump would only ever re-measure our own tables.
///
/// Mov/Lsl/Lsr/Jump all cost the same 11 cycles here and in the reference
/// model; they are kept apart anyway so a dump can be diffed against real DPU
/// disassembly instruction for instruction.
enum class SimOp : uint8_t {
  Mov,
  Lsl,
  Lsr,
  Jump,
  Load,
  Store,
  Add,
  Sub,
  Mul,
  Div,
  DmaLoad,
  DmaStore,
  Barrier,
};

struct SimInsn {
  uint32_t latency;
  SimKind kind;
  SimOp op = SimOp::Mov;
  DType dtype = DType::I32;
  /// DmaLoad/DmaStore only: transfer size, which is what the reference
  /// model's DMA table is keyed on (it has no notion of element counts).
  uint32_t dma_bytes = 0;
  /// Mul only: the power-of-2 immediate that selected the fast multiply
  /// path, or -1 for a general multiply.
  int32_t mul_imm = -1;
  /// Optional static string literal naming what this instruction came from
  /// ("iv increment", "pointer bump", ...). Never owned, never freed.
  const char *note = nullptr;
};

struct LoopInfo {
  uint32_t head_idx;
  uint32_t jump_idx;
  uint32_t trips;
};

/// Records one ifthread conditional block in the flat instruction stream.
/// Non-allowed threads jump from guard_idx directly to end_idx.
/// Allowed threads execute the guard instruction (11-cycle branch check) and
/// fall through the body normally.
struct ConditionalInfo {
  uint32_t guard_idx;
  uint32_t end_idx;
  std::vector<int> allowed_tids;
};

// A 32-bit integer multiply is NOT an instruction on this hardware. It is a
// __mulsi3 library call that, after an operand swap, runs one mul_step per
// significant bit of the smaller operand: 88 cycles of call overhead plus 11
// per step. Cost is therefore data-dependent, anywhere from one step to the
// full 32.
//
// When one operand is a known power-of-2 constant the step count is exactly
// log2(imm), which is what lookupMulImmLatency returns -- reproducing the
// reference model's measured MUL/uint32_t table exactly, entry for entry
// (88 at imm=1, 99 at imm=2, 110 at imm=4, out to 418 at imm=2^30).
//
// With both operands unknown at compile time the step count cannot be derived,
// only assumed — so kMulOperandBits states the assumption explicitly: the
// expected significant-bit count of the smaller operand under the evaluation's
// data distribution.
//
// It is currently set for operands uniform on [0, 50), which is what both this
// evaluation (experiments/bench/common.hpp) and the ATiM baseline it is
// compared against (evaluation/base.py, intdist=50) generate. E[bits(min(a,b))]
// over that range is 4.03. Other ranges: [0,128) gives 5.35, and full-width
// 32-bit operands give 31 — a ~3x spread in the cost of this instruction, so
// this constant has to move with the benchmark data, and any absolute latency
// reported by this model is only valid for the distribution named here.
// 88, not 89: the reference model's measured MUL/uint32_t table is exactly
// 88 + 11*log2(imm) at every one of its 31 entries, so this intercept makes
// lookupMulImmLatency reproduce that table rather than sit a cycle above it.
inline constexpr uint32_t kMulCallOverhead = 88;
inline constexpr uint32_t kMulStepLatency = 11;
inline constexpr uint32_t kMulOperandBits = 4;
inline constexpr uint32_t kMulGeneralLatency =
    kMulCallOverhead + kMulOperandBits * kMulStepLatency;

inline bool isMulLibCallType(DType dt) {
  return dt == DType::U32 || dt == DType::I32;
}

// MUL uint32 by power-of-2 immediate: 88 + log2(imm) * 11
inline uint32_t lookupMulImmLatency(int64_t imm) {
  if (imm <= 0 || (imm & (imm - 1)) != 0)
    return kMulGeneralLatency;
  int log2v = 0;
  for (int64_t v = imm; v > 1; v >>= 1)
    ++log2v;
  return static_cast<uint32_t>(kMulCallOverhead + log2v * kMulStepLatency);
}

// ─────────────────────────────────────────────────────────────────────────────
// Builder internal descriptors
// ─────────────────────────────────────────────────────────────────────────────

struct BufDesc {
  std::string name;
  MemSpace space;
  DType dtype;
};
struct ValInfo {
  DType dtype;
  bool is_const = false;
  int64_t const_value = 0;
};

struct LoopFrame {
  int64_t lb, ub, step;
  uint32_t iv_reg;
  std::map<uint32_t, uint32_t> load_offsets;  // BufId → offset_reg
  std::map<uint32_t, uint32_t> store_offsets; // BufId → offset_reg
  struct XferSrcBump {
    uint32_t buf_id;
    int64_t n_elems;
  };
  std::vector<XferSrcBump> xfer_src_bumps;
  std::vector<SimInsn> body;
  std::vector<LoopInfo> body_loops;
  std::vector<ConditionalInfo> body_conds;
};

struct CondFrame {
  std::vector<int> allowed_tids;
  std::vector<SimInsn> body;
  std::vector<LoopInfo> body_loops;
  std::vector<ConditionalInfo> body_conds;
};

// ─────────────────────────────────────────────────────────────────────────────
// ProgramBuilderImpl
// ─────────────────────────────────────────────────────────────────────────────

class ProgramBuilderImpl {
public:
  using BufId = uint32_t;
  using ValId = uint32_t;

  std::vector<BufDesc> buffers_;
  std::vector<ValInfo> vals_;
  uint32_t next_reg_ = 0;

  std::vector<LoopFrame> stack_;
  std::vector<CondFrame> cond_stack_;
  // One entry per open scope; true = conditional, false = loop.
  // The innermost scope is at the back.
  std::vector<bool> scope_is_cond_;

  std::vector<SimInsn> main_;
  std::vector<LoopInfo> main_loops_;
  std::vector<ConditionalInfo> main_conds_;

  // Current emit target (innermost scope body or top-level program)
  std::vector<SimInsn> &currentInsns();
  std::vector<LoopInfo> &currentLoops();
  std::vector<ConditionalInfo> &currentConds();

  // Low-level emit helpers
  void emitMovLike(SimOp op, const char *note);
  void emitPipe(StatOp op, DType dt, int mul_imm = -1);
  void emitDma(bool is_load, int64_t n_elems, DType dtype);

  /// Re-emit the built program as the JSON interchange format consumed by the
  /// Python reference model's `cnmprog` reader — a symbolic instruction tree
  /// (opcodes, dtypes, DMA byte counts, loop trip counts) with no latencies in
  /// it, so the reader resolves every cost from its own tables.
  std::string emitJson(int nTasklets, std::string_view kernelName) const;

  ValId newVal(DType dtype, bool is_const = false, int64_t cv = 0);

  // Builder API (defined in ProgramBuilder.cpp)
  BufId addBuffer(std::string_view name, MemSpace space, DType dtype);
  void createTransfer(BufId src, BufId dst, int64_t n_elems,
                      bool src_iv_indexed, bool dst_iv_indexed);
  ValId createLoad(BufId buf, bool iv_indexed);
  void createStore(BufId buf, ValId val, bool iv_indexed);
  void createReduceStore(BufId buf, ArithOp op, ValId val);
  ValId createArith(ArithOp op, DType dtype, ValId lhs, ValId rhs);
  ValId createConst(int64_t value, DType dtype);
  void createBarrier();
  void beginLoop(int64_t lb, int64_t ub, int64_t step);
  void endLoop();
  void beginIfThread(std::vector<int> allowedTids);
  void endIfThread();

  // Simulation (defined in Simulator.cpp)
  std::optional<double>
  simulate(int nTasklets, uint64_t freqHz,
           std::chrono::milliseconds timeoutMs = std::chrono::milliseconds(0),
           bool fast = false);

  /// The scheduler both modes run. With `extrapolate`, a loop whose cost per
  /// repeat has stopped moving is skipped whole at that rate instead of
  /// dispatched repeat by repeat -- the reference model's runNested. Without
  /// it, every repeat is dispatched -- its runProgram. Same answers where
  /// the extrapolation converges, and it is only allowed to converge where
  /// widening the lookback stops changing the average.
  std::optional<double> simulateScheduled(int nTasklets, uint64_t freqHz,
                                          std::chrono::milliseconds timeoutMs,
                                          bool extrapolate);
  double simulateFast(int nTasklets, uint64_t freqHz);
};

} // namespace upmem_cm
