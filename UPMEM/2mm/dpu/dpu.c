
#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>
#include <perfcounter.h>
#include <seqread.h>
#include <stdint.h>
#include <stdio.h>

#include "../generated_headers/common_structs.h"
#include "../generated_headers/config.h"
#include "../generated_headers/gen_map.h"

__host dpu_arguments_t DPU_INPUT_ARGUMENTS;
__host uint32_t nb_cycle;

// Barrier
BARRIER_INIT(my_barrier, NR_TASKLETS);

// One tasklet's row-tiled matmul: Z[row, :] = X[row, :] . Y for each row in
// its block, where Y is stored transposed (N_dim rows of K_dim elements, so
// each is one contiguous column of the logical K_dim x N_dim matrix). Used
// twice below: once for E = A x B, once for D = E x C -- "2mm" is exactly
// two back-to-back instances of this same tiled matmul, with the first's
// output (E) becoming the second's input, entirely inside the DPU (E is
// never transferred back to the host).
static void matmul_pass(T *cache_A, T *cache_B, T *cache_C,
                         uint32_t mram_row0_addr_X, uint32_t mram_base_addr_Y,
                         uint32_t mram_row0_addr_Z, uint32_t ROWS,
                         uint32_t K_dim, uint32_t N_dim,
                         uint32_t BUFFER_COUNT) {
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);
  uint32_t num_k_chunks = K_dim / BUFFER_COUNT;
  uint32_t num_n_tiles = N_dim / BUFFER_COUNT;

  uint32_t row_addr_X = mram_row0_addr_X;
  uint32_t write_addr_Z = mram_row0_addr_Z;

  for (uint32_t row = 0; row < ROWS; row++) {
    uint32_t ntile_row0_addr_Y = mram_base_addr_Y;

    for (uint32_t n_tile = 0; n_tile < num_n_tiles; n_tile++) {
      for (uint32_t col = 0; col < BUFFER_COUNT; col++) {
        cache_C[col] = 0;
      }

      uint32_t chunk_addr_X = row_addr_X;
      uint32_t k_chunk_addr_Y = ntile_row0_addr_Y;

      for (uint32_t k_chunk = 0; k_chunk < num_k_chunks; k_chunk++) {
        // Read this row's K-chunk of X once, reuse it against every column
        // in the current N-tile below.
        mram_read((__mram_ptr void const *)(chunk_addr_X), cache_A,
                  BUFFER_SIZE);

        uint32_t row_addr_Y = k_chunk_addr_Y;
        for (uint32_t col = 0; col < BUFFER_COUNT; col++) {
          mram_read((__mram_ptr void const *)(row_addr_Y), cache_B,
                    BUFFER_SIZE);
          for (uint32_t c = 0; c < BUFFER_COUNT; c++) {
            cache_C[col] += cache_A[c] * cache_B[c];
          }
          row_addr_Y += K_dim * sizeof(T);
        }

        chunk_addr_X += BUFFER_SIZE;
        k_chunk_addr_Y += BUFFER_SIZE;
      }

      mram_write(cache_C, (__mram_ptr void *)(write_addr_Z), BUFFER_SIZE);
      write_addr_Z += BUFFER_SIZE;
      ntile_row0_addr_Y += BUFFER_COUNT * K_dim * sizeof(T);
    }

    row_addr_X += K_dim * sizeof(T);
  }
}

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

  uint32_t THREADS = DPU_INPUT_ARGUMENTS.THREADS[0];
  if (tasklet_id >= THREADS)
    return 0;

  uint32_t ITERS_ROW = DPU_INPUT_ARGUMENTS.ITERS[0]; // rows of A per tasklet
  uint32_t K = DPU_INPUT_ARGUMENTS.ITERS[1];         // A's cols / B's rows
  uint32_t N = DPU_INPUT_ARGUMENTS.ITERS[2];         // B's cols / C's rows
  uint32_t P = DPU_INPUT_ARGUMENTS.ITERS[3];         // C's cols (final output)

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  uint32_t total_rows = THREADS * ITERS_ROW;

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(BUFFER_SIZE);

  uint32_t matA_el_size = total_rows * K * sizeof(T);
  // B and C are stored transposed on the host (B: N rows of K elements; C:
  // P rows of N elements), so each is read with contiguous mram_reads on
  // the device, matching how rows of A/E are read -- see matmul_pass.
  uint32_t matB_el_size = (uint32_t)N * K * sizeof(T);
  uint32_t matC_el_size = (uint32_t)P * N * sizeof(T);
  // E is a scratch intermediate (this tasklet's row-block of E = A x B): it
  // never leaves the device -- phase 2 reads it straight back out of MRAM
  // as its own "A" input, so the whole chained 2mm happens on-DPU.
  uint32_t matE_el_size = total_rows * N * sizeof(T);

  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_B = mram_base_addr_A + matA_el_size;
  uint32_t mram_base_addr_C = mram_base_addr_B + matB_el_size;
  uint32_t mram_base_addr_E = mram_base_addr_C + matC_el_size;
  uint32_t mram_base_addr_D = mram_base_addr_E + matE_el_size;

  uint32_t mram_tile_addr_A =
      mram_base_addr_A + (tasklet_id * ITERS_ROW * K) * sizeof(T);
  uint32_t mram_tile_addr_E =
      mram_base_addr_E + (tasklet_id * ITERS_ROW * N) * sizeof(T);
  uint32_t mram_tile_addr_D =
      mram_base_addr_D + (tasklet_id * ITERS_ROW * P) * sizeof(T);

  // Phase 1: E = A x B, for this tasklet's row-block.
  matmul_pass(cache_A, cache_B, cache_C, mram_tile_addr_A, mram_base_addr_B,
              mram_tile_addr_E, ITERS_ROW, K, N, BUFFER_COUNT);

  // Phase 2: D = E x C -- E (just written above) becomes this phase's "A".
  // No barrier needed between phases: each tasklet only ever reads back the
  // E region it itself wrote, never another tasklet's.
  matmul_pass(cache_A, cache_B, cache_C, mram_tile_addr_E, mram_base_addr_C,
              mram_tile_addr_D, ITERS_ROW, N, P, BUFFER_COUNT);

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}
