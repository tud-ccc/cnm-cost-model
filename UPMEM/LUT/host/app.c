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

static T *A;
static T *B;
static long *C;

void initGlobalVars() {
  size_t arr_size = ARR_SIZE * sizeof(T);
  size_t output_size = NR_TASKLETS * sizeof(long);
  A = (T *)malloc(arr_size);
  B = (T *)malloc(arr_size);
  C = (T *)malloc(output_size);
  srand(time(NULL)); // Initialization, should only be called once.
  for (int r = 0; r < ARR_SIZE; r++) {
    A[r] = INIT_VAR;
    B[r] = INIT_VAR;
    // A[r] = 128;
    // B[r] = 128;
  }
}

T expectedResult() {
  T res = 0;
  for (int i = 0; i < (ARR_SIZE); i++) {
    res += (A[i] * B[i]);
  }
  return res;
}
int main() {
  initGlobalVars();
  int rep = 5;
  int warmup = 3;
  size_t arr_size = ARR_SIZE * sizeof(T);
  uint32_t each_dpu;
  struct dpu_set_t dpu_set, dpu;

  Timer timer;
  for (int r = 0; r < (rep + warmup); r++) {
    DPU_ASSERT(dpu_alloc(1, NULL, &dpu_set));
    DPU_ASSERT(dpu_load(dpu_set, DPU_BINARY, NULL));
    dpu_arguments_t *input_args =
        (dpu_arguments_t *)malloc(sizeof(dpu_arguments_t));
    int i = 0;
    DPU_FOREACH(dpu_set, dpu, i) {
      // Copy input arguments to DPU
      input_args[i].ITER = ITERATION;
      input_args[i].buffer_size = BUFFER_SIZE;
      input_args[i].arr_size = ARR_SIZE;

      DPU_ASSERT(dpu_prepare_xfer(dpu, input_args + i));
    }
    DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU, "DPU_INPUT_ARGUMENTS", 0,
                             sizeof(dpu_arguments_t), DPU_XFER_DEFAULT));

    DPU_FOREACH(dpu_set, dpu, each_dpu) {
      DPU_ASSERT(dpu_prepare_xfer(dpu, A));
    }

    DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                             DPU_MRAM_HEAP_POINTER_NAME, 0, arr_size,
                             DPU_XFER_DEFAULT));

    DPU_FOREACH(dpu_set, dpu, each_dpu) {
      DPU_ASSERT(dpu_prepare_xfer(dpu, B));
    }
    DPU_ASSERT(dpu_push_xfer(dpu_set, DPU_XFER_TO_DPU,
                             DPU_MRAM_HEAP_POINTER_NAME, arr_size, arr_size,
                             DPU_XFER_DEFAULT));
    if (r >= (warmup)) {
      start(&timer, 0, r - warmup);
    }

    DPU_ASSERT(dpu_launch(dpu_set, DPU_SYNCHRONOUS));

    if (r >= (warmup)) {
      stop(&timer, 0);
    }
    // DPU_FOREACH(dpu_set, dpu)
    // 	{
    // 		printf("DPU#%d:\n", each_dpu);
    // 		DPU_ASSERT(dpulog_read_for_dpu(dpu.dpu, stdout));
    // 		each_dpu++;
    // 	}

    DPU_ASSERT(dpu_free(dpu_set));
  }
  print(&timer, 0, rep);

  return 0;
}
