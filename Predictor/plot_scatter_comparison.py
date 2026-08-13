#!/usr/bin/env python3
"""Combined measured-vs-predicted scatter (Path A, all kernels on one
log-log axis), read from the comparison CSVs already in output/.

Usage:
    python3 plot_scatter_comparison.py
"""

from __future__ import annotations

import csv
import json
from pathlib import Path

PREDICTOR_DIR = Path(__file__).resolve().parent
CSV_DIR = PREDICTOR_DIR / "output"
OUTPUT_DIR = CSV_DIR / "full_comparison"

# (display name, Path A csv) -- kept in sync with run_full_comparison.py's
# KERNELS list.
KERNELS = [
    ("VectorOp", "vecadd_comparison.csv"),
    ("ReduceOp", "reduceop_comparison.csv"),
    ("Scan", "scan_comparison.csv"),
    ("Hst", "hst_comparison.csv"),
    ("Sel", "sel_comparison.csv"),
    ("Gemv", "gemv_comparison.csv"),
    ("Gemm", "gemm_comparison.csv"),
]

# Fixed categorical order, first 7 slots of the validated palette (dataviz
# skill, references/palette.md).
KERNEL_COLORS = {
    "VectorOp": "#2a78d6",
    "ReduceOp": "#eb6834",
    "Scan": "#1baf7a",
    "Hst": "#eda100",
    "Sel": "#e87ba4",
    "Gemv": "#008300",
    "Gemm": "#4a3aa7",
}


def _load_pairs(csv_path: Path) -> tuple[list[float], list[float]]:
    """(measured, predicted) latencies -- every comparison CSV has these
    two columns regardless of what else varies per kernel."""
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    measured = [float(r["measured"]) for r in rows]
    predicted = [float(r["predicted"]) for r in rows]
    return measured, predicted


def load_all_pairs(kernels=KERNELS) -> dict:
    pairs = {}
    for display_name, real_csv in kernels:
        real_path = CSV_DIR / real_csv
        if not real_path.exists():
            print(f"Skipping {display_name}: {real_path.name} not found (run its benchmark script first)")
            continue
        pairs[display_name] = _load_pairs(real_path)
    return pairs


def plot_combined_scatter(pairs: dict) -> tuple[Path, Path, Path]:
    """One shared-axis plot across all kernels' Path A predictions, colored
    by kernel. Y-axis is the ratio (predicted/measured), not the raw
    value -- at this accuracy, raw values all sit on the y=x diagonal
    regardless of scale, so the ratio is what actually shows deviation."""
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.lines import Line2D

    fig, ax = plt.subplots(figsize=(7, 5))

    all_measured: list[float] = []
    all_ratios: list[float] = []
    for name, (measured, predicted) in pairs.items():
        color = KERNEL_COLORS[name]
        ratio = [p / m for m, p in zip(measured, predicted) if m > 0]
        m_pos = [m for m in measured if m > 0]
        ax.scatter(
            m_pos, ratio, s=18, alpha=0.55, color=color,
            marker="o", linewidths=0, zorder=2,
        )
        all_measured.extend(m_pos)
        all_ratios.extend(ratio)

    x_lo, x_hi = min(all_measured) * 0.8, max(all_measured) * 1.25
    # Linear y-axis: the ratio only spans ~0.8-1.4x, so log scale would just
    # force ugly non-round ticks for no benefit.
    max_dev = max(abs(r - 1.0) for r in all_ratios)
    y_pad = max_dev * 1.15
    y_lo, y_hi = 1.0 - y_pad, 1.0 + y_pad

    ax.axhline(1.0, linestyle="--", linewidth=1, color="#52514e", zorder=1)

    ax.set_xscale("log")
    ax.set_xlim(x_lo, x_hi)
    ax.set_ylim(y_lo, y_hi)
    ax.set_xlabel("Measured latency (cycles, log scale)")
    ax.set_ylabel("Predicted / measured ratio")
    ax.set_title("Prediction ratio vs. measured latency, all kernels\n(dashed line = perfect prediction)")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    kernel_handles = [
        Line2D([0], [0], marker="o", linestyle="", color=KERNEL_COLORS[name], label=name, markersize=7)
        for name in pairs
    ]
    ax.legend(handles=kernel_handles, title="Kernel", loc="upper left", frameon=False, fontsize=8)

    fig.tight_layout()

    png_path = OUTPUT_DIR / "scatter_plot.png"
    pdf_path = OUTPUT_DIR / "scatter_plot.pdf"
    fig.savefig(png_path, dpi=200, bbox_inches="tight")
    fig.savefig(pdf_path, bbox_inches="tight")
    plt.close(fig)

    scatter_data = {
        name: {"measured": measured, "predicted": predicted}
        for name, (measured, predicted) in pairs.items()
    }
    data_path = OUTPUT_DIR / "scatter_data.json"
    data_path.write_text(json.dumps(scatter_data, indent=2))

    return png_path, pdf_path, data_path


def main() -> None:
    pairs = load_all_pairs()
    if not pairs:
        raise SystemExit(f"Nothing to plot -- no comparison CSVs found in {CSV_DIR}")

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    png_path, pdf_path, data_path = plot_combined_scatter(pairs)

    print(f"Scatter plot: {png_path}")
    print(f"              {pdf_path}")
    print(f"Scatter data: {data_path}")


if __name__ == "__main__":
    main()
