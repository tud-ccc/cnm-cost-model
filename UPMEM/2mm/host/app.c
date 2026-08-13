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

#include "../generated_headers/common_structs.h"
#include "../generated_headers/config.h"
#include "../generated_headers/gen_map.h"

#include "../support/timer.h"

// Define the DPU Binary path as DPU_BINARY here
#ifndef DPU_BINARY
#define DPU_BINARY "./bin/dpu"
#endif

static T *A;
// B and C are stored pre-transposed (B: N rows of K elements, row n =
// column n of the logical K x N matrix; C: P rows of N elements, row p =
// column p of the logical N x P matrix) so each matches exactly what
// dpu.c's matmul_pass expects on the device -- see dpu.c's comment for why
// (contiguous per-column DMA reads instead of a strided access pattern).
static T *B_T;
static T *C_T;
static T *host_D;
static T *device_D;

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
  input_args->ITERS[3] = MAP_ITER_DIMS[3];
  input_args->BUFFER_SIZES[0] = MAP_BUFFER_SIZES[0];
}

uint32_t getRowCount() { return MAP_THREAD_DIMS[0] * MAP_ITER_DIMS[0]; }

uint32_t getKCount() { return MAP_ITER_DIMS[1]; }

uint32_t getNCount() { return MAP_ITER_DIMS[2]; }

uint32_t getPCount() { return MAP_ITER_DIMS[3]; }

uint32_t getAMatSize() { return getRowCount() * getKCount(); }

uint32_t getBMatSize() { return getNCount() * getKCount(); }

uint32_t getCMatSize() { return getPCount() * getNCount(); }

// E is a scratch intermediate that never leaves the device (see dpu.c) --
// this is only its size, needed to compute D's offset in MRAM, which sits
// right after E.
uint32_t getEMatSize() { return getRowCount() * getNCount(); }

uint32_t getResSize() { return getRowCount() * getPCount(); }

// dpu.c assumes K, N and P are each exact multiples of BUFFER_COUNT (no
// remainder-tile handling yet), so every row's write in either phase is
// naturally a whole number of BUFFER_SIZE-sized, 8-byte-aligned chunks -- no
// output padding needed here (unlike Gemv, which pads specifically for its
// leftover-row remainder batch).
//
// Mirrors dpu.c's two-phase structure exactly: E = A x B (kept in a local
// buffer here, since it's just as internal to this reference computation as
// it is to the device), then D = E x C.
void kernel_on_host() {
  uint32_t rows = getRowCount();
  uint32_t k = getKCount();
  uint32_t n = getNCount();
  uint32_t p = getPCount();

  T *host_E = malloc(sizeof(T) * rows * n);

  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < n; j++) {
      T sum = 0;
      for (uint32_t c = 0; c < k; c++) {
        sum += A[i * k + c] * B_T[j * k + c];
      }
      host_E[i * n + j] = sum;
    }
  }

  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < p; j++) {
      T sum = 0;
      for (uint32_t c = 0; c < n; c++) {
        sum += host_E[i * n + c] * C_T[j * n + c];
      }
      host_D[i * p + j] = sum;
    }
  }

  free(host_E);
}

bool checkOutput() {
  bool okay = true;
  for (uint32_t i = 0; i < getResSize(); i++) {
    if (host_D[i] != device_D[i]) {
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

      uint32_t mat_c_el_count_per_dpu = getCMatSize();
      uint32_t mat_c_el_size_per_dpu = mat_c_el_count_per_dpu * sizeof(T);

      uint32_t mat_e_el_size_per_dpu = getEMatSize() * sizeof(T);

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

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, C_T + mat_c_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME,
                               mat_a_el_size_per_dpu + mat_b_el_size_per_dpu,
                               mat_c_el_size_per_dpu, DPU_XFER_DEFAULT));
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
        DPU_ASSERT(dpu_prepare_xfer(dpu, device_D + res_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(
          dpu_set, DPU_XFER_FROM_DPU, DPU_MRAM_HEAP_POINTER_NAME,
          mat_a_el_size_per_dpu + mat_b_el_size_per_dpu +
              mat_c_el_size_per_dpu + mat_e_el_size_per_dpu,
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
// convention); B and C are each pinned to a fixed constant (matching Gemv's
// B=1 convention) so a later cost-model trace of either phase's multiply
// stays data-independent/deterministic.
static void init_data() {
  A = malloc(sizeof(T) * getAMatSize());
  B_T = malloc(sizeof(T) * getBMatSize());
  C_T = malloc(sizeof(T) * getCMatSize());
  host_D = malloc(sizeof(T) * getResSize());
  device_D = malloc(sizeof(T) * getResSize());
  srand(0);

  for (uint32_t r = 0; r < getAMatSize(); r++) {
    A[r] = r;
  }
  for (uint32_t r = 0; r < getBMatSize(); r++) {
    B_T[r] = INIT_DATA;
  }
  for (uint32_t r = 0; r < getCMatSize(); r++) {
    C_T[r] = INIT_DATA;
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
  free(C_T);
  free(host_D);
  free(device_D);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
