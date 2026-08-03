#!/usr/bin/env python3
"""Ranking accuracy / top-k selection quality, computed from the same
comparison CSVs run_full_comparison.py already produces (Predictor/output/
*_comparison.csv, *_comparison_xdsl.csv) -- no new hardware runs needed.

Absolute latency error doesn't tell you whether the model would actually
mis-rank configurations during design-space exploration: a systematic bias
(over/under-predicting every candidate by about the same amount) can have
large absolute error but perfect ranking, while uncorrelated noise can have
small absolute error but scramble which candidate looks best. This script
answers the DSE-relevant question directly: for a fixed workload, does the
model pick the actually-fastest (thread_count, buffer_size) configuration,
and if not, how much real performance does trusting it cost you (regret)?

Usage:
    python3 compute_ranking_accuracy.py

Run run_full_comparison.py first (or at least the individual benchmark
scripts) so the comparison CSVs this reads actually exist.
"""

from __future__ import annotations

import csv
import statistics
from collections import defaultdict
from pathlib import Path

from run_full_comparison import CSV_DIR, KERNELS

OUTPUT_DIR = CSV_DIR / "full_comparison"
OUTPUT_FILE = OUTPUT_DIR / "ranking_accuracy.txt"

# The sweep's configuration knobs (what a DSE search would actually choose
# between) -- every kernel's comparison CSV varies these two regardless of
# what else it sweeps (data_type, iteration, vec_length, n_length, ...).
CONFIG_FIELDS = {"thread", "buffer_size"}
EXCLUDED_FIELDS = CONFIG_FIELDS | {"measured", "predicted", "error_percent"}

TOP_KS = (1, 3, 5)
TOLERANCE_PCT = 1.0  # "within 1% of true best" -- a near-tie still counts as correct


def _load_rows(csv_path: Path) -> list[dict]:
    with open(csv_path, newline="") as f:
        return list(csv.DictReader(f))


def _group_by_workload(rows: list[dict]) -> dict[tuple, list[dict]]:
    """Groups sweep rows into DSE 'queries': fix everything except
    (thread_count, buffer_size), so each group is "given this workload,
    which config is fastest" -- the actual question CoMoNM is meant to
    answer, not "predict this one exact latency"."""
    groups: dict[tuple, list[dict]] = defaultdict(list)
    for r in rows:
        key = tuple(sorted((k, v) for k, v in r.items() if k not in EXCLUDED_FIELDS))
        groups[key].append(r)
    return groups


def _rank_correlation(measured: list[float], predicted: list[float]) -> float | None:
    """Spearman's rank correlation (Pearson correlation of the rank
    values), implemented directly rather than pulling in scipy for one
    formula. None if either ranking has zero variance (undefined)."""

    def ranks(values: list[float]) -> list[float]:
        order = sorted(range(len(values)), key=lambda i: values[i])
        r = [0.0] * len(values)
        i = 0
        while i < len(order):
            j = i
            while j + 1 < len(order) and values[order[j + 1]] == values[order[i]]:
                j += 1
            avg_rank = (i + j) / 2 + 1  # average rank for tied values
            for k in range(i, j + 1):
                r[order[k]] = avg_rank
            i = j + 1
        return r

    rm, rp = ranks(measured), ranks(predicted)
    n = len(measured)
    mean_rm, mean_rp = sum(rm) / n, sum(rp) / n
    cov = sum((a - mean_rm) * (b - mean_rp) for a, b in zip(rm, rp))
    var_rm = sum((a - mean_rm) ** 2 for a in rm)
    var_rp = sum((b - mean_rp) ** 2 for b in rp)
    if var_rm == 0 or var_rp == 0:
        return None
    return cov / (var_rm * var_rp) ** 0.5


def _evaluate_group(rows: list[dict]) -> dict:
    measured = [float(r["measured"]) for r in rows]
    predicted = [float(r["predicted"]) for r in rows]
    n = len(rows)

    true_best_idx = min(range(n), key=lambda i: measured[i])
    pred_best_idx = min(range(n), key=lambda i: predicted[i])
    true_best = measured[true_best_idx]

    # How much real performance you lose by trusting the model's pick,
    # relative to the true best candidate in this group.
    regret_pct = (measured[pred_best_idx] / true_best - 1) * 100

    # Where the model's chosen candidate actually ranks, by real latency
    # (1 = it happened to be the true fastest).
    sorted_by_measured = sorted(range(n), key=lambda i: measured[i])
    chosen_true_rank = sorted_by_measured.index(pred_best_idx) + 1

    result = {
        "n": n,
        "top1_hit": chosen_true_rank == 1,
        "top1_within_tol": regret_pct <= TOLERANCE_PCT,
        "regret_pct": regret_pct,
        "rank_correlation": _rank_correlation(measured, predicted) if n >= 4 else None,
    }
    for k in TOP_KS:
        if n >= k:
            result[f"top{k}_hit"] = chosen_true_rank <= k
    return result


