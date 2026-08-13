from pathlib import Path

_REPO_ROOT = Path(__file__).resolve().parents[2]
_NEW_EVAL_ROOT = Path(__file__).resolve().parents[1]
_UPMEM_ROOT = _NEW_EVAL_ROOT / "UPMEM"
_LUT_OUTPUT = _UPMEM_ROOT / "LUT" / "output"

pipeline_normal_csv_file = _LUT_OUTPUT / "pipeline_normal_parsed.csv"
pipeline_func_csv_file = _LUT_OUTPUT / "pipeline_func_parsed.csv"
dma_csv_file = _LUT_OUTPUT / "manual_dma_parsed.csv"
custom_dma_csv_file = _LUT_OUTPUT / "custom_dma_parsed.csv"
dma_service_csv_file = _LUT_OUTPUT / "custom_dma_parsed.csv"
freq = 350000
base_time = 0.066011

real_latenceis_dir = _REPO_ROOT / "temp" / "TestingLUT" / "output"
custom_dma_file = real_latenceis_dir / "raw.csv"
vector_op_file = _UPMEM_ROOT / "Vecadd" / "output" / "vectorop.csv"
gemv_file = _UPMEM_ROOT / "Gemv" / "output" / "gemv.csv"
reduceop_file = _UPMEM_ROOT / "ReduceOp" / "output" / "reduceop.csv"
sel_file = _UPMEM_ROOT / "Sel" / "output" / "sel.csv"
hst_file = _UPMEM_ROOT / "Hst" / "output" / "hst.csv"
gemm_file = _UPMEM_ROOT / "Gemm" / "output" / "gemm.csv"
barrier_cost_csv_file = (
    _UPMEM_ROOT / "BarrierExperiment" / "output" / "barrier_cost.csv"
)
scan_file = _UPMEM_ROOT / "Scan" / "output" / "scan.csv"
dma_contention_csv_file = (
    _NEW_EVAL_ROOT / "DmaScalingExperiment" / "output" / "dma_contention.csv"
)

thread_range = [1, 4, 8, 16]
data_types = ["uint16_t"]
operations_str = ["ADD"]
iterations = [256]
buffer_range = [8, 16, 32, 64, 128]
buffer_sizes = [8, 16, 32, 64, 128, 256, 512, 1024, 2048]

dup_max = 4

custom_ranges = [0, 1, 2, 4, 8, 16, 32, 64, 128]

iteration = 512
