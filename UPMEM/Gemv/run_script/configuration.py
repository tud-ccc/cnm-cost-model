config_header_file_addr = "./generated_headers/config.h"
common_struct_file_addr = "./generated_headers/common_structs.h"
gen_map_file_addr = "./generated_headers/gen_map.h"
execution_output = "./output/gemv.csv"

# How many combos may run concurrently on the board (each uses its own
# bin_slot{N}/generated_headers_slot{N} build directory). Board has 40 DPU
# modules available; keep this well under that.
PARALLEL_INSTANCES = 8

thread_range = [1, 2, 4, 8, 16]
data_types = ["uint32_t"]
iterations = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
vec_lengths = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
# buffer_size=4 excluded: unreliable under this project's concurrent
# (PARALLEL_INSTANCES=8) sweep collection at low thread counts (see
# Predictor/output/full_comparison notes), and the source of most of the
# remaining small-buffer/high-thread DMA residual otherwise. Removed from
# the recorded results and dropped here so a fresh sweep doesn't
# reintroduce it.
buffer_range = [8, 16, 32, 64, 128, 256]

nwarm = 3
nrep = 3
log = False
sim = False