def _aggregate(per_group: list[dict]) -> dict:
    def rate(key: str) -> float | None:
        vals = [r[key] for r in per_group if key in r]
        return 100 * sum(vals) / len(vals) if vals else None

    regrets = [r["regret_pct"] for r in per_group]
    corrs = [r["rank_correlation"] for r in per_group if r["rank_correlation"] is not None]

    agg = {
        "n_queries": len(per_group),
        "top1_accuracy": rate("top1_hit"),
        "top1_within_tol_accuracy": rate("top1_within_tol"),
        "mean_regret_pct": sum(regrets) / len(regrets),
        "median_regret_pct": statistics.median(regrets),
        "max_regret_pct": max(regrets),
        "mean_rank_correlation": sum(corrs) / len(corrs) if corrs else None,
    }
    for k in TOP_KS:
        agg[f"top{k}_accuracy"] = rate(f"top{k}_hit")
    return agg


def _format_report(name: str, path_label: str, agg: dict) -> str:
    lines = [f"{name} -- {path_label}"]
    lines.append(f"  queries (workload groups):          {agg['n_queries']}")
    for k in TOP_KS:
        val = agg.get(f"top{k}_accuracy")
        if val is not None:
            lines.append(f"  top-{k} accuracy:                      {val:.1f}%")
    lines.append(
        f"  top-1 accuracy (within {TOLERANCE_PCT:.0f}% tolerance): {agg['top1_within_tol_accuracy']:.1f}%"
    )
    lines.append(f"  mean regret:                         {agg['mean_regret_pct']:.2f}%")
    lines.append(f"  median regret:                       {agg['median_regret_pct']:.2f}%")
    lines.append(f"  worst-case regret:                   {agg['max_regret_pct']:.2f}%")
    if agg["mean_rank_correlation"] is not None:
        lines.append(f"  mean Spearman rank correlation:      {agg['mean_rank_correlation']:.3f}")
    return "\n".join(lines)


def main() -> None:
    lines = [
        "Ranking accuracy / top-k selection quality",
        "=" * 60,
        "Each 'query' groups sweep rows by fixed workload (everything except",
        "thread_count/buffer_size) and asks: among the (thread_count,",
        "buffer_size) candidates for that workload, does the model pick the",
        "actual fastest one? Regret is how much slower (in real measured",
        "cycles) the model's pick is than the true best, in percent -- the",
        f"practical cost of trusting the model's recommendation. Tolerance = {TOLERANCE_PCT:.0f}%",
        "treats a near-tie with the true best as correct, not just an exact match.",
        "",
    ]

    overall: dict[str, list[dict]] = {"path_a": [], "path_b": []}

    for display_name, _real_mod, _xdsl_mod, real_csv, xdsl_csv in KERNELS:
        real_path = CSV_DIR / real_csv
        xdsl_path = CSV_DIR / xdsl_csv
        if not real_path.exists() or not xdsl_path.exists():
            lines.append(f"{display_name}: skipped (missing comparison CSV)")
            lines.append("")
            continue

        lines.append(f"## {display_name}")
        for path_label, key, csv_path in (
            ("Path A", "path_a", real_path),
            ("Path B", "path_b", xdsl_path),
        ):
            rows = _load_rows(csv_path)
            groups = _group_by_workload(rows)
            per_group = [_evaluate_group(g) for g in groups.values() if len(g) >= 2]
            if not per_group:
                lines.append(f"{display_name} -- {path_label}: no multi-candidate queries")
                continue
            agg = _aggregate(per_group)
            overall[key].extend(per_group)
            lines.append(_format_report(display_name, path_label, agg))
            lines.append("")
        lines.append("")

    lines.append("## Overall (all kernels pooled)")
    for path_label, key in (("Path A", "path_a"), ("Path B", "path_b")):
        if overall[key]:
            agg = _aggregate(overall[key])
            lines.append(_format_report("All kernels", path_label, agg))
            lines.append("")

    report = "\n".join(lines).rstrip() + "\n"
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    OUTPUT_FILE.write_text(report)
    print(report)
    print(f"Written to {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
