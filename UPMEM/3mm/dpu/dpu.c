
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
// each is one contiguous column of the logical K_dim x N_dim matrix). Same
// helper as Gemm/2mm -- "3mm" is three instances of this, chained: E=A*B,
// F=C*D, G=E*F, all inside the DPU (E and F are MRAM scratch, never
// transferred to the host).
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

  // E = A x B (NI x NJ), F = C x D (NJ x NL), G = E x F (NI x NL).
  uint32_t NI_PER_THREAD = DPU_INPUT_ARGUMENTS.ITERS[0]; // A/E/G rows/tasklet
  uint32_t NK = DPU_INPUT_ARGUMENTS.ITERS[1];            // A cols / B rows
  uint32_t NJ = DPU_INPUT_ARGUMENTS.ITERS[2];            // B cols / C rows
  uint32_t NM = DPU_INPUT_ARGUMENTS.ITERS[3];            // C cols / D rows
  uint32_t NL_PER_THREAD = DPU_INPUT_ARGUMENTS.ITERS[4]; // D/F rows/tasklet

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  uint32_t NI_total = THREADS * NI_PER_THREAD;
  uint32_t NL_total = THREADS * NL_PER_THREAD;

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(BUFFER_SIZE);

  // A, B_T, D_T are pushed from the host already in the layout matmul_pass
  // needs (B_T/D_T transposed for contiguous per-column reads -- see
  // Gemm/2mm's dpu.c comment for why). C is pushed in its OWN natural,
  // non-transposed layout (NJ rows of NM elements) -- see the phase-2
  // comment below for why that's exactly the Y_T shape phase 2 needs
  // without any extra transpose.
  uint32_t matA_el_size = NI_total * NK * sizeof(T);
  uint32_t matB_el_size = (uint32_t)NJ * NK * sizeof(T);
  uint32_t matC_el_size = (uint32_t)NJ * NM * sizeof(T);
  uint32_t matD_el_size = (uint32_t)NL_total * NM * sizeof(T);
  // E and F are MRAM scratch -- neither is ever transferred to the host.
  // E is private per-tasklet (each tasklet only ever reads back the E rows
  // it wrote itself in phase 1, for phase 3). F is shared: every tasklet's
  // phase-3 G row-block needs the FULL F matrix (F's rows are indexed by
  // NJ, unrelated to which NI-rows this tasklet owns), so phase 2's writes
  // are partitioned across tasklets by NL instead, and a barrier separates
  // phase 2 from phase 3 so every tasklet sees all of F before reading it.
  uint32_t matE_el_size = NI_total * NJ * sizeof(T);
  uint32_t matF_el_size = (uint32_t)NL_total * NJ * sizeof(T);

  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_B = mram_base_addr_A + matA_el_size;
  uint32_t mram_base_addr_C = mram_base_addr_B + matB_el_size;
  uint32_t mram_base_addr_D = mram_base_addr_C + matC_el_size;
  uint32_t mram_base_addr_E = mram_base_addr_D + matD_el_size;
  uint32_t mram_base_addr_F = mram_base_addr_E + matE_el_size;
  uint32_t mram_base_addr_G = mram_base_addr_F + matF_el_size;

  uint32_t mram_tile_addr_A =
      mram_base_addr_A + (tasklet_id * NI_PER_THREAD * NK) * sizeof(T);
  uint32_t mram_tile_addr_D =
      mram_base_addr_D + (tasklet_id * NL_PER_THREAD * NM) * sizeof(T);
  uint32_t mram_tile_addr_E =
      mram_base_addr_E + (tasklet_id * NI_PER_THREAD * NJ) * sizeof(T);
  uint32_t mram_tile_addr_F =
      mram_base_addr_F + (tasklet_id * NL_PER_THREAD * NJ) * sizeof(T);
  uint32_t mram_tile_addr_G =
      mram_base_addr_G + (tasklet_id * NI_PER_THREAD * NL_total) * sizeof(T);

  // Phase 1: E = A x B, for this tasklet's NI row-block. Private E, no sync
  // needed (same as Gemm/2mm).
  matmul_pass(cache_A, cache_B, cache_C, mram_tile_addr_A, mram_base_addr_B,
              mram_tile_addr_E, NI_PER_THREAD, NK, NJ, BUFFER_COUNT);

  // Phase 2: F_T = D_T x C, for this tasklet's NL row-block of F_T (a
  // different partitioning than phase 1/3's NI-based one). Computing F
  // TRANSPOSED directly -- instead of F=C*D then transposing it -- means
  // using D_T (this tasklet's row-block) as the row-owner and C, in its
  // untransposed NJ x NM layout, as the Y_T operand: F_T[l][j] = sum_m
  // D_T[l][m]*C[j][m] = sum_m D[m][l]*C[j][m] = F[j][l], which is exactly
  // matmul_pass's Z[row][n] = sum_k X[row][k]*Y_T[n][k] shape with no
  // extra transpose step required for either input.
  matmul_pass(cache_A, cache_B, cache_C, mram_tile_addr_D, mram_base_addr_C,
              mram_tile_addr_F, NL_PER_THREAD, NM, NJ, BUFFER_COUNT);

  // F is shared -- every tasklet must see the complete matrix (written by
  // all tasklets, not just itself) before any of them can start phase 3.
  barrier_wait(&my_barrier);

  // Phase 3: G = E x F. E is this tasklet's own (private) row-block; F_T
  // (mram_base_addr_F, now fully written by every tasklet) is exactly the
  // Y_T shape phase 3 needs (NL_total rows of NJ elements).
  matmul_pass(cache_A, cache_B, cache_C, mram_tile_addr_E, mram_base_addr_F,
              mram_tile_addr_G, NI_PER_THREAD, NJ, NL_total, BUFFER_COUNT);

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}
