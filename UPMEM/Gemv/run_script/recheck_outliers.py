import os
import subprocess
import sys

sys.path.insert(0, ".")
from create_headers import *

CONFIGS = [
    (1, 16, 16, 16),
    (1, 16, 32, 4),
    (1, 16, 32, 8),
    (1, 16, 128, 4),
    (1, 16, 128, 32),
    (1, 16, 128, 64),
    (1, 16, 256, 128),
    (1, 16, 1024, 4),
    (1, 16, 1024, 16),
    (1, 16, 1024, 32),
    (1, 16, 2048, 128),
    (1, 32, 16, 4),
    (1, 32, 16, 8),
    (1, 32, 32, 4),
    (1, 32, 32, 8),
    (1, 32, 64, 4),
    (1, 32, 64, 32),
    (1, 32, 1024, 64),
    (1, 32, 1024, 128),
    (1, 32, 1024, 256),
    (1, 64, 16, 4),
    (1, 64, 16, 8),
    (1, 64, 32, 4),
    (1, 64, 32, 8),
    (1, 128, 32, 4),
]


def run(thread, m, k, buf):
    createConfigurationHeader(
        "TEMP", "uint32_t", [1], [1], [thread], [m, k], [buf]
    )
    subprocess.run(["make", "clean"], stdout=subprocess.PIPE)
    subprocess.run(["make", "NR_TASKLETS=" + str(thread)], stdout=subprocess.PIPE)
    r = subprocess.run(["./bin/host"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return r.stdout.decode().strip(), "PASSED" in r.stderr.decode()


os.chdir("..")

print("thread,iteration,vec_length,buffer_size,trial1,trial2,trial3", flush=True)
for thread, m, k, buf in CONFIGS:
    trials = []
    for _ in range(3):
        out, correct = run(thread, m, k, buf)
        trials.append(out if correct else "FAULT")
    print(f"{thread},{m},{k},{buf},{trials[0]},{trials[1]},{trials[2]}", flush=True)
