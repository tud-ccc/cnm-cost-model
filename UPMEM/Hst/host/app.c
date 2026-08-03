#include <assert.h>
#include <dpu.h>
#include <dpu_log.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common_structs.h"
#include "config.h"
#include "gen_map.h"

#include "../support/timer.h"

// Define the DPU Binary path as DPU_BINARY here
#ifndef DPU_BINARY
#define DPU_BINARY "./bin/dpu"
#endif

static T *input;
static T *device_hst;
static T *host_hst;

uint32_t getTotalRankCount() {
  uint32_t res = 1;
  for (int i = 0; i < DIM_COUNT; i++) {
    res *= MAP_RANK_DIMS[i];
  }
  return res;
}

uint32_t getDPUPerRankCount() {
  uint32_t res = 1;
  for (int i = 0; i < DIM_COUNT; i++) {
    res *= MAP_DPU_DIMS[i];
  }
  return res;
}

uint32_t getThreadPerDPUCount() {
  uint32_t res = 1;
  for (int i = 0; i < DIM_COUNT; i++) {
    res *= MAP_THREAD_DIMS[i];
  }
  return res;
}

void copyInputArgs(dpu_arguments_t *input_args) {
  for (int i = 0; i < DIM_COUNT; i++) {
    input_args->THREADS[i] = MAP_THREAD_DIMS[i];
    input_args->ITERS[i] = MAP_ITER_DIMS[i];
    input_args->BUFFER_SIZES[i] = MAP_BUFFER_SIZES[i];
    input_args->BINS = BIN_COUNT;
  }
}

uint32_t elementPerDpu() {
  uint32_t res = 1;
  for (int i = 0; i < DIM_COUNT; i++) {
    res *= MAP_THREAD_DIMS[i];
    res *= MAP_ITER_DIMS[i];
  }
  return res;
}

uint32_t getIterPerThread() { return MAP_ITER_DIMS[0]; }

// Each DPU tasklet bins its own contiguous slice of input (ITER_PER_THREAD
// elements) into its own local BIN_COUNT-sized histogram, with no
// cross-tasklet merge happening on the DPU (dpu.c writes each tasklet's own
// histogram to its own slice of HST) -- so the host reference must do the
// same per-tasklet binning, matching dpu.c's (value*BIN_COUNT)>>DEPTH
// exactly.
void kernel_on_host() {
  uint32_t threads = getThreadPerDPUCount();
  uint32_t iter_per_thread = getIterPerThread();
  for (uint32_t t = 0; t < threads; t++) {
    uint32_t base = t * iter_per_thread;
    T *hst = host_hst + t * BIN_COUNT;
    for (uint32_t b = 0; b < BIN_COUNT; b++) {
      hst[b] = 0;
    }
    for (uint32_t i = 0; i < iter_per_thread; i++) {
      T d = (input[base + i] * BIN_COUNT) >> DEPTH;
      hst[d] += 1;
    }
  }
}

bool checkOutput() {
  bool okay = true;
  uint32_t threads = getThreadPerDPUCount();
  for (uint32_t t = 0; t < threads; t++) {
    for (uint32_t b = 0; b < BIN_COUNT; b++) {
      uint32_t idx = t * BIN_COUNT + b;
      if (host_hst[idx] != device_hst[idx]) {
        okay = false;
      }
    }
  }
  fprintf(stderr, "Correctness check: %s\n", okay ? "PASSED" : "FAILED");
  return okay;
}

void kernel_on_device() {
  int total_ranks = getTotalRankCount();
  int dpuPerRank = getDPUPerRankCount();

  int repeat = N_REP;
  int warmup = N_WARM_UP;

  Timer timer;
  for (int rep = 0; rep < (repeat + warmup); rep++) {
    for (int rank = 0; rank < total_ranks; rank++) {
      int allocated_dpus;
      struct dpu_set_t dpu_set, dpu;
      DPU_ASSERT(dpu_alloc(dpuPerRank, NULL, &dpu_set));
      DPU_ASSERT(dpu_load(dpu_set, DPU_BINARY, NULL));
      DPU_ASSERT(dpu_get_nr_dpus(dpu_set, &allocated_dpus));

      dpu_arguments_t *input_args =
          (dpu_arguments_t *)malloc(sizeof(dpu_arguments_t));
      copyInputArgs(input_args);

      int i = 0;
      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, input_args));
      }

      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU, "DPU_INPUT_ARGUMENTS",
                               0, sizeof(dpu_arguments_t), DPU_XFER_DEFAULT));

      uint32_t arr_element_per_dpu = elementPerDpu();
      uint32_t arr_size_per_dpu = arr_element_per_dpu * sizeof(T);

      if (rep >= (warmup)) {
        start(&timer, 0, rep - warmup);
      }

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, input + arr_element_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, 0, arr_size_per_dpu,
                               DPU_XFER_DEFAULT));
      DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));
      if (rep >= (warmup)) {
        stop(&timer, 0);
      }

      if (rep >= (warmup)) {
        start(&timer, 1, rep - warmup);
      }
      DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));
      if (rep >= (warmup)) {
        stop(&timer, 1);
      }

      if (rep >= (warmup)) {
        start(&timer, 2, rep - warmup);
      }

      int totalThreadPerDPU = getThreadPerDPUCount();
      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(
            dpu_prepare_xfer(dpu, device_hst + totalThreadPerDPU * BIN_COUNT * i));
      }

      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_FROM_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, arr_size_per_dpu,
                               totalThreadPerDPU * BIN_COUNT * sizeof(T),
                               DPU_XFER_DEFAULT));

      if (rep >= (warmup)) {
        stop(&timer, 2);
      }

      DPU_ASSERT(dpu_free(dpu_set));
    }
  }
  print(&timer, 1, repeat);
}

// Create input arrays. Value-dependent (not INIT_DATA's fixed constant) so
// the histogram actually spreads across bins instead of trivially landing
// everything in bin 0. (value*BIN_COUNT)>>DEPTH must stay < BIN_COUNT, i.e.
// value < 1 << DEPTH, so values are wrapped to that range.
//
// Values are additionally kept >= BIN_COUNT: `value * BIN_COUNT` lowers to
// a real __mulsi3 call (BIN_COUNT is a runtime dpu_arguments_t field here,
// not a compile-time constant the compiler could fold into a shift), and
// that routine's cost is data-dependent -- it swaps to put min(value,
// BIN_COUNT) in the bit-scanning operand and early-exits once that's
// exhausted, so its step count equals that operand's bit-length. Keeping
// value >= BIN_COUNT pins the scanning operand to the constant BIN_COUNT
// (256 = 2^8, a fixed 9-bit-length value) for every element, giving a
// deterministic instruction count -- the same "pin the data" trick Gemv
// uses with B=1.
static void init_data(int dim_size) {
  input = malloc(sizeof(T) * dim_size);

  uint32_t threads = getThreadPerDPUCount();
  host_hst = malloc(sizeof(T) * BIN_COUNT * threads);
  device_hst = malloc(sizeof(T) * BIN_COUNT * threads);

  srand(0);
  for (int r = 0; r < dim_size; r++) {
    input[r] = BIN_COUNT + (r % ((1 << DEPTH) - BIN_COUNT));
  }
}

int main() {

  srand(time(NULL));
  init_data(elementPerDpu() * getDPUPerRankCount() * getTotalRankCount());
  kernel_on_host();
  kernel_on_device();
  bool okay = checkOutput();

  free(input);
  free(host_hst);
  free(device_hst);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
