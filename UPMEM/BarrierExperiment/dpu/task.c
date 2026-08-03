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

BARRIER_INIT(my_barrier, NR_TASKLETS);

// Calibrates barrier_wait()'s real cost the same way DmaScalingExperiment
// calibrates a DMA call: one outer ITER loop, a DO_BARRIER-gated block of
// BARRIER_COUNT back-to-back barrier_wait() calls (0 vs N, differenced by
// the run_script the same way DMA_COUNT was), with realistic "before" and
// "after" work around it instead of an idle/degenerate loop -- a small
// vecadd over a WRAM buffer, representative of what a real kernel (Scan,
// 2mm/3mm) actually does around a barrier, and incidentally a source of
// the same kind of small per-tasklet timing jitter real kernels would have
// arriving at the barrier.
int main() {
  uint32_t ITER = DPU_INPUT_ARGUMENTS.ITER;

  T cache_A[VEC_SIZE];
  T cache_B[VEC_SIZE];
  T cache_C[VEC_SIZE];
  for (uint32_t i = 0; i < VEC_SIZE; i++) {
    cache_A[i] = i;
    cache_B[i] = 1;
  }

  // Forces the compiler to keep the before/after loops (and the barrier
  // block, when DO_BARRIER is off) instead of proving them dead, the same
  // role `sink` plays in DmaScalingExperiment's task.c.
  volatile T sink = 0;

  for (int x = 0; x < ITER; x++) {
    // "before": a small vecadd, same shape as VectorOp's real inner loop.
    for (uint32_t i = 0; i < VEC_SIZE; i++) {
      cache_C[i] = cache_A[i] + cache_B[i];
    }
    sink = cache_C[0];

#ifdef DO_BARRIER
    for (uint32_t b = 0; b < BARRIER_COUNT; b++) {
      barrier_wait(&my_barrier);
    }
#endif

    // "after": the same vecadd again.
    for (uint32_t i = 0; i < VEC_SIZE; i++) {
      cache_C[i] = cache_A[i] + cache_B[i];
    }
    sink = cache_C[0];
  }

  return 0;
}
