import os
import subprocess
import sys
from create_headers import *
from functools import reduce

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
import parallel_sweep


def data_type_size(data_type):
    if data_type == "uint16_t":
        return 2
    elif data_type == "uint32_t":
        return 4
    elif data_type == "uint64_t":
        return 8
    elif data_type == "float":
        return 4
    elif data_type == "double":
        return 8


def runKernelOnADPUS(
    rank_dims,
    dpu_dims,
    thread_dims,
    iter_dims,
    buffer_sizes,
):

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


def parse_latency(string):
    if string.replace(".", "").isnumeric():
        return float(string)
    else:
        return "NA"


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
        rank_dims,
        dpu_dims,
        thread_dims,
        iter_dims,
        buffer_sizes,
    )
    try:
        latency = float(latency)
    except ValueError:
        latency = "NA"
    return ["MATVEC", "uint32_t", thread, iteration, vec_length, buffer_size, latency, correct]


if __name__ == "__main__":
    combos = []
    for thread in thread_range:
        for iteration in iterations:
            for vec_length in vec_lengths:
                for buffer_size in buffer_range:
                    # buffer_size > vec_length (k) still must be skipped: it
                    # makes num_col_chunks truncate to 0, so no columns ever get
                    # read and every row's sum stays 0 (wrong result, not a
                    # crash). buffer_size > iteration (m) used to be skipped too,
                    # but dpu.c's output-write batching is now decoupled from m
                    # (a row loop with a running pending_rows flush, padded per
                    # -tasklet regions to keep MRAM writes 8-byte aligned) --
                    # buffer_size > m is correct now, just no longer skipped.
                    if buffer_size > vec_length:
                        continue
                    combos.append((thread, iteration, vec_length, buffer_size))

    parallel_sweep.run_parallel_sweep(
        combos, _run_combo, header, execution_output, PARALLEL_INSTANCES,
        kernel_name="Gemv", combo_key_fn=lambda c: ("MATVEC", "uint32_t") + c,
    )
