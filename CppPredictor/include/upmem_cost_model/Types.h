#pragma once
#include <cstdint>
#include <memory>

namespace upmem_cm {

enum class MemSpace : uint8_t { WRAM = 0, MRAM = 1 };

enum class DType : uint8_t { U8, I8, U16, I16, U32, I32, F32, U64, I64, F64 };

enum class ArithOp : uint8_t { ADD, SUB, MUL, DIV };
enum class StatOp : uint8_t { LOAD, STORE, ADD, SUB, MUL, DIV };

inline int dtypeBits(DType dt) {
  switch (dt) {
  case DType::U8:
  case DType::I8:
    return 8;
  case DType::U16:
  case DType::I16:
    return 16;
  case DType::U32:
  case DType::I32:
  case DType::F32:
    return 32;
  case DType::U64:
  case DType::I64:
  case DType::F64:
    return 64;
  }
  return 32;
}

inline int dtypeBytes(DType dt) { return dtypeBits(dt) / 8; }

inline StatOp arithToStatOp(ArithOp op) {
  switch (op) {
  case ArithOp::ADD:
    return StatOp::ADD;
  case ArithOp::SUB:
    return StatOp::SUB;
  case ArithOp::MUL:
    return StatOp::MUL;
  case ArithOp::DIV:
    return StatOp::DIV;
  }
  return StatOp::ADD;
}

} // namespace upmem_cm
