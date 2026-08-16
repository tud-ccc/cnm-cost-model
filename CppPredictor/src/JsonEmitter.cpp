// Re-emits a built program as the JSON interchange format the Python
// reference model reads (Predictor/cnmprog.py).
//
// The format carries NO latencies — only opcodes, dtypes, DMA byte counts and
// loop trip counts — so the reader resolves every cost from its own calibrated
// tables and a cross-check measures scheduling rather than our latency tables.
//
// Structure is recovered from the flat instruction array plus LoopInfo's
// (head_idx, jump_idx, trips): a loop body is [head_idx, jump_idx] inclusive
// (see Kernel::nextInsn in Simulator.cpp — the head is executed on entry and
// the back edge is the last body instruction), and loops nest properly, so a
// single recursive walk reconstructs the tree.

#include "upmem_cost_model/ProgramBuilder.h"
#include "ProgramBuilderImpl.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace upmem_cm {
namespace {

const char *simOpName(SimOp op) {
  switch (op) {
  case SimOp::Mov:
    return "MOV";
  case SimOp::Lsl:
    return "LSL";
  case SimOp::Lsr:
    return "LSR";
  case SimOp::Jump:
    return "J";
  case SimOp::Load:
    return "LOAD";
  case SimOp::Store:
    return "STORE";
  case SimOp::Add:
    return "ADD";
  case SimOp::Sub:
    return "SUB";
  case SimOp::Mul:
    return "MUL";
  case SimOp::Div:
    return "DIV";
  case SimOp::DmaLoad:
    return "DMA_LOAD";
  case SimOp::DmaStore:
    return "DMA_STORE";
  case SimOp::Barrier:
    return "BARRIER";
  }
  return "MOV";
}

/// The reference model's type names. Its DataType enum has no 8-bit member and
/// does not distinguish signedness, so the mapping collapses exactly the way
/// dtypeGroup() in Simulation.h already does — the two models agree on which
/// types share a latency.
const char *dtypeName(DType dt) {
  switch (dt) {
  case DType::U8:
  case DType::I8:
  case DType::U16:
  case DType::I16:
    return "uint16_t";
  case DType::U32:
  case DType::I32:
    return "uint32_t";
  case DType::F32:
    return "float";
  case DType::U64:
  case DType::I64:
    return "uint64_t";
  case DType::F64:
    return "double";
  }
  return "uint32_t";
}

/// True for opcodes the reference model prices without a dtype (its LUT
/// aliases all of them onto the 32-bit ADD entry).
bool isUntyped(SimOp op) {
  return op == SimOp::Mov || op == SimOp::Lsl || op == SimOp::Lsr ||
         op == SimOp::Jump;
}

void appendEscaped(std::string &out, std::string_view s) {
  for (char c : s) {
    if (c == '"' || c == '\\')
      out += '\\';
    out += c;
  }
}

struct Emitter {
  const std::vector<SimInsn> &insns;
  std::unordered_map<uint32_t, const LoopInfo *> heads;
  std::unordered_map<uint32_t, const ConditionalInfo *> guards;
  std::string out;

  void indent(int depth) { out.append(static_cast<size_t>(depth) * 2, ' '); }

  void emitInsn(const SimInsn &insn, int depth) {
    indent(depth);
    out += "{\"op\": \"";
    out += simOpName(insn.op);
    out += '"';
    std::string note = insn.note ? insn.note : "";
    if (insn.op == SimOp::DmaLoad || insn.op == SimOp::DmaStore) {
      out += ", \"bytes\": " + std::to_string(insn.dma_bytes);
    } else if (!isUntyped(insn.op) && insn.op != SimOp::Barrier) {
      out += ", \"dtype\": \"";
      out += dtypeName(insn.dtype);
      out += '"';
      if (insn.op == SimOp::Mul && isMulLibCallType(insn.dtype)) {
        // A 32-bit multiply is a __mulsi3 call rather than an instruction
        // (see kMulCallOverhead), and both models price it per step count:
        // there is no bare MUL/uint32_t entry on either side, and the
        // reference model refuses such an instruction outright. So the call
        // is always written out as the multiplier whose step count it costs
        // -- the real one where the builder found a power-of-2 constant, and
        // otherwise the one standing for kMulOperandBits assumed significant
        // bits, which is exactly what kMulGeneralLatency charges here.
        bool assumed = insn.mul_imm < 0;
        int64_t imm = assumed ? (int64_t{1} << kMulOperandBits) : insn.mul_imm;
        out += ", \"imm\": " + std::to_string(imm);
        if (assumed) {
          if (!note.empty())
            note += "; ";
          note += "operands unknown: assumed " +
                  std::to_string(kMulOperandBits) +
                  " significant bits, not a literal multiplier";
        }
      }
    }
    if (!note.empty()) {
      out += ", \"note\": \"";
      appendEscaped(out, note);
      out += '"';
    }
    out += '}';
  }

