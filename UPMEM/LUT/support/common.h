#include <stdint.h>
typedef struct {
  uint32_t ITER;
  uint32_t buffer_size;
  uint32_t thread_count;
  uint32_t read;
  uint32_t arr_size;
  uint32_t custom_range;
} dpu_arguments_t;
