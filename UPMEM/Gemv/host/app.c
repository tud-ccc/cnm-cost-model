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
static T *B;
static T *host_C;
static T *device_C;

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

uint32_t getThreadPerDPUCount() { return MAP_THREAD_DIMS[0]; }

void copyInputArgs(dpu_arguments_t *input_args) {
  input_args->THREADS[0] = MAP_THREAD_DIMS[0];
  input_args->ITERS[0] = MAP_ITER_DIMS[0];
  input_args->ITERS[1] = MAP_ITER_DIMS[1];
  input_args->BUFFER_SIZES[0] = MAP_BUFFER_SIZES[0];
}

uint32_t getRowCount() { return MAP_THREAD_DIMS[0] * MAP_ITER_DIMS[0]; }

uint32_t getColCount() { return MAP_ITER_DIMS[1]; }

uint32_t getAMatSize() { return getRowCount() * getColCount(); }

uint32_t getVecSize() { return getColCount(); }

uint32_t getResSize() { return getRowCount(); }

// Must match dpu.c's padded_iters_row: each tasklet's C region in MRAM is
// strided by this many elements (rounded up to even for 8-byte MRAM
// alignment), not by MAP_ITER_DIMS[0] itself.
uint32_t getPaddedRowsPerTasklet() {
  uint32_t rows_per_tasklet = MAP_ITER_DIMS[0];
  return rows_per_tasklet + (rows_per_tasklet % 2);
}

void kernel_on_host() {
  uint32_t rows = getResSize();
  uint32_t cols = getVecSize();
  for (uint32_t i = 0; i < rows; i++) {
    host_C[i] = 0;
    for (uint32_t c = 0; c < cols; c++) {
      host_C[i] += A[i * cols + c] * B[c];
    }
  }
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

      uint32_t mat_el_count_per_dpu = getAMatSize();
      uint32_t mat_el_size_per_dpu = mat_el_count_per_dpu * sizeof(T);

      if (rep >= (warmup)) {
        start(&timer, 0, rep - warmup);
      }

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, A + mat_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, 0,
                               mat_el_size_per_dpu, DPU_XFER_DEFAULT));

      uint32_t vec_el_count_per_dpu = getVecSize();
      uint32_t vec_el_size_per_dpu = vec_el_count_per_dpu * sizeof(T);

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, B + vec_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, mat_el_size_per_dpu,
                               vec_el_size_per_dpu, DPU_XFER_DEFAULT));
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
      uint32_t rows_per_tasklet = MAP_ITER_DIMS[0];
      uint32_t padded_rows_per_tasklet = getPaddedRowsPerTasklet();
      uint32_t padded_res_count_per_dpu =
          totalThreadPerDPU * padded_rows_per_tasklet;
      T *padded_C = malloc(sizeof(T) * padded_res_count_per_dpu);

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, padded_C));
      }

      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_FROM_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME,
                               vec_el_size_per_dpu + mat_el_size_per_dpu,
                               padded_res_count_per_dpu * sizeof(T),
                               DPU_XFER_DEFAULT));

      if (rep >= (warmup)) {
        stop(&timer, 2);
      }

      // Each tasklet's region in the padded readback is
      // padded_rows_per_tasklet elements wide, but only the first
      // rows_per_tasklet of those are real results -- strip the padding.
      for (uint32_t t = 0; t < (uint32_t)totalThreadPerDPU; t++) {
        memcpy(device_C + t * rows_per_tasklet,
               padded_C + t * padded_rows_per_tasklet,
               rows_per_tasklet * sizeof(T));
      }
      free(padded_C);

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

      // #ifdef LOG
      // #if LOG == 1
      unsigned int each_dpu = 0;
      // DPU_FOREACH(dpu_set, dpu) {
      //   printf("DPU#%d:\n", each_dpu);
      //   DPU_ASSERT(dpulog_read_for_dpu(dpu.dpu, stdout));
      //   each_dpu++;
      // }
      // #endif
      // #endif

      //       DPU_ASSERT(dpu_free(dpu_set));
    }
  }
  print(&timer, 1, repeat);
}

// Create input arrays
static void init_data() {
  A = malloc(sizeof(T) * getAMatSize());
  B = malloc(sizeof(T) * getVecSize());
  host_C = malloc(sizeof(T) * getResSize());
  device_C = malloc(sizeof(T) * getResSize());
  srand(0);

  for (int r = 0; r < getAMatSize(); r++) {
    A[r] = r;
  }
  for (int r = 0; r < getVecSize(); r++) {
    B[r] = INIT_DATA;
  }
}

bool checkOutput() {
  bool okay = true;
  for (uint32_t i = 0; i < getResSize(); i++) {
    if (host_C[i] != device_C[i]) {
      okay = false;
    }
  }
  fprintf(stderr, "Correctness check: %s\n", okay ? "PASSED" : "FAILED");
  return okay;
}

int main() {

  srand(time(NULL));
  init_data();
  kernel_on_host();
  kernel_on_device();
  bool okay = checkOutput();
  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
