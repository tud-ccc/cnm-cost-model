#include <assert.h>
#include <dpu.h>
#include <stdint.h>
#include <stdio.h>

#ifndef DPU_BINARY
#define DPU_BINARY "./bin/dpu"
#endif

int main() {
  struct dpu_set_t dpu_set, dpu;
  DPU_ASSERT(dpu_alloc(1, NULL, &dpu_set));
  DPU_ASSERT(dpu_load(dpu_set, DPU_BINARY, NULL));
  DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));

  uint32_t nb_cycle_with = 0, nb_cycle_without = 0;
  DPU_FOREACH(dpu_set, dpu) {
    DPU_ASSERT(dpu_copy_from(dpu, "nb_cycle_with", 0, &nb_cycle_with,
                              sizeof(uint32_t)));
    DPU_ASSERT(dpu_copy_from(dpu, "nb_cycle_without", 0, &nb_cycle_without,
                              sizeof(uint32_t)));
  }
  printf("%u %u\n", nb_cycle_with, nb_cycle_without);

  DPU_ASSERT(dpu_free(dpu_set));
  return 0;
}
