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
// B is stored pre-transposed (N rows of K elements, row n = column n of the
// logical K x N matrix) so it matches exactly what dpu.c expects on the
// device -- see dpu.c's comment for why (contiguous per-column DMA reads).
static T *B_T;
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
  input_args->ITERS[2] = MAP_ITER_DIMS[2];
  input_args->BUFFER_SIZES[0] = MAP_BUFFER_SIZES[0];
}

uint32_t getRowCount() { return MAP_THREAD_DIMS[0] * MAP_ITER_DIMS[0]; }

uint32_t getKCount() { return MAP_ITER_DIMS[1]; }

uint32_t getNCount() { return MAP_ITER_DIMS[2]; }

uint32_t getAMatSize() { return getRowCount() * getKCount(); }

uint32_t getBMatSize() { return getNCount() * getKCount(); }

uint32_t getResSize() { return getRowCount() * getNCount(); }

// dpu.c assumes K and N are each exact multiples of BUFFER_COUNT (no
// remainder-tile handling yet), so every row's C write is naturally a whole
// number of BUFFER_SIZE-sized, 8-byte-aligned chunks -- no output padding
// needed here (unlike Gemv, which pads specifically for its leftover-row
// remainder batch).
void kernel_on_host() {
  uint32_t rows = getRowCount();
  uint32_t k = getKCount();
  uint32_t n = getNCount();
  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < n; j++) {
      T sum = 0;
      for (uint32_t c = 0; c < k; c++) {
        sum += A[i * k + c] * B_T[j * k + c];
      }
      host_C[i * n + j] = sum;
    }
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

      uint32_t mat_a_el_count_per_dpu = getAMatSize();
      uint32_t mat_a_el_size_per_dpu = mat_a_el_count_per_dpu * sizeof(T);

      uint32_t mat_b_el_count_per_dpu = getBMatSize();
      uint32_t mat_b_el_size_per_dpu = mat_b_el_count_per_dpu * sizeof(T);

      if (rep >= (warmup)) {
        start(&timer, 0, rep - warmup);
      }

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, A + mat_a_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME, 0,
                               mat_a_el_size_per_dpu, DPU_XFER_DEFAULT));

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, B_T + mat_b_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(
          dpu_set, DPU_XFER_TO_DPU, DPU_MRAM_HEAP_POINTER_NAME,
          mat_a_el_size_per_dpu, mat_b_el_size_per_dpu, DPU_XFER_DEFAULT));
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

      uint32_t res_el_count_per_dpu = getResSize();

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, device_C + res_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(
          dpu_set, DPU_XFER_FROM_DPU, DPU_MRAM_HEAP_POINTER_NAME,
          mat_a_el_size_per_dpu + mat_b_el_size_per_dpu,
          res_el_count_per_dpu * sizeof(T), DPU_XFER_DEFAULT));

      if (rep >= (warmup)) {
        stop(&timer, 2);
      }

      DPU_ASSERT(dpu_free(dpu_set));
    }
  }
  print(&timer, 1, repeat);
}

// Create input arrays. A is value-dependent (A[r]=r, matching Gemv's
// convention); B is pinned to a fixed constant (matching Gemv's B=1
// convention) so a later cost-model trace of the A*B multiply stays
// data-independent/deterministic.
static void init_data() {
  A = malloc(sizeof(T) * getAMatSize());
  B_T = malloc(sizeof(T) * getBMatSize());
  host_C = malloc(sizeof(T) * getResSize());
  device_C = malloc(sizeof(T) * getResSize());
  srand(0);

  for (uint32_t r = 0; r < getAMatSize(); r++) {
    A[r] = r;
  }
  for (uint32_t r = 0; r < getBMatSize(); r++) {
    B_T[r] = INIT_DATA;
  }
}

int main() {

  srand(time(NULL));
  init_data();
  kernel_on_host();
  kernel_on_device();
  bool okay = checkOutput();

  free(A);
  free(B_T);
  free(host_C);
  free(device_C);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
