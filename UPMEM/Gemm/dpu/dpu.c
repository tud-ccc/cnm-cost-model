
#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>
#include <perfcounter.h>
#include <seqread.h>
#include <stdint.h>
#include <stdio.h>

#include "common_structs.h"
#include "config.h"
#include "gen_map.h"

__host dpu_arguments_t DPU_INPUT_ARGUMENTS;
__host uint32_t nb_cycle;

// Barrier
BARRIER_INIT(my_barrier, NR_TASKLETS);

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
  uint32_t K = DPU_INPUT_ARGUMENTS.ITERS[1];         // reduction dimension
  uint32_t N = DPU_INPUT_ARGUMENTS.ITERS[2];         // output columns

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  uint32_t total_rows = THREADS * ITERS_ROW;

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(BUFFER_SIZE);

  uint32_t matA_el_size = total_rows * K * sizeof(T);
  // B is stored transposed on the host (N rows of K elements, i.e. row n
  // holds column n of the logical K x N matrix), so the DPU can read each
  // column of B with a single contiguous mram_read -- mirroring how it
  // already reads rows of A -- instead of a strided access pattern.
  uint32_t matB_el_size = (uint32_t)N * K * sizeof(T);

  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_B =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + matA_el_size;
  uint32_t mram_base_addr_C =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + matA_el_size + matB_el_size;

  uint32_t mram_tile_addr_A =
      mram_base_addr_A + (tasklet_id * ITERS_ROW * K) * sizeof(T);
  uint32_t mram_tile_addr_C =
      mram_base_addr_C + (tasklet_id * ITERS_ROW * N) * sizeof(T);

  // K and N are each assumed to be exact multiples of BUFFER_COUNT for now
  // (no remainder-tile handling yet -- same starting assumption Gemv
  // originally made for its K dimension, before row-remainder handling was
  // added once the basic version worked).
  uint32_t num_k_chunks = K / BUFFER_COUNT;
  uint32_t num_n_tiles = N / BUFFER_COUNT;

  uint32_t row_addr_A = mram_tile_addr_A;
  uint32_t write_addr_C = mram_tile_addr_C;

  for (uint32_t row = 0; row < ITERS_ROW; row++) {
    uint32_t ntile_row0_addr_B = mram_base_addr_B;

    for (uint32_t n_tile = 0; n_tile < num_n_tiles; n_tile++) {
      for (uint32_t col = 0; col < BUFFER_COUNT; col++) {
        cache_C[col] = 0;
      }

      uint32_t chunk_addr_A = row_addr_A;
      uint32_t k_chunk_addr_B = ntile_row0_addr_B;

      for (uint32_t k_chunk = 0; k_chunk < num_k_chunks; k_chunk++) {
        // Read this row's K-chunk of A once, reuse it against every column
        // in the current N-tile below -- this is what amortizes A's DMA
        // reads across multiple outputs instead of re-reading per column.
        mram_read((__mram_ptr void const *)(chunk_addr_A), cache_A,
                  BUFFER_SIZE);

        uint32_t row_addr_B = k_chunk_addr_B;
        for (uint32_t col = 0; col < BUFFER_COUNT; col++) {
          mram_read((__mram_ptr void const *)(row_addr_B), cache_B,
                    BUFFER_SIZE);
          for (uint32_t c = 0; c < BUFFER_COUNT; c++) {
            cache_C[col] += cache_A[c] * cache_B[c];
          }
          row_addr_B += K * sizeof(T);
        }

        chunk_addr_A += BUFFER_SIZE;
        k_chunk_addr_B += BUFFER_SIZE;
      }

      mram_write(cache_C, (__mram_ptr void *)(write_addr_C), BUFFER_SIZE);
      write_addr_C += BUFFER_SIZE;
      ntile_row0_addr_B += BUFFER_COUNT * K * sizeof(T);
    }

    row_addr_A += K * sizeof(T);
  }

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}
