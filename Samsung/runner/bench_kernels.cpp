// Ours -- not part of Samsung's PIMSimulator. Links against its
// libdramsim2 library and reuses PIMBenchTestCases.h's classes
// (EltPIMBenchTest/ActPIMBenchTest/GemvPIMBenchTest) directly, CLI-driven
// instead of via gtest TEST_F bodies.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "tests/PIMBenchTestCases.h"

using namespace DRAMSim;

static void runAdd(const char *out_path)
{
    FILE *fptr = fopen(out_path, "w");
    for (uint64_t i = 1; i <= 128; i += 1)
    {
        uint64_t dim = 8192 * 16 * i;
        EltPIMBenchTest test(KernelType::ADD, 1, (unsigned)dim, (unsigned)dim);
        uint64_t cycle = test.measureCycle(true);
        fprintf(fptr, "%llu,%llu\n", (unsigned long long)dim, (unsigned long long)cycle);
        fflush(fptr);
    }
    fclose(fptr);
}

static void runRelu(const char *out_path)
{
    FILE *fptr = fopen(out_path, "w");
    for (uint64_t i = 1; i <= 128; i += 1)
    {
        uint64_t dim = 8192 * 16 * i;
        ActPIMBenchTest test(KernelType::RELU, 1, (unsigned)dim, (unsigned)dim);
        uint64_t cycle = test.measureCycle(true);
        fprintf(fptr, "%llu,%llu\n", (unsigned long long)i, (unsigned long long)cycle);
        fflush(fptr);
    }
    fclose(fptr);
}

static void runGemv(const char *out_path)
{
    // GemvPIMBenchTest's DataDim loads weight/input .npy files keyed by
    // exact (out,in) size from data/gemv/ -- only a few sizes are cached
    // there; missing ones need data/gemv/gen_gemv.py run first.
    FILE *fptr = fopen(out_path, "w");
    for (int i = 128; i <= 65536; i *= 2)
    {
        for (int j = 128; j <= 65536; j *= 2)
        {
            GemvPIMBenchTest test(KernelType::GEMV, 1, i, j);
            uint64_t cycle = test.measureCycle(true);
            fprintf(fptr, "%d,%d,%llu\n", i, j, (unsigned long long)cycle);
            fflush(fptr);
        }
    }
    fclose(fptr);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <add|relu|gemv> <output.csv>\n", argv[0]);
        return 1;
    }
    std::string kernel = argv[1];
    std::string out_path = argv[2];

    if (kernel == "add")
        runAdd(out_path.c_str());
    else if (kernel == "relu")
        runRelu(out_path.c_str());
    else if (kernel == "gemv")
        runGemv(out_path.c_str());
    else
    {
        fprintf(stderr, "unknown kernel '%s' (expected add|relu|gemv)\n", kernel.c_str());
        return 1;
    }
    return 0;
}
