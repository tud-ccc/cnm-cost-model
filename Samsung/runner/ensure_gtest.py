#!/usr/bin/env python3
"""Idempotently symlinks Homebrew's googletest into Samsung/PIMSimulator so
its own (unmodified) Sconstruct can find it. PIMSimulator's build assumes a
system-wide gtest install, but Homebrew installs under /opt/homebrew, which
SCons's Environment() doesn't search by default -- this links gtest into
PIMSimulator's own CPPPATH/LIBPATH dirs (lib/gtest, project root) instead
of patching Sconstruct.

Uses Homebrew's stable /opt/homebrew paths, not the versioned Cellar path,
so it keeps working across googletest upgrades.

Usage:
    python3 ensure_gtest.py
"""

from __future__ import annotations

import subprocess
from pathlib import Path

RUNNER_DIR = Path(__file__).resolve().parent
PIM_SIM_DIR = RUNNER_DIR.parent / "PIMSimulator"

_BREW_INCLUDE = Path("/opt/homebrew/include")
_BREW_LIB = Path("/opt/homebrew/lib")


def _install_hint() -> str:
    return "Install it with: brew install googletest"


def main() -> None:
    gtest_header_dir = _BREW_INCLUDE / "gtest"
    libgtest = _BREW_LIB / "libgtest.a"
    libgtest_main = _BREW_LIB / "libgtest_main.a"

    missing = [p for p in (gtest_header_dir, libgtest, libgtest_main) if not p.exists()]
    if missing:
        missing_str = ", ".join(str(p) for p in missing)
        raise SystemExit(f"googletest not found at expected Homebrew paths ({missing_str}). {_install_hint()}")

    link_gtest_dir = PIM_SIM_DIR / "lib" / "gtest"
    link_libgtest = PIM_SIM_DIR / "libgtest.a"
    link_libgtest_main = PIM_SIM_DIR / "libgtest_main.a"

    links = [(link_gtest_dir, gtest_header_dir), (link_libgtest, libgtest), (link_libgtest_main, libgtest_main)]

    already_linked = all(link.is_symlink() and link.resolve() == target.resolve() for link, target in links)
    if already_linked:
        print("gtest already linked into PIMSimulator -- nothing to do")
        return

    for link, target in links:
        link.parent.mkdir(parents=True, exist_ok=True)
        if link.is_symlink() or link.exists():
            link.unlink()
        link.symlink_to(target)
        print(f"Linked {link} -> {target}")


if __name__ == "__main__":
    main()
