#include "../generated_headers/config.h"
#include "../support/common.h"
#include "../support/timer.h"
#include <assert.h>
#include <dpu.h>
#include <dpu_log.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#ifndef DPU_BINARY
#define DPU_BINARY "./bin/dpu"
#endif

int main() {
  int rep = 5;
  int warmup = 3;
  struct dpu_set_t dpu_set, dpu;

  Timer timer;
  for (int r = 0; r < (rep + warmup); r++) {
    DPU_ASSERT(dpu_alloc(1, NULL, &dpu_set));
    DPU_ASSERT(dpu_load(dpu_set, DPU_BINARY, NULL));
    dpu_arguments_t *input_args =
        (dpu_arguments_t *)malloc(sizeof(dpu_arguments_t));
    int i = 0;
    DPU_FOREACH(dpu_set, dpu, i) {
      input_args[i].ITER = ITERATION;
      DPU_ASSERT(dpu_prepare_xfer(dpu, input_args + i));
    }
    DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU, "DPU_INPUT_ARGUMENTS", 0,
                             sizeof(dpu_arguments_t), DPU_XFER_DEFAULT));

    if (r >= (warmup)) {
      start(&timer, 0, r - warmup);
    }

    DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));

    if (r >= (warmup)) {
      stop(&timer, 0);
    }

    DPU_ASSERT(dpu_free(dpu_set));
  }
  print(&timer, 0, rep);

  return 0;
}
