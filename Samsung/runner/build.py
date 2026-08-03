#!/usr/bin/env python3
"""One-shot build: links Homebrew's gtest into PIMSimulator if needed,
applies pimsimulator.patch if needed, builds PIMSimulator's library, then
builds bench_kernels against it.

Usage:
    python3 build.py
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

RUNNER_DIR = Path(__file__).resolve().parent
PIM_SIM_DIR = RUNNER_DIR.parent / "PIMSimulator"


def _run(cmd: list[str], cwd: Path) -> None:
    print(f"$ {' '.join(cmd)}  (in {cwd})")
    subprocess.run(cmd, cwd=cwd, check=True)


def main() -> None:
    _run([sys.executable, "ensure_gtest.py"], cwd=RUNNER_DIR)
    _run([sys.executable, "ensure_patch.py"], cwd=RUNNER_DIR)
    _run(["scons"], cwd=PIM_SIM_DIR)
    _run(["scons"], cwd=RUNNER_DIR)
    print(f"Done -- {RUNNER_DIR / 'bench_kernels'} is ready.")


if __name__ == "__main__":
    main()
