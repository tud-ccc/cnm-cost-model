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
    operation,
    data_type,
    rank_dims,
    dpu_dims,
    thread_dims,
    iter_dims,
    buffer_sizes,
):
    slot = parallel_sweep.get_slot()
    createConfigurationHeader(
        operation,
        data_type,
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
    "buffer_size",
    "latency",
    "correct",
]


def _run_combo(combo):
    operation, data_type, thread, iteration, buffer_size = combo

    rank_dims = [1]
    dpu_dims = [1]
    thread_dims = [thread]
    iter_dims = [iteration * buffer_size]
    buffer_sizes = [buffer_size]

    print(
        f"[slot {parallel_sweep.get_slot()}] Running operation={operation}, data_type={data_type}, "
        f"thread={thread}, iteration={iteration}, "
        f"buffer_size={buffer_size}",
        flush=True,
    )

    latency, correct = runKernelOnADPUS(
        operation,
        data_type,
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
    return [operation, data_type, thread, iteration, buffer_size, latency, correct]


if __name__ == "__main__":
    combos = [
        (operation, data_type, thread, iteration, buffer_size)
        for operation in operations_str
        for data_type in data_types
        for thread in thread_range
        for iteration in iterations
        for buffer_size in buffer_range
    ]
    parallel_sweep.run_parallel_sweep(
        combos, _run_combo, header, execution_output, PARALLEL_INSTANCES,
        kernel_name="ReduceOp", combo_key_fn=lambda c: c,
    )
