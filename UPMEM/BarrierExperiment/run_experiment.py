import csv
import subprocess
from pathlib import Path

FREQ_PER_MS = 350000  # 350 MHz -> cycles per millisecond
VEC_SIZE = 32  # fixed "before"/"after" vecadd size, same shape every run


def write_config(iteration, barrier_count, do_barrier):
    lines = [
        f"#define ITERATION {iteration}",
        f"#define BARRIER_COUNT {barrier_count}",
        f"#define VEC_SIZE {VEC_SIZE}",
        "#define T uint32_t",
    ]
    if do_barrier:
        lines.append("#define DO_BARRIER 1")
    with open("generated_headers/config.h", "w") as f:
        f.write("\n".join(lines) + "\n")


def build(thread):
    subprocess.run(["make", "clean"], stdout=subprocess.PIPE)
    subprocess.run(["make", f"NR_TASKLETS={thread}"], stdout=subprocess.PIPE)


def run():
    out = subprocess.run(["./bin/host"], stdout=subprocess.PIPE)
    return float(out.stdout.decode("utf-8").strip())


def measure(thread, iteration, barrier_count):
    # "with": barrier_count real barrier_wait() calls per outer iteration.
    write_config(iteration, barrier_count, do_barrier=True)
    build(thread)
    time_with = run()

    # "without": identical loop/vecadd structure, but the barrier block is
    # never compiled in at all -- isolates the barrier's own marginal cost
    # from the surrounding loop/compute overhead, same differencing trick as
    # DmaScalingExperiment's DO_DMA on/off.
    write_config(iteration, barrier_count, do_barrier=False)
    build(thread)
    time_without = run()

    op_time_ms = (time_with - time_without) / (iteration * barrier_count)
    return op_time_ms * FREQ_PER_MS


Path("output").mkdir(parents=True, exist_ok=True)
output_path = Path("output/barrier_cost.csv")

print("=== Barrier cost vs thread count (balanced load: every tasklet runs the ===")
print("=== same before/after vecadd, so all arrive at the barrier together)   ===")
print("thread,cycles_per_barrier_call")

with open(output_path, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["thread", "vec_size", "iteration", "barrier_count",
                      "cycles_per_barrier_call"])
    for thread in [1, 2, 4, 8, 16, 24]:
        iteration, barrier_count = 512, 8
        cycles = measure(thread, iteration=iteration, barrier_count=barrier_count)
        print(f"{thread},{cycles:.2f}", flush=True)
        writer.writerow([thread, VEC_SIZE, iteration, barrier_count, f"{cycles:.2f}"])
        f.flush()
