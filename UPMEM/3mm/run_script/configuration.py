config_header_file_addr = "./generated_headers/config.h"
common_struct_file_addr = "./generated_headers/common_structs.h"
gen_map_file_addr = "./generated_headers/gen_map.h"
execution_output = "./output/3mm.csv"

thread_range = [1, 4, 8, 16]
data_types = ["uint32_t"]

ni_lengths = [16, 128]
nk_lengths = [16, 128]
nj_lengths = [16, 128]
nm_lengths = [16, 128]
nl_lengths = [16, 128]
buffer_range = [8, 64]

nwarm = 3
nrep = 3
log = False
sim = False
