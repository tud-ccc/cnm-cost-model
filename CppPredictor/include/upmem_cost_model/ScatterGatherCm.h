#include <assert.h>

namespace upmem_cm {
double scatterSgCostMs(int num_dpus, int block_size, int blocks_per_dpu);
double scatterBlockCostMs(int num_dpus, int block_size);
double broadcastCostMs(int num_dpus, int block_size);
double gatherCostMs(int num_dpus, int block_size);
} // namespace upmem_cm
