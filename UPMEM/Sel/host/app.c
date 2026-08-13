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
#include "../support/pred.h"

#include "../support/timer.h"

// Define the DPU Binary path as DPU_BINARY here
#ifndef DPU_BINARY
#define DPU_BINARY "./bin/dpu"
#endif

static T *A;
static T *host_output;
static T *device_output;
static T *host_count;
static T *device_count;

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

// Matches dpu.c's local_iter_count: one 4-element count slot per chunk,
// per tasklet.
uint32_t getChunksPerThread() { return MAP_ITER_DIMS[0] / MAP_BUFFER_SIZES[0]; }

// Host reference: for each tasklet's chunk, pack elements satisfying
// pred() (shared with dpu.c via support/pred.h, so this can never diverge
// from what the device actually filters on) densely at the front of that
// chunk's slice of host_output, and record how many passed.
void kernel_on_host() {
  uint32_t threads = getThreadPerDPUCount();
  uint32_t iter_per_thread = MAP_ITER_DIMS[0];
  uint32_t buffer_count = MAP_BUFFER_SIZES[0];
  uint32_t chunks_per_thread = getChunksPerThread();

  for (uint32_t t = 0; t < threads; t++) {
    uint32_t thread_base = t * iter_per_thread;
    for (uint32_t ch = 0; ch < chunks_per_thread; ch++) {
      uint32_t chunk_base = thread_base + ch * buffer_count;
      uint32_t count = 0;
      for (uint32_t c = 0; c < buffer_count; c++) {
        T value = A[chunk_base + c];
        if (pred(value)) {
          host_output[chunk_base + count] = value;
          count++;
        }
      }
      host_count[t * chunks_per_thread + ch] = count;
    }
  }
}

bool checkOutput() {
  bool okay = true;
  uint32_t threads = getThreadPerDPUCount();
  uint32_t iter_per_thread = MAP_ITER_DIMS[0];
  uint32_t buffer_count = MAP_BUFFER_SIZES[0];
  uint32_t chunks_per_thread = getChunksPerThread();

  for (uint32_t t = 0; t < threads; t++) {
    uint32_t thread_base = t * iter_per_thread;
    for (uint32_t ch = 0; ch < chunks_per_thread; ch++) {
      uint32_t chunk_base = thread_base + ch * buffer_count;
      uint32_t count_idx = t * chunks_per_thread + ch;
      uint32_t expected_count = host_count[count_idx];
      // device_count was written with 4 elements per chunk (MRAM alignment
      // padding); only slot 0 is meaningful.
      uint32_t actual_count = device_count[count_idx * 4];
      if (actual_count != expected_count) {
        okay = false;
        continue;
      }
      for (uint32_t i = 0; i < expected_count; i++) {
        if (host_output[chunk_base + i] != device_output[chunk_base + i]) {
          okay = false;
        }
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

  uint32_t chunks_per_thread = getChunksPerThread();
  uint32_t count_element_per_dpu =
      getThreadPerDPUCount() * chunks_per_thread * 4;

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
        DPU_ASSERT(dpu_prepare_xfer(dpu, device_output + arr_element_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_FROM_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, arr_size_per_dpu,
                               arr_size_per_dpu, DPU_XFER_DEFAULT));

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(
            dpu_prepare_xfer(dpu, device_count + count_element_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_FROM_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME,
                               2 * arr_size_per_dpu,
                               count_element_per_dpu * sizeof(T),
                               DPU_XFER_DEFAULT));

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

// Create input arrays. Value-dependent (not a fixed constant) so the
// selection actually splits the data instead of trivially selecting
// everything or nothing.
static void init_data(int dim_size) {
  A = malloc(sizeof(T) * dim_size);
  host_output = malloc(sizeof(T) * dim_size);
  device_output = malloc(sizeof(T) * dim_size);

  uint32_t count_elements = getThreadPerDPUCount() * getChunksPerThread() * 4;
  host_count = malloc(sizeof(T) * getThreadPerDPUCount() * getChunksPerThread());
  device_count = malloc(sizeof(T) * count_elements);

  srand(0);
  for (int r = 0; r < dim_size; r++) {
    A[r] = r;
  }
}

int main() {
  int dim_size = elementPerDpu() * getDPUPerRankCount() * getTotalRankCount();

  srand(time(NULL));
  init_data(dim_size);
  kernel_on_host();
  kernel_on_device();

  bool okay = checkOutput();

  free(A);
  free(host_output);
  free(device_output);
  free(host_count);
  free(device_count);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
