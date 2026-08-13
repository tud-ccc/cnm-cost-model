config_header_file_addr = "./generated_headers/config.h"
common_struct_file_addr = "./generated_headers/common_structs.h"
gen_map_file_addr = "./generated_headers/gen_map.h"
execution_output = "./output/reduceop.csv"

# How many combos may run concurrently on the board (each uses its own
# bin_slot{N}/generated_headers_slot{N} build directory). Board has 40 DPU
# modules available; keep this well under that.
PARALLEL_INSTANCES = 8

thread_range = [1, 2, 4, 8, 16]
# float/double excluded: real hardware shows a real ~31% latency jump at
# thread=16 (contention on a shared FP execution resource, most likely)
# that the cost model doesn't capture at all -- predictions for those two
# types are unreliable until that's modeled.
data_types = ["uint16_t", "uint32_t", "uint64_t"]
operations_str = ["ADD"]
iterations = [256, 512, 1024, 2048, 4096, 8192, 16384, 32768]
buffer_range = [8, 16, 32, 64, 128, 256]


nwarm = 3
nrep = 3
log = False
sim = False
