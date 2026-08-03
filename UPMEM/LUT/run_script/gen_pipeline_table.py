import subprocess
from pathlib import Path
import csv
from configuration import *
import os

os.chdir("..")
Path("./generated_headers").mkdir(parents=True, exist_ok=True)
Path("./output").mkdir(parents=True, exist_ok=True)

header = ["operation", "data_type", "iteration", "cycle_count"]

normal_raw_csv_file = open(
    "./output/pipeline_normal_raw.csv", "w", encoding="UTF8", newline=""
)
normal_parsed_csv_file = open(
    "./output/pipeline_normal_parsed.csv", "w", encoding="UTF8", newline=""
)
normal_raw_cv_writer = csv.writer(normal_raw_csv_file)
normal_parsed_cv_writer = csv.writer(normal_parsed_csv_file)

func_raw_csv_file = open(
    "./output/pipeline_func_raw.csv", "w", encoding="UTF8", newline=""
)
func_parsed_csv_file = open(
    "./output/pipeline_func_parsed.csv", "w", encoding="UTF8", newline=""
)
func_raw_cv_writer = csv.writer(func_raw_csv_file)
func_parsed_cv_writer = csv.writer(func_parsed_csv_file)


def createConfigurationHeader(
    arr_size, iteration, buffer_size, str_data_type, str_operation, INIT_VAR
):
    f = open("./generated_headers/config.h", "w")
    f.write("#define ARR_SIZE " + arr_size + "\n")
    f.write("#define ITERATION " + iteration + "\n")
    f.write("#define BUFFER_SIZE " + buffer_size + "\n")
    f.write("#define T " + str_data_type + "\n")
    f.write("#define " + str_operation + " 1\n")
    f.write("#define INIT_VAR " + INIT_VAR + "\n")
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


def getWithAndWithoutTime(thread, operation, data_type):
    baseAddr = (
        pipeline_asm_addr
        + "/"
        + operation
        + "/"
        + operation
        + "_"
        + data_type
        + "_t"
        + str(thread)
    )
    assembly_file_1 = baseAddr + ".s"
    base_assembly = baseAddr + "_0.s"
    compileDPUCodeABinary(assembly_file_1, thread)
    output_with = subprocess.run(["./bin/host"], stdout=subprocess.PIPE).stdout.decode(
        "utf-8"
    )
    compileDPUCodeABinary(base_assembly, thread)
    output_without = subprocess.run(
        ["./bin/host"], stdout=subprocess.PIPE
    ).stdout.decode("utf-8")
    return output_with, output_without


def getWithTime(thread, operation, data_type):
    baseAddr = (
        pipeline_asm_addr
        + "/"
        + operation
        + "/"
        + operation
        + "_"
        + data_type
        + "_t"
        + str(thread)
    )
    assembly_file_1 = baseAddr + ".s"
    compileDPUCodeABinary(assembly_file_1, thread)
    output_with = subprocess.run(["./bin/host"], stdout=subprocess.PIPE).stdout.decode(
        "utf-8"
    )
    return output_with


def getWithoutTime(thread, data_type):
    base_assembly = (
        pipeline_asm_addr + "/ADD/ADD_" + data_type + "_t" + str(thread) + "_0.s"
    )
    compileDPUCodeABinary(base_assembly, thread)
    output_without = subprocess.run(
        ["./bin/host"], stdout=subprocess.PIPE
    ).stdout.decode("utf-8")
    return output_without


def getCustomWithoutTime(thread, operation, data_type):
    base_assembly = (
        pipeline_asm_addr
        + "/"
        + operation
        + "/"
        + operation
        + "_"
        + data_type
        + "_t"
        + str(thread)
        + "_0.s"
    )
    compileDPUCodeABinary(base_assembly, thread)
    output_without = subprocess.run(
        ["./bin/host"], stdout=subprocess.PIPE
    ).stdout.decode("utf-8")
    return output_without


def process_operation(
    normal_raw_execution_result,
    normal_parsed_execution_result,
    func_raw_execution_result,
    func_parsed_execution_result,
    operation,
    thread,
    data_type,
):
    global buffer_size
    if operation == "MUL" and data_type == "uint32_t":
        init_val = INIT_VAL_MIN
        createConfigurationHeader(
            str(ARR_SIZE),
            str(iteration),
            str(buffer_size),
            str(data_type),
            operation,
            str(init_val),
        )
        compileHostBinary(thread)
        time_without = getWithoutTime(thread, data_type)
        while init_val < INIT_VAL_MAX:
            createConfigurationHeader(
                str(ARR_SIZE),
                str(iteration),
                str(buffer_size),
                str(data_type),
                operation,
                str(init_val),
            )
            compileHostBinary(thread)
            time_with = getWithTime(thread, operation, data_type)
            func_raw_execution_result.append(
                [operation, thread, data_type, init_val, "WITH", time_with]
            )
            func_raw_execution_result.append(
                [operation, thread, data_type, init_val, "WITHOUT", time_without]
            )

            op_time = (float(time_with) - float(time_without)) / (
                iteration * buffer_size
            )
            func_parsed_execution_result.append(
                [operation, thread, data_type, init_val, op_time, op_time * FREQ]
            )
            init_val *= 2
    else:
        createConfigurationHeader(
            str(ARR_SIZE),
            str(iteration),
            str(buffer_size),
            str(data_type),
            operation,
            str(128),
        )
        compileHostBinary(thread)
        time_with, time_without = 0, 0
        if operation in ["LOAD", "STORE"]:
            time_without = getCustomWithoutTime(thread, operation, data_type)
            time_with = getWithTime(thread, operation, data_type)
        else:
            time_with, time_without = getWithAndWithoutTime(
                thread, operation, data_type
            )

        normal_raw_execution_result.append(
            [operation, thread, data_type, "WITH", time_with]
        )
        normal_raw_execution_result.append(
            [operation, thread, data_type, "WITHOUT", time_without]
        )

        op_time = (float(time_with) - float(time_without)) / (iteration * buffer_size)
        normal_parsed_execution_result.append(
            [operation, thread, data_type, op_time, op_time * FREQ]
        )


normal_raw_execution_result = []
normal_parsed_execution_result = []
func_raw_execution_result = []
func_parsed_execution_result = []
for operation in operations_str:
    for thread in thread_range:
        for data_type in data_types:
            print(operation, thread, data_type)
            process_operation(
                normal_raw_execution_result,
                normal_parsed_execution_result,
                func_raw_execution_result,
                func_parsed_execution_result,
                operation,
                thread,
                data_type,
            )


normal_raw_cv_writer.writerow(header)
normal_raw_cv_writer.writerows(normal_raw_execution_result)

normal_parsed_cv_writer.writerow(header)
normal_parsed_cv_writer.writerows(normal_parsed_execution_result)

func_raw_cv_writer.writerow(header)
func_raw_cv_writer.writerows(func_raw_execution_result)

func_parsed_cv_writer.writerow(header)
func_parsed_cv_writer.writerows(func_parsed_execution_result)
