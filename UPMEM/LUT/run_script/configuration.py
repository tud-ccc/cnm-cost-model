thread_range = [1, 2, 4, 8, 12, 16, 24]
data_types = ["uint16_t", "uint32_t", "uint64_t", "float", "double"]
operations_str = ["ADD", "SUB", "MUL", "DIV", "LOAD", "STORE"]

pipeline_asm_addr = "./dpu/pipeline"
dma_asm_addr = "./dpu/dma"
custom_dma_asm_addr = "./dpu/custom_dma"

ARR_SIZE = 1 << 21

INIT_VAL_MIN = 1
INIT_VAL_MAX = 1 << 31
RANGE_STEP = 1

buffer_size = 256

iteration = 2048
iteration2 = 128
FREQ = 350000

## DMA specific config
dma_types = ["DMA_LOAD", "DMA_STORE"]
dma_start_size = 8
dma_max_size = 4096

duplicate_count_max = 4
