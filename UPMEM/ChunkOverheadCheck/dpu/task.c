#include <alloc.h>
#include <defs.h>
#include <mram.h>
#include <perfcounter.h>
#include <stdint.h>

#include "../generated_headers/config.h"

__host uint32_t nb_cycle_with;
__host uint32_t nb_cycle_without;

// Tests whether two back-to-back DMA reads from the same thread (the real
// chunk_loop's A-read then B-read, with only one address-calc instruction
// between them) cost less in aggregate on real hardware than the model's
// two independent latency charges. DUAL=1 does both reads per iteration
// (matching chunk_loop); DUAL=0 does only the first (a single-DMA
// baseline), everything else identical, so a diff isolates the marginal
// cost of adding the second, closely-spaced read. Addresses stream forward
// each iteration (same per-tasklet-region + slot-advance pattern as the
// official DMA calibration in LUT/), not a fixed repeated address, to avoid
// any same-address caching effect skewing the comparison.
int main() {
  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  uint32_t mram_base_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_B = mram_base_A + ARR_SIZE;
  uint32_t num_slots = ARR_SIZE / BUFFER_SIZE;
  uint32_t slot = 0;

  perfcounter_config(COUNT_CYCLES, true);
  uint32_t start = perfcounter_get();
  for (uint32_t i = 0; i < ITER; i++) {
    uint32_t addr_a = mram_base_A + slot * BUFFER_SIZE;
    uint32_t addr_b = mram_base_B + slot * BUFFER_SIZE;
    mram_read((__mram_ptr void const *)(addr_a), cache_A, BUFFER_SIZE);
#if DUAL
    mram_read((__mram_ptr void const *)(addr_b), cache_B, BUFFER_SIZE);
#endif
    slot++;
    if (slot >= num_slots) {
      slot = 0;
    }
  }
  uint32_t end = perfcounter_get();

#if DUAL
  nb_cycle_with = end - start;
#else
  nb_cycle_without = end - start;
#endif

  return 0;
}
