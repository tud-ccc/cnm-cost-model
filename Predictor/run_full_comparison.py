#!/usr/bin/env python3
"""Runs every kernel's Path A (hand-traced from real compiled assembly) and
Path B (hand-transcribed from the xDSL/CnmGen-generated code) cost-model
comparison against real hardware measurements, then prints and plots a
summary -- meant to be regenerated whenever the underlying kernels or cost
model change, for use in the paper's own accuracy table/figure.

Usage:
    python3 run_full_comparison.py                  # run everything
    python3 run_full_comparison.py --only Hst,Sel    # just these kernels
    python3 run_full_comparison.py --skip-run        # re-summarize/replot
                                                      # the CSVs already in
                                                      # output/, without
                                                      # re-running any
                                                      # benchmark

Gemv and Gemm's full sweeps are the slow part (thousands of rows each,
Simulator work per row) -- expect this to take several minutes end to end
with both included.
"""

from __future__ import annotations

import argparse
import contextlib
import csv
import importlib
import io
import json
import sys
from pathlib import Path

import plot_residual_comparison
import plot_scatter_comparison

PREDICTOR_DIR = Path(__file__).resolve().parent
CSV_DIR = PREDICTOR_DIR / "output"
OUTPUT_DIR = CSV_DIR / "full_comparison"

# (display name, Path A module, Path B module, Path A csv, Path B csv)
KERNELS = [
    ("VectorOp", "benchmarks_vecadd", "benchmarks_vecadd_xdsl",
     "vecadd_comparison.csv", "vecadd_comparison_xdsl.csv"),
    ("ReduceOp", "benchmarks_reduceop", "benchmarks_reduceop_xdsl",
     "reduceop_comparison.csv", "reduceop_comparison_xdsl.csv"),
    ("Scan", "benchmarks_scan", "benchmarks_scan_xdsl",
     "scan_comparison.csv", "scan_comparison_xdsl.csv"),
    ("Hst", "benchmarks_hst", "benchmarks_hst_xdsl",
     "hst_comparison.csv", "hst_comparison_xdsl.csv"),
    ("Sel", "benchmarks_sel", "benchmarks_sel_xdsl",
     "sel_comparison.csv", "sel_comparison_xdsl.csv"),
    ("Gemv", "benchmarks_gemv", "benchmarks_gemv_xdsl",
     "gemv_comparison.csv", "gemv_comparison_xdsl.csv"),
    ("Gemm", "benchmarks_gemm", "benchmarks_gemm_xdsl",
     "gemm_comparison.csv", "gemm_comparison_xdsl.csv"),
]

# Reference categorical palette (blue/orange), validated colorblind-safe as
# an adjacent pair -- see the dataviz skill's references/palette.md.
COLOR_PATH_A = "#2a78d6"
COLOR_PATH_B = "#eb6834"


def _run_benchmark(module_name: str) -> None:
    """Runs one benchmark module's main(), suppressing its own per-row
    stdout spam (each writes thousands of `print(line)` calls) so only this
    script's own summary reaches the console."""
    module = importlib.import_module(module_name)
    with contextlib.redirect_stdout(io.StringIO()):
        module.main()


def _stats(csv_path: Path) -> dict:
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    errs = [abs(float(r["error_percent"])) for r in rows]
    return {
        "n": len(errs),
        "mean_abs_error": sum(errs) / len(errs),
        "worst_case": max(errs),
        "best_case": min(errs),
    }


def _load_pairs(csv_path: Path) -> tuple[list[float], list[float]]:
    """(measured, predicted) latencies for the scatter plot -- every
    comparison CSV has these two columns regardless of what else varies
    (operation/data_type/vec_length/n_length differ per kernel)."""
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    measured = [float(r["measured"]) for r in rows]
    predicted = [float(r["predicted"]) for r in rows]
    return measured, predicted


def _load_residuals(csv_path: Path) -> tuple[list[float], list[float]]:
    """(measured, signed error_percent) for the residual plot -- signed
    (not abs) so over- vs under-prediction stays visible, e.g. Gemm's
    xDSL path drifting one direction at high latency, not just "off"."""
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    measured = [float(r["measured"]) for r in rows]
    error_percent = [float(r["error_percent"]) for r in rows]
    return measured, error_percent


def _print_summary(results: dict) -> None:
    header = f"{'Kernel':<10} {'Path':<8} {'n':>6} {'average':>9} {'worst':>9} {'best':>9}"
    rule = "-" * len(header)
    print()
    print(header)
    print(rule)
    for name, r in results.items():
        for path_label, key in (("Path A", "path_a"), ("Path B", "path_b")):
            s = r[key]
            print(
                f"{name:<10} {path_label:<8} {s['n']:>6} "
                f"{s['mean_abs_error']:>8.2f}% {s['worst_case']:>8.2f}% {s['best_case']:>8.2f}%"
            )
    print(rule)
    print()


def _save_summary(results: dict) -> Path:
    out_path = OUTPUT_DIR / "summary.json"
    out_path.write_text(json.dumps(results, indent=2))
    return out_path


