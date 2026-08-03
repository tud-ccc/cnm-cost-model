#!/usr/bin/env python3
"""Ours -- drives Samsung/runner/bench_kernels (built via Samsung/runner/
SConstruct against PIMSimulator's own libdramsim2 library) to regenerate
the ground-truth latency CSVs, without touching PIMSimulator's own vendored
test files.

Runs with PIMSimulator/ as the working directory, since
MultiChannelMemorySystem resolves its .ini config paths relative to cwd.

Usage:
    python3 run_code.py add
    python3 run_code.py relu
    python3 run_code.py gemv   # needs data/gemv/*.npy for the swept sizes
"""

import subprocess
import sys
from pathlib import Path

RUNNER_DIR = Path(__file__).resolve().parent
PIM_SIM_DIR = RUNNER_DIR.parent / "PIMSimulator"
BINARY_PATH = RUNNER_DIR / "bench_kernels"
RESULTS_DIR = RUNNER_DIR / "results"

KERNELS = ("add", "relu", "gemv")


def run(kernel: str) -> Path:
    if kernel not in KERNELS:
        raise SystemExit(f"unknown kernel {kernel!r} (expected one of {KERNELS})")
    if not BINARY_PATH.exists():
        raise SystemExit(f"{BINARY_PATH} not found -- build it first: (cd {RUNNER_DIR} && scons)")

    RESULTS_DIR.mkdir(parents=True, exist_ok=True)
    out_path = RESULTS_DIR / f"res_{kernel}_latency.csv"

    subprocess.run(
        [str(BINARY_PATH), kernel, str(out_path)],
        cwd=PIM_SIM_DIR,  # relative .ini paths resolve from here
        check=True,
    )
    return out_path


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"usage: {sys.argv[0]} <{'|'.join(KERNELS)}>")
    result_path = run(sys.argv[1])
    print(f"Wrote {result_path}")
