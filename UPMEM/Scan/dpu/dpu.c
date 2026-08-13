
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

// Ordinary WRAM global (no __mram/__host qualifier) -- shared by every
// tasklet on this DPU. Each tasklet writes only its own slot
// (partial_sums[tasklet_id]), so the writes themselves never race; the
// barrier below is what guarantees every tasklet's write is visible before
// any tasklet reads the other slots.
T partial_sums[NR_TASKLETS];

// Inclusive prefix sum (scan), ADD only, single DPU launch. Each tasklet
// reduces its own chunk of INPUT, publishes that sum into the shared
// partial_sums array, then (after a barrier) computes its own starting
// offset from every *other* tasklet's sum before scanning its own chunk.
//
// The offset loop below iterates all NR_TASKLETS slots unconditionally for
// every tasklet (not just the first tasklet_id of them) -- same trip count
// regardless of which tasklet runs it, only the add-or-skip branch outcome
// depends on tasklet_id. That keeps it simulatable despite depending on
// tasklet_id: a uniform, tasklet-independent instruction count, since
// taken/not-taken branches cost the same on this ISA (same property this
// project already leans on for Sel's predicate check and Gemv's
// boolean-replacement pattern).
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

  uint32_t arr_el_count = ITER_PER_THREAD * THREADS;
  uint32_t arr_el_size = arr_el_count * sizeof(T);

  uint32_t mram_base_addr_INPUT = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_OUTPUT =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + arr_el_size;

  uint32_t mram_temp_addr_INPUT =
      mram_base_addr_INPUT + (tasklet_id * ITER_PER_THREAD) * sizeof(T);
  uint32_t mram_temp_addr_OUTPUT =
      mram_base_addr_OUTPUT + (tasklet_id * ITER_PER_THREAD) * sizeof(T);

  uint32_t local_iter_count = ITER_PER_THREAD / BUFFER_COUNT;

  // Reduce: sum this tasklet's own chunk of INPUT (exactly ReduceOp's
  // kernel, single array).
  T result = 0;
  uint32_t addr = mram_temp_addr_INPUT;
  for (int r = 0; r < local_iter_count; r++) {
    mram_read((__mram_ptr void const *)(addr), cache_INPUT, BUFFER_SIZE);
    for (int c = 0; c < BUFFER_COUNT; c++) {
      result += cache_INPUT[c];
    }
    addr += BUFFER_SIZE;
  }
  partial_sums[tasklet_id] = result;

  // Every tasklet must see every other tasklet's partial_sums write before
  // computing its own offset below.
#ifndef NO_BARRIER
  barrier_wait(&my_barrier);
#endif

  T offset = 0;
  for (uint32_t t = 0; t < THREADS; t++) {
    if (t < tasklet_id) {
      offset += partial_sums[t];
    }
  }

  // Scan: this tasklet's own chunk, running sum starting from offset.
  T running = offset;
  for (int r = 0; r < local_iter_count; r++) {
    mram_read((__mram_ptr void const *)(mram_temp_addr_INPUT), cache_INPUT,
              BUFFER_SIZE);
    for (int c = 0; c < BUFFER_COUNT; c++) {
      running += cache_INPUT[c];
      cache_INPUT[c] = running;
    }
    mram_write(cache_INPUT, (__mram_ptr void *)(mram_temp_addr_OUTPUT),
               BUFFER_SIZE);
    mram_temp_addr_INPUT += BUFFER_SIZE;
    mram_temp_addr_OUTPUT += BUFFER_SIZE;
  }

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}
