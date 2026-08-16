#include "upmem_cost_model/ProgramBuilder.h"
#include "ProgramBuilderImpl.h"

#include <chrono>
#include <map>
#include <string>

namespace upmem_cm {

// ─────────────────────────────────────────────────────────────────────────────
// Scope helpers
// ─────────────────────────────────────────────────────────────────────────────

std::vector<SimInsn> &ProgramBuilderImpl::currentInsns() {
  if (scope_is_cond_.empty()) return main_;
  return scope_is_cond_.back() ? cond_stack_.back().body : stack_.back().body;
}

std::vector<LoopInfo> &ProgramBuilderImpl::currentLoops() {
  if (scope_is_cond_.empty()) return main_loops_;
  return scope_is_cond_.back() ? cond_stack_.back().body_loops : stack_.back().body_loops;
}

std::vector<ConditionalInfo> &ProgramBuilderImpl::currentConds() {
  if (scope_is_cond_.empty()) return main_conds_;
  return scope_is_cond_.back() ? cond_stack_.back().body_conds : stack_.back().body_conds;
}

// ─────────────────────────────────────────────────────────────────────────────
// Emit helpers
// ─────────────────────────────────────────────────────────────────────────────

namespace {
SimOp statOpToSimOp(StatOp op) {
  switch (op) {
  case StatOp::LOAD:
    return SimOp::Load;
  case StatOp::STORE:
    return SimOp::Store;
  case StatOp::ADD:
    return SimOp::Add;
  case StatOp::SUB:
    return SimOp::Sub;
  case StatOp::MUL:
    return SimOp::Mul;
  case StatOp::DIV:
    return SimOp::Div;
  }
  return SimOp::Add;
}
} // namespace

void ProgramBuilderImpl::emitMovLike(SimOp op, const char *note) {
  SimInsn insn{11, SimKind::PIPE};
  insn.op = op;
  insn.note = note;
  currentInsns().push_back(insn);
}

void ProgramBuilderImpl::emitPipe(StatOp op, DType dt, int mul_imm) {
  uint32_t lat;
  if (op == StatOp::MUL && isMulLibCallType(dt)) {
    // 32-bit multiply is a __mulsi3 call, never a single instruction; the
    // static table has no entry for it. See lookupMulImmLatency.
    lat = mul_imm >= 0 ? lookupMulImmLatency(mul_imm) : kMulGeneralLatency;
  } else {
    lat = lookupStaticLatency(op, dt);
  }
  SimInsn insn{lat, SimKind::PIPE};
  insn.op = statOpToSimOp(op);
  insn.dtype = dt;
  if (op == StatOp::MUL)
    insn.mul_imm = mul_imm;
  currentInsns().push_back(insn);
}

void ProgramBuilderImpl::emitDma(bool is_load, int64_t n_elems, DType dtype) {
  int64_t size_bytes = n_elems * dtypeBytes(dtype);
  uint32_t lat = lookupDmaLatency(is_load, size_bytes);
  SimInsn insn{lat, is_load ? SimKind::DMA_LOAD : SimKind::DMA_STORE};
  insn.op = is_load ? SimOp::DmaLoad : SimOp::DmaStore;
  insn.dtype = dtype;
  insn.dma_bytes = static_cast<uint32_t>(size_bytes);
  currentInsns().push_back(insn);
}

// ─────────────────────────────────────────────────────────────────────────────
// Value registry
// ─────────────────────────────────────────────────────────────────────────────

ProgramBuilderImpl::ValId ProgramBuilderImpl::newVal(DType dtype, bool is_const,
                                                     int64_t cv) {
  ValId id = static_cast<ValId>(vals_.size());
  vals_.push_back({dtype, is_const, cv});
  return id;
}

// ─────────────────────────────────────────────────────────────────────────────
// Builder methods
// ─────────────────────────────────────────────────────────────────────────────

ProgramBuilderImpl::BufId ProgramBuilderImpl::addBuffer(std::string_view name,
                                                        MemSpace space,
                                                        DType dtype) {
  BufId id = static_cast<BufId>(buffers_.size());
  buffers_.push_back({std::string(name), space, dtype});
  return id;
}

void ProgramBuilderImpl::createTransfer(BufId src, BufId dst, int64_t n_elems,
                                        bool src_iv_indexed,
                                        bool dst_iv_indexed) {
  bool is_load = (buffers_[src].space == MemSpace::MRAM);
  BufId mram_buf = is_load ? src : dst;
  bool mram_iv = is_load ? src_iv_indexed : dst_iv_indexed;

  emitMovLike(SimOp::Mov, "mram address: mov r_tmp, -1");
  emitMovLike(SimOp::Lsr, "mram address: lsr r_tmp, 3");

  if (mram_iv && !stack_.empty()) {
    LoopFrame &top = stack_.back();
    if (!top.load_offsets.count(mram_buf))
      top.load_offsets[mram_buf] = next_reg_++;
    if (src_iv_indexed)
      top.xfer_src_bumps.push_back({src, n_elems});
    emitMovLike(SimOp::Lsl, "mram address: scale offset to bytes");
    emitMovLike(SimOp::Add, "mram address: add base");
  }

  emitDma(is_load, n_elems, buffers_[mram_buf].dtype);
}

ProgramBuilderImpl::ValId ProgramBuilderImpl::createLoad(BufId buf,
                                                         bool iv_indexed) {
  if (iv_indexed && !stack_.empty()) {
    LoopFrame &top = stack_.back();
    if (!top.load_offsets.count(buf))
      top.load_offsets[buf] = next_reg_++;
  }
  emitPipe(StatOp::LOAD, buffers_[buf].dtype);
  return newVal(buffers_[buf].dtype);
}

void ProgramBuilderImpl::createStore(BufId buf, ValId /*val*/,
                                     bool iv_indexed) {
  if (iv_indexed && !stack_.empty()) {
    LoopFrame &top = stack_.back();
    if (!top.store_offsets.count(buf))
      top.store_offsets[buf] = next_reg_++;
  }
  emitPipe(StatOp::STORE, buffers_[buf].dtype);
}

void ProgramBuilderImpl::createReduceStore(BufId buf, ArithOp op,
                                           ValId /*val*/) {
  DType dt = buffers_[buf].dtype;
  emitPipe(StatOp::LOAD, dt);
  emitPipe(arithToStatOp(op), dt);
  emitPipe(StatOp::STORE, dt);
}

ProgramBuilderImpl::ValId
ProgramBuilderImpl::createArith(ArithOp op, DType dtype, ValId lhs, ValId rhs) {
  int mul_imm = -1;
  if (op == ArithOp::MUL && (dtype == DType::U32 || dtype == DType::I32)) {
    auto check = [&](ValId vid) -> int {
      if (vid < vals_.size() && vals_[vid].is_const) {
        int64_t v = vals_[vid].const_value;
        if (v > 0 && (v & (v - 1)) == 0)
          return static_cast<int>(v);
      }
      return -1;
    };
    int ml = check(lhs), mr = check(rhs);
    mul_imm = (ml >= 0) ? ml : mr;
  }
  emitPipe(arithToStatOp(op), dtype, mul_imm);
  return newVal(dtype);
}

ProgramBuilderImpl::ValId ProgramBuilderImpl::createConst(int64_t value,
                                                          DType dtype) {
  emitMovLike(SimOp::Mov, "materialise constant");
  return newVal(dtype, /*is_const=*/true, value);
}

void ProgramBuilderImpl::beginLoop(int64_t lb, int64_t ub, int64_t step) {
  uint32_t iv_reg = next_reg_++;
  stack_.push_back(LoopFrame{lb, ub, step, iv_reg, {}, {}, {}, {}, {}, {}});
  scope_is_cond_.push_back(false);
}

void ProgramBuilderImpl::endLoop() {
  LoopFrame frame = std::move(stack_.back());
  stack_.pop_back();
  scope_is_cond_.pop_back();

  uint32_t trips =
      (frame.ub > frame.lb && frame.step > 0)
          ? static_cast<uint32_t>((frame.ub - frame.lb + frame.step - 1) /
                                  frame.step)
          : 0u;

  std::vector<SimInsn> loop_buf;
  std::vector<LoopInfo> loop_loops;

  // Prologue: the induction variable's initialisation. 
  loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Mov, DType::I64, 0, -1,
                             "loop prologue: iv init"});

  // Prologue: offset resets for iv-indexed loads and stores (union, deduped)
  std::map<uint32_t, uint32_t> all_offsets;
  for (auto &[bid, reg] : frame.load_offsets)
    all_offsets.emplace(bid, reg);
  for (auto &[bid, reg] : frame.store_offsets)
    all_offsets.emplace(bid, reg);
  for (size_t i = 0; i < all_offsets.size(); ++i)
    loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Mov, DType::I64, 0, -1,
                               "loop prologue: offset init"});

  // Prologue: iv-indexed transfer src offset inits (if not already reset above)
  std::map<uint32_t, bool> xfer_init;
  for (auto &xb : frame.xfer_src_bumps) {
    if (!all_offsets.count(xb.buf_id) && !xfer_init.count(xb.buf_id)) {
      loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Mov, DType::I64, 0,
                                 -1, "loop prologue: transfer offset init"});
      xfer_init[xb.buf_id] = true;
    }
  }

  uint32_t head_idx = static_cast<uint32_t>(loop_buf.size());
  uint32_t body_offset = head_idx;

  // Body (nested loop/cond indices adjusted by body insertion offset)
  std::vector<ConditionalInfo> loop_conds;
  for (auto &li : frame.body_loops)
    loop_loops.push_back(
        {li.head_idx + body_offset, li.jump_idx + body_offset, li.trips});
  for (auto &ci : frame.body_conds)
    loop_conds.push_back(
        {ci.guard_idx + body_offset, ci.end_idx + body_offset, ci.allowed_tids});
  for (auto &insn : frame.body)
    loop_buf.push_back(insn);

  // Epilogue: offset bumps
  for (size_t i = 0; i < frame.load_offsets.size(); ++i)
    loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Add, DType::I64, 0, -1,
                               "loop epilogue: load pointer bump"});
  for (size_t i = 0; i < frame.store_offsets.size(); ++i)
    loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Add, DType::I64, 0, -1,
                               "loop epilogue: store pointer bump"});
  for (size_t i = 0; i < frame.xfer_src_bumps.size(); ++i)
    loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Add, DType::I64, 0, -1,
                               "loop epilogue: transfer offset bump"});

  loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Add, DType::I64, 0, -1,
                             "loop epilogue: iv increment"});

  uint32_t jump_idx = static_cast<uint32_t>(loop_buf.size());
  loop_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Jump, DType::I64, 0, -1,
                             "loop epilogue: back edge"});

  loop_loops.push_back({head_idx, jump_idx, trips});

  // Flush into outer scope
  uint32_t outer_offset = static_cast<uint32_t>(currentInsns().size());
  for (auto &li : loop_loops)
    currentLoops().push_back(
        {li.head_idx + outer_offset, li.jump_idx + outer_offset, li.trips});
  for (auto &ci : loop_conds)
    currentConds().push_back(
        {ci.guard_idx + outer_offset, ci.end_idx + outer_offset, ci.allowed_tids});
  for (auto &insn : loop_buf)
    currentInsns().push_back(insn);
}

