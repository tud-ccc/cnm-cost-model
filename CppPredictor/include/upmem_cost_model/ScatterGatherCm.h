#include <assert.h>

namespace upmem_cm {
double scatterSgCostMs(int num_dpus, int block_size, int blocks_per_dpu);
double scatterBlockCostMs(int num_dpus, int block_size);
/// A scatter of `block_size` bytes to each of `num_dpus` DPUs, all read from
/// one host block that stays in cache: the transfer without the read of
/// distinct data from DRAM.
double scatterSharedCostMs(int num_dpus, int block_size);
/// A scatter of `block_size` bytes to each of `num_dpus` DPUs from a source
/// with `unique_bytes` distinct bytes: T_xfer(D, B) + T_read(U, D), the
/// shared transfer plus the read those unique bytes cost. Equal to
/// scatterBlockCostMs when every DPU's block is distinct (unique_bytes >=
/// num_dpus * block_size).
double scatterReplicatedCostMs(int num_dpus, int block_size, double unique_bytes);
double broadcastCostMs(int num_dpus, int block_size);
double gatherCostMs(int num_dpus, int block_size);
double gatherSgCostMs(int num_dpus, int blocks_per_dpu, int block_size);
} // namespace upmem_cm
