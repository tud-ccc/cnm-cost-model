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

// E = A x B (NI x NJ), F = C x D (NJ x NL), G = E x F (NI x NL).
static T *A;
// B and D are stored pre-transposed (B_T: NJ rows of NK elements; D_T: NL
// rows of NM elements) so each matches exactly what dpu.c's matmul_pass
// expects for a "Y" operand -- contiguous per-column DMA reads instead of a
// strided access pattern (see dpu.c's comments for the full reasoning,
// including why C is deliberately left NOT transposed).
static T *B_T;
static T *C;
static T *D_T;
static T *host_G;
static T *device_G;

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
  input_args->ITERS[4] = MAP_ITER_DIMS[4];
  input_args->BUFFER_SIZES[0] = MAP_BUFFER_SIZES[0];
}

uint32_t getRowCount() { return MAP_THREAD_DIMS[0] * MAP_ITER_DIMS[0]; } // NI

uint32_t getKCount() { return MAP_ITER_DIMS[1]; } // NK

uint32_t getJCount() { return MAP_ITER_DIMS[2]; } // NJ

uint32_t getMCount() { return MAP_ITER_DIMS[3]; } // NM

uint32_t getLCount() { return MAP_THREAD_DIMS[0] * MAP_ITER_DIMS[4]; } // NL

uint32_t getAMatSize() { return getRowCount() * getKCount(); }

uint32_t getBMatSize() { return getJCount() * getKCount(); }

uint32_t getCMatSize() { return getJCount() * getMCount(); }

uint32_t getDMatSize() { return getLCount() * getMCount(); }

// E and F are MRAM scratch that never leave the device (see dpu.c) -- these
// are only their sizes, needed to compute G's offset in MRAM, which sits
// right after both.
uint32_t getEMatSize() { return getRowCount() * getJCount(); }

uint32_t getFMatSize() { return getLCount() * getJCount(); }

uint32_t getResSize() { return getRowCount() * getLCount(); }

// dpu.c assumes NK, NJ, NM and NL are each exact multiples of BUFFER_COUNT
// (no remainder-tile handling yet), so every row's write in any phase is
// naturally a whole number of BUFFER_SIZE-sized, 8-byte-aligned chunks -- no
// output padding needed here (unlike Gemv, which pads specifically for its
// leftover-row remainder batch).
//
// Mirrors dpu.c's three-phase structure exactly: E = A x B, F = C x D (kept
// in local buffers here, since both are just as internal to this reference
// computation as they are to the device), then G = E x F.
void kernel_on_host() {
  uint32_t rows = getRowCount();
  uint32_t k = getKCount();
  uint32_t j = getJCount();
  uint32_t m = getMCount();
  uint32_t l = getLCount();

  T *host_E = malloc(sizeof(T) * rows * j);
  T *host_F = malloc(sizeof(T) * j * l);

  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t jj = 0; jj < j; jj++) {
      T sum = 0;
      for (uint32_t c = 0; c < k; c++) {
        sum += A[i * k + c] * B_T[jj * k + c];
      }
      host_E[i * j + jj] = sum;
    }
  }

  for (uint32_t jj = 0; jj < j; jj++) {
    for (uint32_t ll = 0; ll < l; ll++) {
      T sum = 0;
      for (uint32_t c = 0; c < m; c++) {
        sum += C[jj * m + c] * D_T[ll * m + c];
      }
      host_F[jj * l + ll] = sum;
    }
  }

  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t ll = 0; ll < l; ll++) {
      T sum = 0;
      for (uint32_t jj = 0; jj < j; jj++) {
        sum += host_E[i * j + jj] * host_F[jj * l + ll];
      }
      host_G[i * l + ll] = sum;
    }
  }

  free(host_E);
  free(host_F);
}

bool checkOutput() {
  bool okay = true;
  for (uint32_t i = 0; i < getResSize(); i++) {
    if (host_G[i] != device_G[i]) {
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

      uint32_t mat_d_el_count_per_dpu = getDMatSize();
      uint32_t mat_d_el_size_per_dpu = mat_d_el_count_per_dpu * sizeof(T);

      uint32_t mat_e_el_size_per_dpu = getEMatSize() * sizeof(T);
      uint32_t mat_f_el_size_per_dpu = getFMatSize() * sizeof(T);

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
        DPU_ASSERT(dpu_prepare_xfer(dpu, C + mat_c_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                               DPU_MRAM_HEAP_POINTER_NAME,
                               mat_a_el_size_per_dpu + mat_b_el_size_per_dpu,
                               mat_c_el_size_per_dpu, DPU_XFER_DEFAULT));

      DPU_FOREACH(dpu_set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, D_T + mat_d_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(
          dpu_set, DPU_XFER_TO_DPU, DPU_MRAM_HEAP_POINTER_NAME,
          mat_a_el_size_per_dpu + mat_b_el_size_per_dpu + mat_c_el_size_per_dpu,
          mat_d_el_size_per_dpu, DPU_XFER_DEFAULT));
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
        DPU_ASSERT(dpu_prepare_xfer(dpu, device_G + res_el_count_per_dpu * i));
      }
      DPU_ASSERT(dpu_push_xfer(
          dpu_set, DPU_XFER_FROM_DPU, DPU_MRAM_HEAP_POINTER_NAME,
          mat_a_el_size_per_dpu + mat_b_el_size_per_dpu +
              mat_c_el_size_per_dpu + mat_d_el_size_per_dpu +
              mat_e_el_size_per_dpu + mat_f_el_size_per_dpu,
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
// convention); B, C, D are each pinned to a fixed constant (matching Gemv's
// B=1 convention) so a later cost-model trace of any phase's multiply stays
// data-independent/deterministic.
static void init_data() {
  A = malloc(sizeof(T) * getAMatSize());
  B_T = malloc(sizeof(T) * getBMatSize());
  C = malloc(sizeof(T) * getCMatSize());
  D_T = malloc(sizeof(T) * getDMatSize());
  host_G = malloc(sizeof(T) * getResSize());
  device_G = malloc(sizeof(T) * getResSize());
  srand(0);

  for (uint32_t r = 0; r < getAMatSize(); r++) {
    A[r] = r;
  }
  for (uint32_t r = 0; r < getBMatSize(); r++) {
    B_T[r] = INIT_DATA;
  }
  for (uint32_t r = 0; r < getCMatSize(); r++) {
    C[r] = INIT_DATA;
  }
  for (uint32_t r = 0; r < getDMatSize(); r++) {
    D_T[r] = INIT_DATA;
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
  free(C);
  free(D_T);
  free(host_G);
  free(device_G);

  return okay ? EXIT_SUCCESS : EXIT_FAILURE;
}