void ProgramBuilderImpl::beginIfThread(std::vector<int> allowedTids) {
  cond_stack_.push_back(CondFrame{std::move(allowedTids), {}, {}, {}});
  scope_is_cond_.push_back(true);
}

void ProgramBuilderImpl::endIfThread() {
  CondFrame frame = std::move(cond_stack_.back());
  cond_stack_.pop_back();
  scope_is_cond_.pop_back();

  std::vector<SimInsn> local_buf;
  std::vector<LoopInfo> local_loops;
  std::vector<ConditionalInfo> local_conds;

  // Guard instruction: 11-cycle branch check, paid only by allowed threads.
  uint32_t guard_local = 0;
  local_buf.push_back(SimInsn{11, SimKind::PIPE, SimOp::Jump, DType::I64, 0, -1,
                              "ifthread: guard check"});
  uint32_t body_off = 1; // body starts immediately after the guard

  for (auto &li : frame.body_loops)
    local_loops.push_back(
        {li.head_idx + body_off, li.jump_idx + body_off, li.trips});
  for (auto &ci : frame.body_conds)
    local_conds.push_back(
        {ci.guard_idx + body_off, ci.end_idx + body_off, ci.allowed_tids});
  for (auto &insn : frame.body)
    local_buf.push_back(insn);

  uint32_t end_local = static_cast<uint32_t>(local_buf.size());
  local_conds.push_back({guard_local, end_local, frame.allowed_tids});

  // Flush into outer scope
  uint32_t outer_off = static_cast<uint32_t>(currentInsns().size());
  for (auto &li : local_loops)
    currentLoops().push_back(
        {li.head_idx + outer_off, li.jump_idx + outer_off, li.trips});
  for (auto &ci : local_conds)
    currentConds().push_back(
        {ci.guard_idx + outer_off, ci.end_idx + outer_off, ci.allowed_tids});
  for (auto &insn : local_buf)
    currentInsns().push_back(insn);
}

