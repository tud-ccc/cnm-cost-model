import os
import subprocess
import csv
from create_headers import *
from functools import reduce


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

result_file = open(execution_output, "w")
csv_file_write = csv.writer(result_file)
header = [
    "operation",
    "data_type",
    "thread",
    "ni_length",
    "nk_length",
    "nj_length",
    "nm_length",
    "nl_length",
    "buffer_size",
    "latency",
    "correct",
]
csv_file_write.writerow(header)

for thread in thread_range:
    for ni_length in ni_lengths:
        for nk_length in nk_lengths:
            for nj_length in nj_lengths:
                for nm_length in nm_lengths:
                    for nl_length in nl_lengths:
                        for buffer_size in buffer_range:
                            # dpu.c assumes NK, NJ, NM and (per-thread) NL
                            # are each exact multiples of BUFFER_COUNT -- no
                            # remainder-tile handling yet (NI has no such
                            # constraint: it's only ever a row-loop trip
                            # count, never chunked by BUFFER_COUNT). Skipped
                            # here rather than silently producing bad data,
                            # same as Gemm/2mm.
                            if (
                                buffer_size > nk_length
                                or buffer_size > nj_length
                                or buffer_size > nm_length
                                or buffer_size > nl_length
                                or nk_length % buffer_size != 0
                                or nj_length % buffer_size != 0
                                or nm_length % buffer_size != 0
                                or nl_length % buffer_size != 0
                            ):
                                continue

                            print(
                                f"Running thread={thread}, ni={ni_length}, "
                                f"nk={nk_length}, nj={nj_length}, "
                                f"nm={nm_length}, nl={nl_length}, "
                                f"buffer_size={buffer_size}",
                                flush=True,
                            )

                            rank_dims = [1]
                            dpu_dims = [1]
                            thread_dims = [thread]
                            iter_dims = [
                                ni_length,
                                nk_length,
                                nj_length,
                                nm_length,
                                nl_length,
                            ]
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
                                    "3MM",
                                    "uint32_t",
                                    thread,
                                    ni_length,
                                    nk_length,
                                    nj_length,
                                    nm_length,
                                    nl_length,
                                    buffer_size,
                                    latency,
                                    correct,
                                ]
                            )
result_file.close()
