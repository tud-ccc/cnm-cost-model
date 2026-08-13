
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
#include "../support/pred.h"

__host dpu_arguments_t DPU_INPUT_ARGUMENTS;
__host uint32_t nb_cycle;
#ifdef COMPUTE_ONLY
// Written to (not read) purely so -O3 can't prove cache_OUTPUT/cache_COUNT
// are dead once mram_write is skipped below -- keeps the per-element
// pack/count instruction profile identical to the real kernel.
__host uint32_t sink;
#endif

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

  T *cache_INPUT = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_OUTPUT = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_COUNT = (T *)mem_alloc(4 * sizeof(T));

  uint32_t arr_el_count = ITER_PER_THREAD * THREADS;
  uint32_t arr_el_size = arr_el_count * sizeof(T);

  uint32_t mram_base_addr_INPUT = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_OUTPUT =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + arr_el_size;
  uint32_t mram_base_addr_COUNT =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + 2 * arr_el_size;

  uint32_t local_iter_count = ITER_PER_THREAD / BUFFER_COUNT;

  uint32_t mram_temp_addr_INPUT =
      mram_base_addr_INPUT + (tasklet_id * ITER_PER_THREAD) * sizeof(T);
  uint32_t mram_temp_addr_OUTPUT =
      mram_base_addr_OUTPUT + (tasklet_id * ITER_PER_THREAD) * sizeof(T);
  // Each chunk gets its own 4-element count slot, so this tasklet's count
  // region is local_iter_count slots wide, not ITER_PER_THREAD-wide.
  uint32_t mram_temp_addr_COUNT = mram_base_addr_COUNT +
                                  (tasklet_id * local_iter_count) * 4 *
                                      sizeof(T);

  for (int r = 0; r < local_iter_count; r++) {
#ifdef COMPUTE_ONLY
    // Timing-only variant used to isolate real DMA-attributable cost: skip
    // the mram_read entirely (leaving cache_INPUT's stale WRAM contents from
    // mem_alloc/the previous chunk) rather than substituting a synthetic
    // fill loop, which would itself add uncounted compute cost and defeat
    // the whole point of a clean subtraction.
#else
    mram_read((__mram_ptr void const *)(mram_temp_addr_INPUT), cache_INPUT,
              BUFFER_SIZE);
#endif

    // Pack elements satisfying pred() densely at the front of cache_OUTPUT;
    // cache_COUNT[0] records how many of this chunk's BUFFER_COUNT elements
    // were selected (only that many entries of cache_OUTPUT are valid).
    uint32_t count = 0;
    for (int c = 0; c < BUFFER_COUNT; c++) {
      T value = cache_INPUT[c];
      if (pred(value)) {
        cache_OUTPUT[count] = value;
        count++;
      }
    }
    cache_COUNT[0] = count;

#ifdef COMPUTE_ONLY
    sink ^= cache_OUTPUT[0] ^ cache_COUNT[0];
#else
    mram_write(cache_OUTPUT, (__mram_ptr void *)(mram_temp_addr_OUTPUT),
               BUFFER_SIZE);
    mram_write(cache_COUNT, (__mram_ptr void *)(mram_temp_addr_COUNT),
               4 * sizeof(T));
#endif

    mram_temp_addr_INPUT += BUFFER_SIZE;
    mram_temp_addr_OUTPUT += BUFFER_SIZE;
    mram_temp_addr_COUNT += 4 * sizeof(T);
  }

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}