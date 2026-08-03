import os
import subprocess
import sys
from create_headers import *
from functools import reduce

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
import parallel_sweep

# Thread=16 only. m = per-tasklet row count (iteration), k = vec_length,
# buffer = buffer_size. Only combos where buffer_size <= m produce a
# non-empty num_row_batches (dpu.c: num_row_batches = iteration/buffer_size,
# integer division truncates to 0 and the kernel does no work if buffer > m).
THREAD = 16
M_VALUES = [1, 2, 4, 8, 16, 32]
K_VALUES = [128, 256, 512, 1024]
BUFFER_VALUES = [16, 32, 64, 128]


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
    m, k, buffer_size = combo

    print(
        f"[slot {parallel_sweep.get_slot()}] Running thread=16, m(iteration)={m}, k(vec_length)={k}, "
        f"buffer_size={buffer_size}",
        flush=True,
    )

    latency, correct = runKernelOnADPUS([1], [1], [THREAD], [m, k], [buffer_size])
    try:
        latency = float(latency)
    except ValueError:
        latency = "NA"
    return ["MATVEC", "uint32_t", THREAD, m, k, buffer_size, latency, correct]


if __name__ == "__main__":
    combos = []
    for m in M_VALUES:
        for k in K_VALUES:
            for buffer_size in BUFFER_VALUES:
                if buffer_size > m or buffer_size > k:
                    continue
                combos.append((m, k, buffer_size))

    parallel_sweep.run_parallel_sweep(
        combos, _run_combo, header, "output/gemv_mk_sweep.csv", PARALLEL_INSTANCES,
        kernel_name="Gemm-mk-sweep", combo_key_fn=lambda c: ("MATVEC", "uint32_t", THREAD) + c,
    )
