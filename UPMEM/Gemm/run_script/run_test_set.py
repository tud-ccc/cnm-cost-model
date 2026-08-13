import os
import subprocess
import sys
from create_headers import *
from functools import reduce

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
import parallel_sweep

# Small, curated set of distinct matrix shapes / buffer sizes -- deliberately
# different from the configs already in output/gemv.csv -- run across
# thread=[1,2,4,8,16] to spot-check whether the mulsi3 instruction-count fix
# still holds up on shapes we haven't tested yet, without doing the full
# sweep.
TEST_CONFIGS = [
    # (iteration, vec_length, buffer_size)
    (64, 64, 32),      # small square
    (2048, 128, 64),   # tall/thin: many rows, few cols
    (64, 2048, 64),    # wide/short: few rows, many cols
    (512, 512, 64),    # medium square
    (1024, 1024, 512), # large buffer
    (256, 2048, 8),    # many small-buffer chunks
]

THREADS = [1, 2, 4, 8, 16]


def runKernelOnADPUS(rank_dims, dpu_dims, thread_dims, iter_dims, buffer_sizes):
    slot = parallel_sweep.get_slot()
    createConfigurationHeader(
        "TEMP",
        "uint32_t",
        rank_dims,
        dpu_dims,
        thread_dims,
        iter_dims,
        buffer_sizes,
        generated_dir=f"generated_headers_slot{slot}",
    )
    make_vars = [f"GENERATED_DIR=generated_headers_slot{slot}", f"BUILDDIR=bin_slot{slot}"]
    total_thread_count = reduce(lambda x, y: x * y, thread_dims)
    subprocess.run(["make", "clean"] + make_vars, stdout=subprocess.PIPE)
    subprocess.run(
        ["make", "NR_TASKLETS=" + str(total_thread_count)] + make_vars,
        stdout=subprocess.PIPE,
    )
    execution = subprocess.run([f"./bin_slot{slot}/host"], stdout=subprocess.PIPE)
    return execution.stdout.decode("utf-8").strip(), execution.returncode == 0


os.chdir("../")

header = [
    "operation",
    "data_type",
    "thread",
    "iteration",
    "vec_length",
    "buffer_size",
    "latency",
    "correct",
]


def _run_combo(combo):
    thread, iteration, vec_length, buffer_size = combo

    print(
        f"[slot {parallel_sweep.get_slot()}] Running thread={thread}, iteration={iteration}, "
        f"vec_length={vec_length}, buffer_size={buffer_size}",
        flush=True,
    )

    rank_dims = [1]
    dpu_dims = [1]
    thread_dims = [thread]
    iter_dims = [iteration, vec_length]
    buffer_sizes = [buffer_size]

    latency, correct = runKernelOnADPUS(
        rank_dims, dpu_dims, thread_dims, iter_dims, buffer_sizes
    )
    try:
        latency = float(latency)
    except ValueError:
        latency = "NA"
    return ["MATVEC", "uint32_t", thread, iteration, vec_length, buffer_size, latency, correct]


if __name__ == "__main__":
    combos = [
        (thread, iteration, vec_length, buffer_size)
        for thread in THREADS
        for iteration, vec_length, buffer_size in TEST_CONFIGS
    ]

    parallel_sweep.run_parallel_sweep(
        combos, _run_combo, header, "output/gemv_test_set.csv", PARALLEL_INSTANCES,
        kernel_name="Gemm-test-set", combo_key_fn=lambda c: ("MATVEC", "uint32_t") + c,
    )
