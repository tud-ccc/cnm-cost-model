#!/usr/bin/env python3
"""Idempotently applies pimsimulator.patch to Samsung/PIMSimulator.

PIMSimulator is kept as a clean, unmodified vendor checkout (its own git
repo, tracking github.com/SAITPublic/PIMSimulator) -- everything this
project needs beyond that pristine checkout lives in pimsimulator.patch
instead of as permanent local edits. The patch bumps -std=c++14 to
-std=c++17 (required to build against current gtest) and fixes three
correctness bugs in src/tests/PIMKernel.cpp (see the patch itself, or
pimsimulator.patch's own diff context, for details on each).

Detects whether the patch is already applied via `git apply --reverse
--check` (reverse-applies cleanly iff already applied forward), so it's
safe to run before every build regardless of PIMSimulator's state.

Usage:
    python3 ensure_patch.py
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

RUNNER_DIR = Path(__file__).resolve().parent
PIM_SIM_DIR = RUNNER_DIR.parent / "PIMSimulator"
PATCH_FILE = RUNNER_DIR / "pimsimulator.patch"


def patch_is_applied() -> bool:
    result = subprocess.run(
        ["git", "apply", "--reverse", "--check", str(PATCH_FILE)],
        cwd=PIM_SIM_DIR,
        capture_output=True,
        text=True,
    )
    return result.returncode == 0


def patch_applies_cleanly() -> bool:
    result = subprocess.run(
        ["git", "apply", "--check", str(PATCH_FILE)],
        cwd=PIM_SIM_DIR,
        capture_output=True,
        text=True,
    )
    return result.returncode == 0


def main() -> None:
    if not PIM_SIM_DIR.exists():
        raise SystemExit(f"{PIM_SIM_DIR} not found")

    if patch_is_applied():
        print("pimsimulator.patch already applied -- nothing to do")
        return

    if not patch_applies_cleanly():
        raise SystemExit(
            "pimsimulator.patch does not apply cleanly and does not look "
            "already-applied either -- PIMSimulator's checkout has likely "
            "drifted from what the patch expects. Try resetting it first: "
            "(cd ../PIMSimulator && git checkout -- . && git clean -fdx)"
        )

    print("Applying pimsimulator.patch (C++17 build flag + PIMKernel correctness fixes)...")
    subprocess.run(["git", "apply", str(PATCH_FILE)], cwd=PIM_SIM_DIR, check=True)
    print("Applied.")


if __name__ == "__main__":
    main()
