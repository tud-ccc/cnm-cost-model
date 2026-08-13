import os
import subprocess
import csv
from create_headers import *
from functools import reduce


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

    createConfigurationHeader(
        "TEMP",
        "uint32_t",
        rank_dims,
        dpu_dims,
        thread_dims,
        iter_dims,
        buffer_sizes,
    )
    total_thread_count = reduce(lambda x, y: x * y, thread_dims)
    subprocess.run(["make", "clean"], stdout=subprocess.PIPE)
    subprocess.run(
        ["make", "NR_TASKLETS=" + str(total_thread_count)], stdout=subprocess.PIPE
    )
    execution = subprocess.run(["./bin/host"], stdout=subprocess.PIPE)
    return execution.stdout.decode("utf-8").strip(), execution.returncode == 0


os.chdir("../")


def parse_latency(string):
    if string.replace(".", "").isnumeric():
        return float(string)
    else:
        return "NA"


result_file = open(execution_output, "w")
csv_file_write = csv.writer(result_file)
header = [
    "operation",
    "data_type",
    "thread",
    "iteration",
    "vec_length",
    "n_length",
    "p_length",
    "buffer_size",
    "latency",
    "correct",
]
csv_file_write.writerow(header)

for thread in thread_range:
    for iteration in iterations:
        for vec_length in vec_lengths:
            for n_length in n_lengths:
                for p_length in p_lengths:
                    for buffer_size in buffer_range:
                        # dpu.c assumes K (vec_length), N (n_length) and P
                        # (p_length) are each exact multiples of
                        # BUFFER_COUNT -- no remainder-tile handling yet.
                        # buffer_size > any of them, or any not evenly
                        # divisible, makes num_k_chunks/num_n_tiles truncate
                        # to 0 or leave a partial tile uncomputed in one of
                        # the two matmul_pass phases (wrong result, not a
                        # crash), so those combinations are skipped here
                        # rather than silently producing bad data.
                        if (
                            buffer_size > vec_length
                            or buffer_size > n_length
                            or buffer_size > p_length
                            or vec_length % buffer_size != 0
                            or n_length % buffer_size != 0
                            or p_length % buffer_size != 0
                        ):
                            continue

                        print(
                            f"Running thread={thread}, iteration={iteration}, "
                            f"vec_length={vec_length}, n_length={n_length}, "
                            f"p_length={p_length}, buffer_size={buffer_size}",
                            flush=True,
                        )

                        rank_dims = [1]
                        dpu_dims = [1]
                        thread_dims = [thread]
                        iter_dims = [iteration, vec_length, n_length, p_length]
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
                        csv_file_write.writerow(
                            [
                                "2MM",
                                "uint32_t",
                                thread,
                                iteration,
                                vec_length,
                                n_length,
                                p_length,
                                buffer_size,
                                latency,
                                correct,
                            ]
                        )
result_file.close()
