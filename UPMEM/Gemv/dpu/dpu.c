
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
  uint32_t ITERS_ROW = DPU_INPUT_ARGUMENTS.ITERS[0];
  uint32_t VEC_LEN = DPU_INPUT_ARGUMENTS.ITERS[1];

  uint32_t BUFFER_COUNT = DPU_INPUT_ARGUMENTS.BUFFER_SIZES[0];
  uint32_t BUFFER_SIZE = BUFFER_COUNT * sizeof(T);

  uint32_t total_rows = THREADS * ITERS_ROW;
  uint32_t total_cols = VEC_LEN;

  // Each tasklet's C region is strided by PADDED_ITERS_ROW, not ITERS_ROW:
  // MRAM writes must be a multiple of 8 bytes, so when ITERS_ROW is odd the
  // final partial-batch write (pending_rows elements, rounded up to even)
  // would overrun by one element. Since tasklets' C regions are packed with
  // no gap, that overrun would land in the next tasklet's first row. Padding
  // every tasklet's stride up to the next even element count gives that
  // one-element overrun somewhere harmless (this tasklet's own unread
  // padding) instead. The host strips the padding back out on readback.
  uint32_t padded_iters_row = ITERS_ROW + (ITERS_ROW % 2);

  T *cache_A = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_B = (T *)mem_alloc(BUFFER_SIZE);
  T *cache_C = (T *)mem_alloc(BUFFER_SIZE);

  uint32_t mat_el_size = total_rows * total_cols * sizeof(T);
  uint32_t vec_el_size = total_cols * sizeof(T);

  uint32_t mram_base_addr_A = (uint32_t)(DPU_MRAM_HEAP_POINTER);
  uint32_t mram_base_addr_B = (uint32_t)(DPU_MRAM_HEAP_POINTER) + mat_el_size;
  uint32_t mram_base_addr_C =
      (uint32_t)(DPU_MRAM_HEAP_POINTER) + mat_el_size + vec_el_size;

  uint32_t mram_tile_addr_A =
      mram_base_addr_A + (tasklet_id * ITERS_ROW * total_cols) * sizeof(T);
  uint32_t mram_tile_addr_C =
      mram_base_addr_C + (tasklet_id * padded_iters_row) * sizeof(T);

  uint32_t num_col_chunks = total_cols / BUFFER_COUNT;

  uint32_t row_addr_A = mram_tile_addr_A;
  uint32_t write_addr_C = mram_tile_addr_C;
  uint32_t pending_rows = 0;

  // The output write-batch size is decoupled from the K-chunk size: rows are
  // computed one at a time and cache_C is flushed to MRAM whenever it fills,
  // independent of how ITERS_ROW (m) relates to BUFFER_COUNT. This removes
  // the old requirement that buffer_size <= m (previously the row loop ran
  // in groups of exactly BUFFER_COUNT, so num_row_batches = m / BUFFER_COUNT
  // truncated to 0 -- and computed nothing -- whenever buffer_size > m).
  for (uint32_t rr = 0; rr < ITERS_ROW; rr++) {
    T sum = 0;
    uint32_t chunk_addr_A = row_addr_A;
    uint32_t chunk_addr_B = mram_base_addr_B;

    for (uint32_t chunk = 0; chunk < num_col_chunks; chunk++) {
      mram_read((__mram_ptr void const *)(chunk_addr_A), cache_A,
                BUFFER_SIZE);
      mram_read((__mram_ptr void const *)(chunk_addr_B), cache_B,
                BUFFER_SIZE);
      for (uint32_t c = 0; c < BUFFER_COUNT; c++) {
        sum += cache_A[c] * cache_B[c];
      }
      chunk_addr_A += BUFFER_SIZE;
      chunk_addr_B += BUFFER_SIZE;
    }

    cache_C[pending_rows] = sum;
    pending_rows++;
    row_addr_A += total_cols * sizeof(T);

    if (pending_rows == BUFFER_COUNT) {
      mram_write(cache_C, (__mram_ptr void *)(write_addr_C), BUFFER_SIZE);
      write_addr_C += BUFFER_SIZE;
      pending_rows = 0;
    }
  }

  // Flush a final partial batch (fewer than BUFFER_COUNT rows left over),
  // rounded up to an even element count for MRAM's 8-byte alignment
  // requirement. Safe within cache_C's own BUFFER_COUNT-element capacity
  // (pending_rows < BUFFER_COUNT always), and safe against the next
  // tasklet's region thanks to the padded_iters_row stride above.
  if (pending_rows != 0) {
    uint32_t write_count = pending_rows + (pending_rows % 2);
    mram_write(cache_C, (__mram_ptr void *)(write_addr_C),
               write_count * sizeof(T));
  }

#ifdef ENABLE_PERFCOUNTER
  barrier_wait(&my_barrier);
  if (tasklet_id == 0) {
    nb_cycle = perfcounter_get();
  }
#endif

  return 0;
}