// ─────────────────────────────────────────────────────────────────────────────
// ProgramBuilder public API — thin delegation to impl_
// ─────────────────────────────────────────────────────────────────────────────

ProgramBuilder::ProgramBuilder()
    : impl_(std::make_unique<ProgramBuilderImpl>()) {}
ProgramBuilder::~ProgramBuilder() = default;

ProgramBuilder::BufId ProgramBuilder::addBuffer(std::string_view name,
                                                MemSpace space, DType dtype) {
  return impl_->addBuffer(name, space, dtype);
}
void ProgramBuilder::createTransfer(BufId src, BufId dst, int64_t n_elems,
                                    bool src_iv, bool dst_iv) {
  impl_->createTransfer(src, dst, n_elems, src_iv, dst_iv);
}
ProgramBuilder::ValId ProgramBuilder::createLoad(BufId buf, bool iv_indexed) {
  return impl_->createLoad(buf, iv_indexed);
}
void ProgramBuilder::createStore(BufId buf, ValId val, bool iv_indexed) {
  impl_->createStore(buf, val, iv_indexed);
}
void ProgramBuilder::createReduceStore(BufId buf, ArithOp op, ValId val) {
  impl_->createReduceStore(buf, op, val);
}
ProgramBuilder::ValId ProgramBuilder::createArith(ArithOp op, DType dtype,
                                                  ValId lhs, ValId rhs) {
  return impl_->createArith(op, dtype, lhs, rhs);
}
ProgramBuilder::ValId ProgramBuilder::createConst(int64_t value, DType dtype) {
  return impl_->createConst(value, dtype);
}
void ProgramBuilderImpl::createBarrier() {
  currentInsns().push_back(SimInsn{0, SimKind::BARRIER, SimOp::Barrier,
                                  DType::I64, 0, -1, "barrier_wait"});
}

void ProgramBuilder::createBarrier() { impl_->createBarrier(); }

void ProgramBuilder::beginIfThread(std::vector<int> allowedTids) {
  impl_->beginIfThread(std::move(allowedTids));
}
void ProgramBuilder::endIfThread() { impl_->endIfThread(); }

void ProgramBuilder::beginLoop(int64_t lb, int64_t ub, int64_t step) {
  impl_->beginLoop(lb, ub, step);
}
void ProgramBuilder::endLoop() { impl_->endLoop(); }
std::optional<double>
ProgramBuilder::simulate(int nTasklets, std::chrono::milliseconds timeoutMs, bool fast,
                         uint64_t freqHz) {
  return impl_->simulate(nTasklets, freqHz, timeoutMs, fast);
}

} // namespace upmem_cm