  /// Emits [lo, hi) as a segment node. `ignoreAt` suppresses the loop/guard
  /// lookup at exactly one index — used when recursing into a construct whose
  /// body starts at its own marker instruction, which would otherwise be
  /// re-entered forever.
  void emitRange(uint32_t lo, uint32_t hi, int depth,
                 std::optional<uint32_t> ignoreAt = std::nullopt) {
    struct Part {
      uint32_t lo, hi;    // leaf run, when kind == Leaf
      const LoopInfo *loop = nullptr;
      const ConditionalInfo *cond = nullptr;
    };
    std::vector<Part> parts;
    uint32_t leaf_lo = lo;

    auto flushLeaf = [&](uint32_t upto) {
      if (upto > leaf_lo)
        parts.push_back({leaf_lo, upto});
    };

    for (uint32_t pc = lo; pc < hi;) {
      bool ignored = ignoreAt && *ignoreAt == pc;
      auto git = ignored ? guards.end() : guards.find(pc);
      auto hit = ignored ? heads.end() : heads.find(pc);

      if (git != guards.end()) {
        flushLeaf(pc);
        parts.push_back({0, 0, nullptr, git->second});
        pc = git->second->end_idx;
        leaf_lo = pc;
      } else if (hit != heads.end()) {
        flushLeaf(pc);
        parts.push_back({0, 0, hit->second, nullptr});
        pc = hit->second->jump_idx + 1;
        leaf_lo = pc;
      } else {
        pc++;
      }
    }
    flushLeaf(hi);

    // A single leaf needs no surrounding "seq" wrapper.
    if (parts.size() == 1 && parts[0].loop == nullptr &&
        parts[0].cond == nullptr) {
      emitLeaf(parts[0].lo, parts[0].hi, depth);
      return;
    }
    if (parts.empty()) {
      out += "{\"leaf\": []}";
      return;
    }

    out += "{\"seq\": [\n";
    for (size_t i = 0; i < parts.size(); ++i) {
      indent(depth + 1);
      emitPart(parts[i].loop, parts[i].cond, parts[i].lo, parts[i].hi,
               depth + 1);
      out += (i + 1 < parts.size()) ? ",\n" : "\n";
    }
    indent(depth);
    out += "]}";
  }

  void emitPart(const LoopInfo *loop, const ConditionalInfo *cond, uint32_t lo,
                uint32_t hi, int depth) {
    if (loop) {
      out += "{\"repeat\": {\"count\": " + std::to_string(loop->trips) +
             ", \"body\": ";
      emitRange(loop->head_idx, loop->jump_idx + 1, depth, loop->head_idx);
      out += "}}";
      return;
    }
    if (cond) {
      // No counterpart in the reference model — emitted so its reader fails
      // loudly rather than silently mispricing a predicated region.
      out += "{\"ifthread\": {\"tids\": [";
      for (size_t i = 0; i < cond->allowed_tids.size(); ++i) {
        out += std::to_string(cond->allowed_tids[i]);
        if (i + 1 < cond->allowed_tids.size())
          out += ", ";
      }
      out += "], \"body\": ";
      emitRange(cond->guard_idx, cond->end_idx, depth, cond->guard_idx);
      out += "}}";
      return;
    }
    emitLeaf(lo, hi, depth);
  }

  void emitLeaf(uint32_t lo, uint32_t hi, int depth) {
    out += "{\"leaf\": [\n";
    for (uint32_t pc = lo; pc < hi; ++pc) {
      emitInsn(insns[pc], depth + 1);
      out += (pc + 1 < hi) ? ",\n" : "\n";
    }
    indent(depth);
    out += "]}";
  }
};

} // namespace

std::string ProgramBuilderImpl::emitJson(int nTasklets,
                                         std::string_view kernelName) const {
  Emitter em{main_, {}, {}, {}};
  for (const auto &li : main_loops_)
    em.heads[li.head_idx] = &li;
  for (const auto &ci : main_conds_)
    em.guards[ci.guard_idx] = &ci;

  em.out = "{\n  \"kernel\": \"";
  appendEscaped(em.out, kernelName);
  em.out += "\",\n  \"tasklets\": " + std::to_string(nTasklets) +
            ",\n  \"body\": ";
  em.emitRange(0, static_cast<uint32_t>(main_.size()), 1);
  em.out += "\n}\n";
  return em.out;
}

std::string ProgramBuilder::emitJson(int nTasklets,
                                     std::string_view kernelName) const {
  return impl_->emitJson(nTasklets, kernelName);
}

} // namespace upmem_cm
