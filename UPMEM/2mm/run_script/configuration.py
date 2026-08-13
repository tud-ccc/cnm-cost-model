config_header_file_addr = "./generated_headers/config.h"
common_struct_file_addr = "./generated_headers/common_structs.h"
gen_map_file_addr = "./generated_headers/gen_map.h"
execution_output = "./output/2mm.csv"

thread_range = [1, 4, 8, 16]
data_types = ["uint32_t"]

iterations = [16, 128, 512]
vec_lengths = [16, 128, 512]
n_lengths = [16, 128, 512]
p_lengths = [16, 128, 512]
buffer_range = [8, 64, 256]

nwarm = 3
nrep = 3
log = False
sim = False
