#pragma once
#include "Types.h"
#include <cmath>

namespace upmem_cm {

// ─────────────────────────────────────────────────────────────────────────────
// Latency tables (constexpr data + inline lookups)
// ─────────────────────────────────────────────────────────────────────────────

// [op][dtype_group] — groups: u16, u32, f32, u64, f64
//
// These are the reference model's own numbers, entry for entry: the values
// its loadBaseInstructionsDict() produces from
// cnm-cost-model/UPMEM/LUT/output/pipeline_normal_parsed.csv. Feeding the
// same program through both engines is only a comparison of their scheduling
// if their tables agree, so this table is not independently rounded from the
// measurements -- it is transcribed from theirs, including the truncation.
//
// The truncation is worth knowing about before someone "fixes" a value here:
// load_lut.round() returns int(n) unless the fraction is exactly 0.5, so a
// measured 10.996 becomes 10 and not 11. Every entry is therefore up to a
// cycle below the measurement, and matching that is the point.
//
// MUL u32 is absent from the reference table on purpose -- the hardware has
// no such instruction, it is a __mulsi3 call (see kMulCallOverhead), and
// emitPipe never reaches this row for a 32-bit multiply. The 12 keeps the
// row rectangular.
inline constexpr uint32_t kStaticLatency[6][5] = {
    {11, 10, 11, 10, 11},         // LOAD
    {11, 10, 11, 11, 11},         // STORE
    {22, 10, 683, 11, 1002},      // ADD
    {22, 10, 517, 11, 672},       // SUB
    {11, 12, 1939, 1421, 7195},   // MUL  (u32: unreachable, see above)
    {110, 99, 11306, 518, 23526}, // DIV
};

inline int dtypeGroup(DType dt) {
  switch (dt) {
  case DType::U8:
  case DType::I8:
  case DType::U16:
  case DType::I16:
    return 0;
  case DType::U32:
  case DType::I32:
    return 1;
  case DType::F32:
    return 2;
  case DType::U64:
  case DType::I64:
    return 3;
  case DType::F64:
    return 4;
  }
  return 1;
}

inline uint32_t lookupStaticLatency(StatOp op, DType dt) {
  return kStaticLatency[static_cast<int>(op)][dtypeGroup(dt)];
}
struct DmaEntry {
  uint32_t bytes;
  /// Fractional, as the reference model's are: these come out of a
  /// regression and it schedules on them unrounded. The scheduler carries
  /// its cycle counters as doubles for the same reason.
  double load;
  double store;
  /// What one request of this size occupies the shared DMA queue for, which
  /// is not how long it takes to complete: a request enters the queue when
  /// the queue frees up and completes `load`/`store` cycles after that, but
  /// the next request may enter only `load_service`/`store_service` cycles
  /// later. Under contention the service interval, not the latency, is what
  /// sets the rate.
  double load_service;
  double store_service;
};
// Barrier latency, as the reference model computes it: a power law
// cycles = a * nTasklets^b, which its loadBarrierCostModel() fits by log-log
// regression over cnm-cost-model/UPMEM/BarrierExperiment/output/
// barrier_cost.csv. The coefficients below are that fit; rerun the loader to
// re-derive them if that CSV changes.
//
// This replaced a three-segment linear fit of the same measurements. The two
// disagree by up to 20% (at 16 tasklets, 2551 cycles against 3066), so which
// one is in force is not a detail -- but a cross-check whose two engines
// price a barrier differently measures the barrier tables and not the
// scheduling, which is what the exchange format exists to isolate.
inline constexpr double kBarrierCostA = 111.8924;
inline constexpr double kBarrierCostB = 1.1941;

inline uint32_t lookupBarrierLatency(int ntasklets) {
  return static_cast<uint32_t>(
      kBarrierCostA * std::pow(static_cast<double>(ntasklets), kBarrierCostB));
}

// The reference model's DMA numbers, to the digit: latency and service
// interval, load and store. They are fractional because they are regressed
// rather than read off a counter -- loadDmaLatencies() fits the per-call cost
// across dma_count at one thread, and loadDmaServiceIntervals() takes the
// local slope between the two highest-concurrency points, where the queue is
// already saturated.
//
// At 2048 bytes there is no service measurement and the latency stands in for
// it, exactly as it does there.
inline constexpr DmaEntry kDmaLatency[] = {
    // bytes    load    store  load_svc store_svc
    {8, 63.1, 55.7, 33.0625, 23.125},
    {16, 68.3, 56.6, 42.82, 42.82},
    {32, 71.4, 64.3, 58.0, 58.0},
    {64, 96.2, 86.2, 68.43, 68.43},
    {128, 127.3, 119.3, 118.875, 105.125},
    {256, 195.2, 187.3, 177.875, 168.75},
    {512, 330.7, 325.5, 315.75, 305.75},
    {1024, 617.1, 615.6, 467.5, 456.0},
    {2048, 1195.1, 1178.7, 1195.1, 1178.7},
};

/// The calibrated entry a transfer of `size_bytes` is priced by: the
/// smallest one at least that large, and the largest entry for anything
/// past the end of the table. Rounding up rather than interpolating is what
/// the reference model's _dma() does, so an off-table size is priced the
/// same way on both sides instead of becoming a spurious difference.
inline const DmaEntry &lookupDmaEntry(int64_t size_bytes) {
  for (const auto &e : kDmaLatency)
    if (static_cast<int64_t>(e.bytes) >= size_bytes)
      return e;
  return kDmaLatency[std::size(kDmaLatency) - 1];
}

inline double lookupDmaLatency(bool is_load, int64_t size_bytes) {
  const DmaEntry &e = lookupDmaEntry(size_bytes);
  return is_load ? e.load : e.store;
}

inline double lookupDmaServiceInterval(bool is_load, int64_t size_bytes) {
  const DmaEntry &e = lookupDmaEntry(size_bytes);
  return is_load ? e.load_service : e.store_service;
}

} // namespace upmem_cm
