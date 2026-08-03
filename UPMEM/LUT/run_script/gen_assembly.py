import subprocess
from pathlib import Path
import csv
from comment_line import *
from configuration import *
import os


os.chdir("..")
Path("./generated_headers").mkdir(parents=True, exist_ok=True)
Path("./output").mkdir(parents=True, exist_ok=True)
Path(pipeline_asm_addr).mkdir(parents=True, exist_ok=True)
Path(dma_asm_addr).mkdir(parents=True, exist_ok=True)

for operation in operations_str:
    Path(pipeline_asm_addr + "/" + operation).mkdir(parents=True, exist_ok=True)


def createInitCaches():
    f = open("./generated_headers/init_vec.h", "w")
    f.write('#include "config.h"\n')

    f.write(
        "void init_vec(T *bufferC, T *bufferA, T *bufferB){ for (int i = 0; i < BUFFER_SIZE; i++) bufferC[i] = bufferA[i] = bufferB[i] = 1;}\n"
    )
    f.close()


def pipelineCreateAssemblyConfigurationHeader(str_data_type, str_operation):
    f = open("./generated_headers/config.h", "w")
    f.write("#define T " + str_data_type + "\n")
    f.write("#define " + str_operation + " 1\n")
    f.write("#define PIPELINE 1\n")
    f.close()


def dmaCreateAssemblyConfigurationHeader(dma_type):
    f = open("./generated_headers/config.h", "w")
    f.write("#define T uint32_t\n")
    f.write("#define DMA 1\n")
    f.write("#define " + dma_type + " 1\n")
    f.close()


createInitCaches()

comment_signature = {
    "pipeline": {
        "uint16_t": {
            # NOTE: line numbers below were fixed from a stale 39/41 (off by
            # one vs. the current dpu/task.c, where ADD's statement is on
            # line 38 and SUB's is on line 40). With the stale references,
            # comments() never matched, so the WITHOUT baseline silently
            # ended up byte-identical to WITH for every data type's plain
            # add/sub instruction (only uint64_t's independent
            # substring-matched "addc"/"subc" marker still worked).
            "ADD": [("task.c:38:31", False), ("task.c:38:18", False)],
            "SUB": [("task.c:40:31", False), ("task.c:40:18", False)],
            "MUL": [("Not needed", False)],
            "DIV": [("Not needed", False)],
            "LOAD": [("task.c:47:20", False)],
            "STORE": [("task.c:49:18", False)],
        },
        "uint32_t": {
            "ADD": [("task.c:38:31", False)],
            "SUB": [("task.c:40:31", False)],
            "MUL": [("Not needed", False)],
            "DIV": [("Not needed", False)],
            "LOAD": [("task.c:47:20", False)],
            "STORE": [("task.c:49:18", False)],
        },
        "uint64_t": {
            "ADD": [("task.c:38:31", False), ("addc", True)],
            "SUB": [("task.c:40:31", False), ("subc", True)],
            "MUL": [("Not needed", False)],
            "DIV": [("not needed", False)],
            "LOAD": [("task.c:47:20", False)],
            "STORE": [("task.c:49:18", False)],
        },
        "float": {
            "ADD": [("task.c:38:31", False)],
            "SUB": [("task.c:40:31", False)],
            "MUL": [("Not needed", False)],
            "DIV": [("Not needed", False)],
            "LOAD": [("task.c:47:20", False)],
            "STORE": [("task.c:49:18", False)],
        },
        "double": {
            "ADD": [("task.c:38:31", False)],
            "SUB": [("task.c:40:31", False)],
            "MUL": [("Not needed", False)],
            "DIV": [("Not needed", False)],
            "LOAD": [("task.c:47:20", False)],
            "STORE": [("task.c:49:18", False)],
        },
    },
    "dma": {
        dma_types[0]: [("ldma", True)],
        dma_types[1]: [("sdma", True)],
    },
}

for operation in operations_str:
    for data_type in data_types:
        for thread in thread_range:
            pipelineCreateAssemblyConfigurationHeader(data_type, operation)
            assembly_file_1 = operation + "_" + data_type + "_t" + str(thread) + ".s"
            assembly_file_0 = operation + "_" + data_type + "_t" + str(thread) + "_0.s"
            subprocess.run(
                [
                    "dpu-upmem-dpurte-clang",
                    "-S",
                    "-O2",
                    "-DNR_TASKLETS=" + str(thread),
                    "./dpu/task.c",
                    "-o",
                    pipeline_asm_addr + "/" + operation + "/" + assembly_file_1,
                ]
            )
            subprocess.run(
                [
                    "dpu-upmem-dpurte-clang",
                    "-S",
                    "-O2",
                    "-DNR_TASKLETS=" + str(thread),
                    "./dpu/task.c",
                    "-o",
                    pipeline_asm_addr + "/" + operation + "/" + assembly_file_0,
                ]
            )
            comments(
                pipeline_asm_addr + "/" + operation + "/" + assembly_file_0,
                comment_signature["pipeline"][data_type][operation],
            )

### DMA
for thread in thread_range:
    for dma_type in dma_types:
        dmaCreateAssemblyConfigurationHeader(dma_type)
        assembly_file_1 = dma_type + "_t" + str(thread) + ".s"
        assembly_file_0 = dma_type + "_t" + str(thread) + "_0.s"
        subprocess.run(
            [
                "dpu-upmem-dpurte-clang",
                "-S",
                "-O2",
                "-DNR_TASKLETS=" + str(thread),
                "./dpu/task.c",
                "-o",
                dma_asm_addr + "/" + assembly_file_1,
            ]
        )
        subprocess.run(
            [
                "dpu-upmem-dpurte-clang",
                "-S",
                "-O2",
                "-DNR_TASKLETS=" + str(thread),
                "./dpu/task.c",
                "-o",
                dma_asm_addr + "/" + assembly_file_0,
            ]
        )
        comments(
            dma_asm_addr + "/" + assembly_file_0, comment_signature["dma"][dma_type]
        )
