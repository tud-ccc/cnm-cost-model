
#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>
#include <perfcounter.h>
#include <seqread.h>
#include <stdint.h>
#include <stdio.h>

#include "common_structs.h"
#include "config.h"
#include "gen_map.h"

__host dpu_arguments_t DPU_INPUT_ARGUMENTS;
__host uint32_t nb_cycle;

// Barrier
BARRIER_INIT(my_barrier, NR_TASKLETS);

// main
int main() {
  unsigned int tasklet_id = me();
  if (tasklet_id == 0) { // Initialize once the cycle counter
    mem_reset();         // Reset the heap
#ifdef SIM
#if SIM == 1
    perfcounter_config(COUNT_CYCLES, true);
#endif
#endif
#ifdef ENABLE_PERFCOUNTER
#ifdef PERF_COUNT_INSTRUCTIONS
    perfcounter_config(COUNT_INSTRUCTIONS, true);
#else
    perfcounter_config(COUNT_CYCLES, true);
#endif
#endif
  }
  barrier_wait(&my_barrier);

  uint32_t ITER_PER_THREAD = DPU_INPUT_ARGUMENTS.ITERS[0];
  uint32_t THREADS = DPU_INPUT_ARGUMENTS.THREADS[0];
  if (tasklet_id >= THREADS)
    return 0;

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(4 * sizeof(T));

  uint32_t arr_el_count = ITER_PER_THREAD * THREADS;
  uint32_t arr_el_size = arr_el_count * sizeof(T);

  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_B = (uint32_t)(DPU_MRAM_HEAP_POINTER) + arr_el_size;
  uint32_t mram_base_addr_C =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + 2 * arr_el_size;

  uint32_t mram_temp_addr_A =
      mram_base_addr_A + (tasklet_id * ITER_PER_THREAD) * sizeof(T);
  uint32_t mram_temp_addr_B =
      mram_base_addr_B + (tasklet_id * ITER_PER_THREAD) * sizeof(T);
  uint32_t mram_temp_addr_C = mram_base_addr_C + (tasklet_id * 4) * sizeof(T);

  uint32_t local_iter_count = ITER_PER_THREAD / BUFFER_COUNT;
#ifdef ADD
  T result = 0;
#elif MAC
  T result = 0;
#elif MUL
  T result = 1;
#endif
  for (int r = 0; r < local_iter_count; r++) {
    mram_read((__mram_ptr void const *)(mram_temp_addr_A), cache_A,
              BUFFER_SIZE);

    mram_read((__mram_ptr void const *)(mram_temp_addr_B), cache_B,
              BUFFER_SIZE);
    for (int c = 0; c < BUFFER_COUNT; c++) {
#ifdef ADD
      result += cache_A[c] + cache_B[c];
#elif MAC
      result += cache_A[c] * cache_B[c];
#elif MUL
      result *= cache_A[c] * cache_B[c];
#endif
    }
    mram_temp_addr_A += BUFFER_SIZE;
    mram_temp_addr_B += BUFFER_SIZE;
  }
  cache_C[0] = result;
  mram_write(cache_C, (__mram_ptr void *)(mram_temp_addr_C), 4 * sizeof(T));

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}