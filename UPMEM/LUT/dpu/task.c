#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>
#include <perfcounter.h>
#include <seqread.h>
#include <stdint.h>

#include "../generated_headers/config.h"
#include "../support/common.h"

__host uint32_t nb_cycle;

__host dpu_arguments_t DPU_INPUT_ARGUMENTS;

int main() {
  uint32_t ITER = DPU_INPUT_ARGUMENTS.ITER;
  uint32_t custom_range = DPU_INPUT_ARGUMENTS.custom_range;
  uint32_t buffer_size = DPU_INPUT_ARGUMENTS.buffer_size;
  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t arr_size = DPU_INPUT_ARGUMENTS.arr_size;
  uint32_t mram_base_addr_B =
      (uint32_t)(DPU_MRAM_HEAP_POINTER + arr_size * sizeof(T));
  T *cache_A = (T *)mem_alloc(buffer_size * sizeof(uint64_t));
  T *cache_B = (T *)mem_alloc(buffer_size * sizeof(uint64_t));
  T *cache_C = (T *)mem_alloc(buffer_size * sizeof(uint64_t));

  uint32_t a_offset = mram_base_addr_A;
  uint32_t b_offset = mram_base_addr_B;
#ifdef PIPELINE
  for (uint32_t i = 0; i < ITER; i++) {
    mram_read((__mram_ptr void const *)a_offset, cache_A,
              buffer_size * sizeof(uint64_t));
    mram_read((__mram_ptr void const *)b_offset, cache_B,
              buffer_size * sizeof(uint64_t));
    for (uint32_t j = 0; j < buffer_size; j++) {
#ifdef ADD
      cache_C[j] = cache_A[j] + cache_B[j];
#elif SUB
      cache_C[j] = cache_A[j] - cache_B[j];
#elif DIV
      cache_C[j] = cache_A[j] / cache_B[j];
#elif MUL
      cache_C[j] = cache_A[j] * cache_B[j];
#elif LOAD
      cache_C[j] = cache_A[j];
#elif STORE
      cache_C[j] = cache_A[j];
#endif
    }
  }
#elif DMA
  // Each tasklet streams through its own, non-overlapping region of A,
  // advancing to a new address on every DMA call (wrapping within the
  // region), instead of every tasklet repeatedly hitting the same address.
  unsigned int tasklet_id = me();
  uint32_t region_size = (arr_size * sizeof(T)) / NR_TASKLETS;
  uint32_t num_slots = region_size / buffer_size;
  uint32_t tasklet_base_A = mram_base_addr_A + tasklet_id * region_size;
  uint32_t slot = 0;
  // Volatile sink: forces the compiler to keep computing/advancing the
  // address and running the outer loop even when DMA_COUNT == 0, so the
  // "without DMA" baseline still reflects the real loop+address overhead
  // instead of being optimized away entirely.
  volatile uint32_t sink = 0;
#ifdef DMA_LOAD
  for (int x = 0; x < ITER; x++) {
    uint32_t addr = tasklet_base_A + slot * buffer_size;
    sink = addr;
    for (uint32_t d = 0; d < DMA_COUNT; d++) {
      mram_read((__mram_ptr void const *)(addr), cache_A, buffer_size);
    }
    slot++;
    if (slot >= num_slots) {
      slot = 0;
    }
  }
#elif DMA_STORE
  for (int x = 0; x < ITER; x++) {
    uint32_t addr = tasklet_base_A + slot * buffer_size;
    sink = addr;
    for (uint32_t d = 0; d < DMA_COUNT; d++) {
      mram_write(cache_A, (__mram_ptr void *)(addr), buffer_size);
    }
    slot++;
    if (slot >= num_slots) {
      slot = 0;
    }
  }
#endif
#endif
  return 0;
}