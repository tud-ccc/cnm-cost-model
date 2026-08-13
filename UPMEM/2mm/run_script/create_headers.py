from configuration import *


def createCommonStructs(thread_dims, iter_dims, buffer_sizes):
    f = open(common_struct_file_addr, "w")
    f.write('#include "config.h"\n')
    f.write(
        "typedef struct{\n"
        + "\tuint32_t BUFFER_SIZES["
        + str(len(buffer_sizes))
        + "];\n"
        + "\tuint32_t THREADS["
        + str(len(thread_dims))
        + "];\n"
        + "\tuint32_t ITERS["
        + str(len(iter_dims))
        + "];\n"
        + "} dpu_arguments_t;\n"
    )


def createGenMap(rank_dims, dpu_dims, thread_dims, iter_dims, buffer_sizes):
    f = open(gen_map_file_addr, "w")
    f.write("#include <stdint.h>\n")
    f.write(
        "uint32_t MAP_RANK_DIMS[] = {" + ", ".join(str(x) for x in rank_dims) + "};\n"
    )
    f.write(
        "uint32_t MAP_DPU_DIMS[] = {" + ", ".join(str(x) for x in dpu_dims) + "};\n"
    )
    f.write(
        "uint32_t MAP_THREAD_DIMS[] = {"
        + ", ".join(str(x) for x in thread_dims)
        + "};\n"
    )
    f.write(
        "uint32_t MAP_ITER_DIMS[] = {" + ", ".join(str(x) for x in iter_dims) + "};\n"
    )
    f.write(
        "uint32_t MAP_BUFFER_SIZES[] = {"
        + ", ".join(str(x) for x in buffer_sizes)
        + "};\n"
    )


def createConfig(data_type, dim_count, operation):
    f = open(config_header_file_addr, "w")
    f.write("#define LOG " + ("1" if log else "0") + "\n")
    f.write("#define SIM " + ("1" if sim else "0") + "\n")
    f.write("#define T " + data_type + "\n")
    f.write("#define DIM_COUNT " + str(dim_count) + "\n")
    f.write("#define N_WARM_UP " + ("0" if sim else str(nwarm) + "\n"))
    f.write("#define N_REP " + ("1" if sim else str(nrep) + "\n"))
    f.write("#define SIM " + ("1" if sim else "0" + "\n"))
    f.write("#define INIT_DATA 1\n")

    f.write("#define " + operation + " 1\n")


def createConfigurationHeader(
    operation,
    data_type,
    rank_dims,
    dpu_dims,
    thread_dims,
    iter_dims,
    buffer_sizes,
):
    createCommonStructs(thread_dims, iter_dims, buffer_sizes)
    createGenMap(rank_dims, dpu_dims, thread_dims, iter_dims, buffer_sizes)
    createConfig(data_type, len(rank_dims), operation)
