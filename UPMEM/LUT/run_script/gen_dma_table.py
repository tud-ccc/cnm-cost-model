import subprocess
from pathlib import Path
import csv
from configuration import *
import os

os.chdir("..")
Path("./generated_headers").mkdir(parents=True, exist_ok=True)
Path("./output").mkdir(parents=True, exist_ok=True)

header = ["operation", "data_type", "iteration", "cycle_count"]

raw_csv_file = open("./output/dma_raw.csv", "w", encoding="UTF8", newline="")
parsed_csv_file = open("./output/dma_parsed.csv", "w", encoding="UTF8", newline="")
raw_cv_writer = csv.writer(raw_csv_file)
parsed_cv_writer = csv.writer(parsed_csv_file)


def createConfigurationHeader(arr_size, iteration, iteration2, buffer_size, dma_type):
    f = open("./generated_headers/config.h", "w")
    f.write("#define ARR_SIZE " + arr_size + "\n")
    f.write("#define ITERATION " + iteration + "\n")
    f.write("#define ITERATION2 " + iteration2 + "\n")
    f.write("#define BUFFER_SIZE " + buffer_size + "\n")
    f.write("#define " + dma_type + " 1\n")
    f.write("#define INIT_VAR 1\n")
    f.write("#define T uint32_t\n")
    f.close()


def compileDPUCodeABinary(assembly_file, thread):
    subprocess.run(
        [
            "dpu-upmem-dpurte-clang",
            "-DNR_TASKLETS=" + str(thread),
            assembly_file,
            "-o",
            "./bin/dpu",
        ],
        stdout=subprocess.PIPE,
    )


def compileHostBinary(thread):
    subprocess.run(["make", "clean"], stdout=subprocess.PIPE)
    subprocess.run(["make", "NR_TASKLETS=" + str(thread)], stdout=subprocess.PIPE)


def getWithAndWithoutTime(thread, dma_type):
    baseAddr = "./dpu/dma/" + dma_type + "_t" + str(thread)
    assembly_file_1 = baseAddr + ".s"
    base_assembly = "./dpu/dma/" + dma_type + "_t" + str(thread) + "_0.s"
    compileDPUCodeABinary(assembly_file_1, thread)
    output_with = subprocess.run(["./bin/host"], stdout=subprocess.PIPE).stdout.decode(
        "utf-8"
    )
    compileDPUCodeABinary(base_assembly, thread)
    output_without = subprocess.run(
        ["./bin/host"], stdout=subprocess.PIPE
    ).stdout.decode("utf-8")
    return output_with, output_without


def process_operation(
    raw_execution_result, parsed_execution_result, dma_type, size, thread
):
    global buffer_size
    createConfigurationHeader(
        str(ARR_SIZE),
        str(iteration),
        str(1),
        str(size),
        dma_type,
    )
    compileHostBinary(thread)
    time_with, time_without = getWithAndWithoutTime(thread, dma_type)
    raw_execution_result.append([dma_type, size, thread, "WITH", time_with])
    raw_execution_result.append([dma_type, size, thread, "WITHOUT", time_without])
    try:
        time_with = float(time_with)
        op_time = (float(time_with) - float(time_without)) / (iteration)
        parsed_execution_result.append(
            [dma_type, size, thread, op_time, op_time * FREQ]
        )
    except ValueError:
        parsed_execution_result.append([dma_type, size, thread, -1, -1])


raw_execution_result = []
parsed_execution_result = []
for dma_type in dma_types:
    for thread in thread_range:
        size = dma_start_size
        while size < dma_max_size:
            print(dma_type, thread, size)
            process_operation(
                raw_execution_result,
                parsed_execution_result,
                dma_type,
                size,
                thread,
            )
            size *= 2


raw_cv_writer.writerow(header)
raw_cv_writer.writerows(raw_execution_result)

parsed_cv_writer.writerow(header)
parsed_cv_writer.writerows(parsed_execution_result)
