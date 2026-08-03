#!/usr/bin/env python3
"""Compares kernels.Hbmpim's predictions against the ground-truth CSVs
Samsung/runner/ produces (Samsung/runner/results/res_{add,relu,gemv}_
latency.csv) -- the HBM-PIM counterpart to this file's UPMEM siblings
(benchmarks_vecadd.py etc.), just reading real-simulator output instead of
real-hardware output.

Each output CSV keeps the runner's own columns unchanged and appends
`predicted` and `error_percent`.

The runner's CSVs store raw sweep parameters (data dimensions, or -- for
relu -- the sweep index i where dim = 8192*16*i), not the internal tile
counts kernels.Hbmpim's calc functions expect. The conversion mirrors
PIMKernel's real internal math:
  - add/relu: PIMKernel::executeEltwise burst-packs the dim first (/16,
    NumpyBurstType), then divides by num_banks * num_pim_chans *
    num_pim_ranks * num_grf to get the tile count.
  - gemv: PIMKernel::executeGemv computes
    `ot = ceil((out_dim / num_total_pim_blocks) / num_grfB)` and
    `in = ceil(ceil(in_dim / 16) / num_grfA)`.
Constants (NUM_BANKS=16, NUM_PIM_BLOCKS=8, num_pim_chans=64, num_pim_ranks=1,
num_grf=num_grfA=num_grfB=8) are read from
Samsung/PIMSimulator/ini/HBM2_samsung_2M_16B_x64.ini and PIMKernel's
constructor (`PIMKernel(pim_mem_, 64, 1)` in PIMBenchTestCases.h).

Usage:
    python3 benchmarks_hbmpim.py
"""

from __future__ import annotations

import csv
import math
from pathlib import Path

from kernels.Hbmpim import calcAdd, calcGemv, calcRelu

_RUNNER_RESULTS = Path(__file__).resolve().parents[1] / "Samsung" / "runner" / "results"
_OUTPUT_DIR = Path(__file__).resolve().parent / "output"

# NumpyBurstType packs this many elements per burst (applied before any
# tile-count division, for both the add/relu and gemv paths).
_BURST_WIDTH = 16

# num_banks(16) * num_pim_chans(64) * num_pim_ranks(1) * num_grf(8).
_ELTWISE_TILE_DIVISOR = 16 * 64 * 1 * 8  # = 8192

# num_pim_blocks(8) * num_pim_chans(64) * num_pim_ranks(1).
_GEMV_TOTAL_PIM_BLOCKS = 8 * 64 * 1  # = 512
_NUM_GRF = 8


def _eltwise_tile_count(dim: int) -> int:
    burst_dim = math.ceil(dim / _BURST_WIDTH)
    return burst_dim // _ELTWISE_TILE_DIVISOR


def _error_percent(predicted: float, measured: float) -> float:
    return ((predicted - measured) / measured) * 100


def _compare_add(in_path: Path, out_path: Path) -> None:
    with open(in_path, newline="") as f:
        rows = list(csv.reader(f))
    with open(out_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["dim", "cycle", "predicted", "error_percent"])
        for dim_str, cycle_str in rows:
            dim, measured = int(dim_str), float(cycle_str)
            tile_count = _eltwise_tile_count(dim)
            predicted = calcAdd(tile_count)
            writer.writerow([dim, cycle_str, f"{predicted:.2f}", f"{_error_percent(predicted, measured):.2f}"])


def _compare_relu(in_path: Path, out_path: Path) -> None:
    with open(in_path, newline="") as f:
        rows = list(csv.reader(f))
    with open(out_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["i", "cycle", "predicted", "error_percent"])
        for i_str, cycle_str in rows:
            i, measured = int(i_str), float(cycle_str)
            # runner's bench_kernels.cpp builds relu's dim the same way as
            # add's (dim = 8192*16*i), just writes `i` instead of `dim` as
            # the CSV's first column -- reconstruct dim to get the same
            # real tile count executeEltwise would compute.
            dim = 8192 * 16 * i
            tile_count = _eltwise_tile_count(dim)
            predicted = calcRelu(tile_count)
            writer.writerow([i, cycle_str, f"{predicted:.2f}", f"{_error_percent(predicted, measured):.2f}"])


def _compare_gemv(in_path: Path, out_path: Path) -> None:
    with open(in_path, newline="") as f:
        rows = list(csv.reader(f))
    with open(out_path, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["out", "in", "cycle", "predicted", "error_percent"])
        for out_str, in_str, cycle_str in rows:
            out_dim, in_dim, measured = int(out_str), int(in_str), float(cycle_str)
            output_tile = math.ceil((out_dim / _GEMV_TOTAL_PIM_BLOCKS) / _NUM_GRF)
            in_bursts = math.ceil(in_dim / _BURST_WIDTH)
            input_tile = math.ceil(in_bursts / _NUM_GRF)
            predicted = calcGemv(output_tile, input_tile)
            writer.writerow(
                [out_dim, in_dim, cycle_str, f"{predicted:.2f}", f"{_error_percent(predicted, measured):.2f}"]
            )


def main() -> None:
    _OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    jobs = [
        ("add", _compare_add),
        ("relu", _compare_relu),
        ("gemv", _compare_gemv),
    ]
    for name, compare_fn in jobs:
        in_path = _RUNNER_RESULTS / f"res_{name}_latency.csv"
        if not in_path.exists():
            print(f"Skipping {name}: {in_path} not found (run Samsung/runner/run_code.py {name} first)")
            continue
        out_path = _OUTPUT_DIR / f"hbmpim_{name}_comparison.csv"
        compare_fn(in_path, out_path)
        print(f"Wrote {out_path}")


if __name__ == "__main__":
    main()
