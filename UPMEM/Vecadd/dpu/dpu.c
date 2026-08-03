
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
  }

  barrier_wait(&my_barrier);
  uint32_t ITER_PER_THREAD = DPU_INPUT_ARGUMENTS.ITERS[0];
  uint32_t THREADS = DPU_INPUT_ARGUMENTS.THREADS[0];

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(BUFFER_SIZE);

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
  uint32_t mram_temp_addr_C =
      mram_base_addr_C + (tasklet_id * ITER_PER_THREAD) * sizeof(T);

  uint32_t local_iter_count = ITER_PER_THREAD / BUFFER_COUNT;
  for (int r = 0; r < local_iter_count; r++) {
    mram_read((__mram_ptr void const *)(mram_temp_addr_A), cache_A,
              BUFFER_SIZE);

    mram_read((__mram_ptr void const *)(mram_temp_addr_B), cache_B,
              BUFFER_SIZE);
    for (int c = 0; c < BUFFER_COUNT; c++) {
      cache_C[c] = cache_A[c] + cache_B[c];
    }
    mram_write(cache_C, (__mram_ptr void *)(mram_temp_addr_C), BUFFER_SIZE);
    mram_temp_addr_A += BUFFER_SIZE;
    mram_temp_addr_B += BUFFER_SIZE;
    mram_temp_addr_C += BUFFER_SIZE;
  }


  return 0;
}
