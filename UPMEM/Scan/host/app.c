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

static T *A;
static T *host_OUTPUT;
static T *device_OUTPUT;

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

// The scan spans the whole array regardless of how dpu.c partitions the
// work across tasklets internally, so the reference is just the ordinary
// serial inclusive prefix sum over all elements.
void kernel_on_host() {
  uint32_t total = elementPerDpu();
  T running = 0;
  for (uint32_t i = 0; i < total; i++) {
    running += A[i];
    host_OUTPUT[i] = running;
  }
}

bool checkOutput() {
  bool okay = true;
  uint32_t total = elementPerDpu();
  for (uint32_t i = 0; i < total; i++) {
    if (host_OUTPUT[i] != device_OUTPUT[i]) {
      okay = false;
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
        DPU_ASSERT(dpu_prepare_xfer(dpu, A + arr_element_per_dpu * i));
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

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(
            dpu_prepare_xfer(dpu, device_OUTPUT + arr_element_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_FROM_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, arr_size_per_dpu,
                               arr_size_per_dpu, DPU_XFER_DEFAULT));

      if (rep >= (warmup)) {
        stop(&timer, 2);
      }

#ifdef ENABLE_PERFCOUNTER
      uint32_t nb_cycle_readback = 0;
      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_copy_from(dpu, "nb_cycle", 0, &nb_cycle_readback,
                                  sizeof(uint32_t)));
      }
      if (rep >= (warmup)) {
        fprintf(stderr, "PERFCOUNTER rep=%d value=%u\n", rep - warmup,
                nb_cycle_readback);
      }
#endif

      DPU_ASSERT(dpu_free(dpu_set));
    }
  }
  print(&timer, 1, repeat);
}

// Create input arrays. Value-dependent (A[r]=r), matching this project's
// usual convention.
static void init_data(int dim_size) {
  A = malloc(sizeof(T) * dim_size);
  host_OUTPUT = malloc(sizeof(T) * dim_size);
  device_OUTPUT = malloc(sizeof(T) * dim_size);
  srand(0);

  for (int r = 0; r < dim_size; r++) {
    A[r] = r;
  }
}

int main() {

  srand(time(NULL));
  init_data(elementPerDpu() * getDPUPerRankCount() * getTotalRankCount());
  kernel_on_host();
  kernel_on_device();
  bool okay = checkOutput();

  free(A);
  free(host_OUTPUT);
  free(device_OUTPUT);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
