import subprocess
from pathlib import Path
import csv
from comment_line import *
from configuration import *
import os


os.chdir("..")
Path("./generated_headers").mkdir(parents=True, exist_ok=True)
Path("./output").mkdir(parents=True, exist_ok=True)
Path(custom_dma_asm_addr).mkdir(parents=True, exist_ok=True)

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

def duplicate(file_name, positions, count):
    f = open(file_name, "r")
    new_file_lines = []
    next_line_is_the_line = False

    for line in f:
        for position in positions:
            if position[0] in line and position[1] == True:
                canWriteLine = False
                for i in range(count):
                    new_file_lines.append(line)
        new_file_lines.append(line)
    f.close()
    f = open(file_name, "w")
    for line in new_file_lines:
        f.write(line)
    f.close()


createInitCaches()

comment_signature = {
    "dma": {
        dma_types[0]: [("ldma", True)],
        dma_types[1]: [("sdma", True)],
    },
}


### DMA
for thread in thread_range:
    for dma_type in dma_types:
        for dup_count in range(0, duplicate_count_max+1):
            dmaCreateAssemblyConfigurationHeader(dma_type)
            assembly_file_1 = dma_type + "_t" + str(thread) + "_d" + str(dup_count + 1) + ".s"
            assembly_file_0 = dma_type + "_t" + str(thread) + "_d0.s"
            subprocess.run(
                [
                    "dpu-upmem-dpurte-clang",
                    "-S",
                    "-O2",
                    "-DNR_TASKLETS=" + str(thread),
                    "./dpu/task.c",
                    "-o",
                    custom_dma_asm_addr + "/" + assembly_file_1,
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
                    custom_dma_asm_addr + "/" + assembly_file_0,
                ]
            )
            duplicate(
                custom_dma_asm_addr + "/" + assembly_file_1, comment_signature["dma"][dma_type], dup_count )
            comments(
                custom_dma_asm_addr + "/" + assembly_file_0, comment_signature["dma"][dma_type]
            )
