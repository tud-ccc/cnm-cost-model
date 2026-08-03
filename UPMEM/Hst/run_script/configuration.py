config_header_file_addr = "./generated_headers/config.h"
common_struct_file_addr = "./generated_headers/common_structs.h"
gen_map_file_addr = "./generated_headers/gen_map.h"
execution_output = "./output/hst.csv"

# How many combos may run concurrently on the board (each uses its own
# bin_slot{N}/generated_headers_slot{N} build directory). Board has 40 DPU
# modules available; keep this well under that.
PARALLEL_INSTANCES = 8

thread_range = [1, 2, 4, 8, 16]
# (value*BINS)>>DEPTH is undefined for float/double, so those two are
# excluded here -- only integer types are supported (same situation as Sel).
data_types = ["uint16_t", "uint32_t", "uint64_t"]
iterations = [128, 256, 512, 1024, 2048, 4096, 8192, 16384]
buffer_range = [8, 16, 32, 64, 128]

nwarm = 3
nrep = 3
log = False
sim = False
DEPTH = 12
BINS = 256