def _plot(results: dict) -> tuple[Path, Path, Path]:
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import numpy as np

    names = list(results.keys())
    path_a_mean = [results[n]["path_a"]["mean_abs_error"] for n in names]
    path_b_mean = [results[n]["path_b"]["mean_abs_error"] for n in names]
    path_a_worst = [results[n]["path_a"]["worst_case"] for n in names]
    path_b_worst = [results[n]["path_b"]["worst_case"] for n in names]

    x = np.arange(len(names))
    width = 0.35

    fig, ax = plt.subplots(figsize=(10, 5.5))

    ax.bar(x - width / 2, path_a_mean, width, label="Path A (real assembly)", color=COLOR_PATH_A)
    ax.bar(x + width / 2, path_b_mean, width, label="Path B (xDSL-generated)", color=COLOR_PATH_B)

    # Worst-case markers on top of each bar -- a thin tick, not a second bar,
    # so the chart stays one axis/one job (mean magnitude) with worst-case as
    # a secondary annotation rather than a competing series.
    ax.scatter(
        x - width / 2, path_a_worst, marker="_", s=220, linewidths=2,
        color="#0b0b0b", zorder=3, label="Worst case (either path)",
    )
    ax.scatter(x + width / 2, path_b_worst, marker="_", s=220, linewidths=2, color="#0b0b0b", zorder=3)

    ax.set_ylabel("Absolute error vs. real hardware (%)")
    ax.set_title("Cost-model prediction error: real-assembly-derived vs xDSL-generated code")
    ax.set_xticks(x)
    ax.set_xticklabels(names)
    ax.legend(frameon=False)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    fig.tight_layout()

    png_path = OUTPUT_DIR / "comparison_plot.png"
    pdf_path = OUTPUT_DIR / "comparison_plot.pdf"
    fig.savefig(png_path, dpi=200)
    fig.savefig(pdf_path)
    plt.close(fig)

    plot_data = {
        "kernels": names,
        "path_a_mean_abs_error": path_a_mean,
        "path_b_mean_abs_error": path_b_mean,
        "path_a_worst_case": path_a_worst,
        "path_b_worst_case": path_b_worst,
    }
    data_path = OUTPUT_DIR / "plot_data.json"
    data_path.write_text(json.dumps(plot_data, indent=2))

    return png_path, pdf_path, data_path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        "--only",
        help="comma-separated kernel display names to run (default: all 7)",
        default=None,
    )
    parser.add_argument(
        "--skip-run",
        action="store_true",
        help="don't re-run any benchmark -- just summarize/plot the CSVs already in output/",
    )
    args = parser.parse_args()

    selected = KERNELS
    if args.only:
        wanted = {name.strip().lower() for name in args.only.split(",")}
        selected = [k for k in KERNELS if k[0].lower() in wanted]
        if not selected:
            print(f"No kernels matched --only={args.only!r}", file=sys.stderr)
            sys.exit(1)

    results = {}
    pairs = {}
    residuals = {}
    for display_name, real_mod, xdsl_mod, real_csv, xdsl_csv in selected:
        if not args.skip_run:
            print(f"Running {display_name} (Path A + Path B)...")
            _run_benchmark(real_mod)
            _run_benchmark(xdsl_mod)

        real_path = CSV_DIR / real_csv
        xdsl_path = CSV_DIR / xdsl_csv
        if not real_path.exists() or not xdsl_path.exists():
            missing = real_path.name if not real_path.exists() else xdsl_path.name
            print(f"Skipping {display_name}: {missing} not found (run its benchmark script first)")
            continue

        results[display_name] = {
            "path_a": _stats(real_path),
            "path_b": _stats(xdsl_path),
        }
        pairs[display_name] = {
            "path_a": _load_pairs(real_path),
            "path_b": _load_pairs(xdsl_path),
        }
        residuals[display_name] = {
            "path_a": _load_residuals(real_path),
            "path_b": _load_residuals(xdsl_path),
        }

    if not results:
        print("Nothing to summarize -- no comparison CSVs found.", file=sys.stderr)
        sys.exit(1)

    _print_summary(results)

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    summary_path = _save_summary(results)
    png_path, pdf_path, data_path = _plot(results)
    scatter_pairs = {name: p["path_a"] for name, p in pairs.items()}
    scatter_png, scatter_pdf, scatter_data_path = plot_scatter_comparison.plot_combined_scatter(scatter_pairs)
    residual_png, residual_pdf, residual_data_path = plot_residual_comparison.plot_combined_residuals(residuals)

    print(f"Summary:       {summary_path}")
    print(f"Bar plot:      {png_path}")
    print(f"               {pdf_path}")
    print(f"Bar data:      {data_path}")
    print(f"Scatter plot:  {scatter_png}")
    print(f"               {scatter_pdf}")
    print(f"Scatter data:  {scatter_data_path}")
    print(f"Residual plot: {residual_png}")
    print(f"               {residual_pdf}")
    print(f"Residual data: {residual_data_path}")


if __name__ == "__main__":
    main()
