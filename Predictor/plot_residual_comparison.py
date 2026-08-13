#!/usr/bin/env python3
"""Per-kernel prediction-error figure: one error%-vs-measured panel per
kernel (4 on the first row, 3 on the second), read from the comparison
CSVs already in output/.

x = measured latency (log scale), y = signed error % -- error % is
scale-normalized, so outliers stay visible regardless of latency
magnitude, unlike a raw measured-vs-predicted scatter.

Usage:
    python3 plot_residual_comparison.py
"""

from __future__ import annotations

import csv
import json
from pathlib import Path

PREDICTOR_DIR = Path(__file__).resolve().parent
CSV_DIR = PREDICTOR_DIR / "output"
OUTPUT_DIR = CSV_DIR / "full_comparison"

# (display name, Path A csv, Path B csv) -- names/order match the paper's
# own kernel naming (Table 1 / fig:upmem_validation): sel, hst, scan, red,
# va, GEMV, GEMM. Order matters: first 4 go on row 1, remaining 3 on row 2.
KERNELS = [
    ("sel", "sel_comparison.csv", "sel_comparison_xdsl.csv"),
    ("hst", "hst_comparison.csv", "hst_comparison_xdsl.csv"),
    ("scan", "scan_comparison.csv", "scan_comparison_xdsl.csv"),
    ("red", "reduceop_comparison.csv", "reduceop_comparison_xdsl.csv"),
    ("va", "vecadd_comparison.csv", "vecadd_comparison_xdsl.csv"),
    ("GEMV", "gemv_comparison.csv", "gemv_comparison_xdsl.csv"),
    ("GEMM", "gemm_comparison.csv", "gemm_comparison_xdsl.csv"),
]

# Same "c" / "gen" labels and colors as the paper's own UPMEM validation
# figure (upmem_validate_small.tex's `\legend{c, gen}`).
COLOR_C = "#7bccc4"
COLOR_GEN = "#08589E"


def _load_residuals(csv_path: Path) -> tuple[list[float], list[float]]:
    """(measured, signed error_percent) -- signed, not abs, so over- vs
    under-prediction stays visible."""
    with open(csv_path, newline="") as f:
        rows = list(csv.DictReader(f))
    measured = [float(r["measured"]) for r in rows]
    error_percent = [float(r["error_percent"]) for r in rows]
    return measured, error_percent


def load_all_residuals(kernels=KERNELS) -> dict:
    residuals = {}
    for display_name, real_csv, xdsl_csv in kernels:
        real_path = CSV_DIR / real_csv
        xdsl_path = CSV_DIR / xdsl_csv
        if not real_path.exists() or not xdsl_path.exists():
            missing = real_path.name if not real_path.exists() else xdsl_path.name
            print(f"Skipping {display_name}: {missing} not found (run its benchmark script first)")
            continue
        residuals[display_name] = {
            "path_a": _load_residuals(real_path),
            "path_b": _load_residuals(xdsl_path),
        }
    return residuals


def plot_combined_residuals(residuals: dict) -> tuple[Path, Path, Path]:
    """4-then-3 panel grid, all seven panels the same size, row 2 centered
    under row 1 rather than stretched. A 24-column grid makes the
    centering land on exact boundaries (6 cols/panel either way; row 2's
    18 cols get a 3-col margin on each side). No figure-level title --
    that belongs in the LaTeX caption."""
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.gridspec import GridSpec

    names = list(residuals.keys())
    row1, row2 = names[:4], names[4:]

    fig = plt.figure(figsize=(16, 7.5))
    gs = GridSpec(2, 24, figure=fig, hspace=0.55, wspace=0.55, top=0.87)

    panels = [(fig.add_subplot(gs[0, i * 6:(i + 1) * 6]), name) for i, name in enumerate(row1)]
    row2_offset = (24 - 6 * len(row2)) // 2
    panels += [
        (fig.add_subplot(gs[1, row2_offset + i * 6:row2_offset + (i + 1) * 6]), name)
        for i, name in enumerate(row2)
    ]

    for ax, name in panels:
        m_c, e_c = residuals[name]["path_a"]
        m_gen, e_gen = residuals[name]["path_b"]

        ax.scatter(m_c, e_c, s=16, alpha=0.45, color=COLOR_C, label="c", linewidths=0)
        ax.scatter(m_gen, e_gen, s=16, alpha=0.45, color=COLOR_GEN, label="gen", linewidths=0)
        ax.axhline(0, linestyle="--", linewidth=1, color="#52514e", zorder=1)

        # Headroom so the 0%-error line never sits flush against a panel's
        # own top/bottom edge (e.g. Gemm, whose error is always negative).
        ax.margins(y=0.15)

        ax.set_xscale("log")
        ax.set_title(name, fontsize=14)
        ax.tick_params(labelsize=11)
        ax.spines["top"].set_visible(False)
        ax.spines["right"].set_visible(False)

    # No axis labels baked in -- x is measured latency (ms, log scale) and
    # y is signed prediction error (%); both go in the LaTeX caption.
    handles, labels = panels[0][0].get_legend_handles_labels()
    fig.legend(handles, labels, loc="upper center", ncol=2, frameon=False, bbox_to_anchor=(0.5, 1.02), fontsize=13)

    png_path = OUTPUT_DIR / "residual_plot.png"
    pdf_path = OUTPUT_DIR / "residual_plot.pdf"
    fig.savefig(png_path, dpi=200, bbox_inches="tight")
    fig.savefig(pdf_path, bbox_inches="tight")
    plt.close(fig)

    residual_data = {
        name: {
            "path_a": {"measured": paths["path_a"][0], "error_percent": paths["path_a"][1]},
            "path_b": {"measured": paths["path_b"][0], "error_percent": paths["path_b"][1]},
        }
        for name, paths in residuals.items()
    }
    data_path = OUTPUT_DIR / "residual_data.json"
    data_path.write_text(json.dumps(residual_data, indent=2))

    return png_path, pdf_path, data_path


def main() -> None:
    residuals = load_all_residuals()
    if not residuals:
        raise SystemExit(f"Nothing to plot -- no comparison CSVs found in {CSV_DIR}")

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    png_path, pdf_path, data_path = plot_combined_residuals(residuals)

    print(f"Residual plot: {png_path}")
    print(f"               {pdf_path}")
    print(f"Residual data: {data_path}")


if __name__ == "__main__":
    main()
