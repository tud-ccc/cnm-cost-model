#!/usr/bin/env python3
"""Builds every kernel and writes each pipeline stage's IR to its own file
under output/, so the generated linalg/cnm/llvcnm can be inspected directly
instead of only printed ad hoc during testing:

  output/linalg/<name>.mlir   -- the linalg-level source (frontend/linalg_kernels.py)
  output/cnm/<name>.mlir      -- after lowering to the `cnm` dialect
  output/llvcnm/<name>.llvcnm -- after lowering to llvcnm virtual assembly

`vecadd` predates the linalg front end and is still built directly as a
`cnm.kernel` (examples/build_vecadd.py) -- it has no linalg stage, so only
its cnm/llvcnm files are written.
"""

import io
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from xdsl.printer import Printer

from frontend.linalg_kernels import KERNEL_BUILDERS
from lowering.cnm_to_llvcnm import lower_kernel
from lowering.linalg_to_cnm import lower_module

OUTPUT_DIR = Path(__file__).resolve().parent / "output"
LINALG_DIR = OUTPUT_DIR / "linalg"
CNM_DIR = OUTPUT_DIR / "cnm"
LLVCNM_DIR = OUTPUT_DIR / "llvcnm"


def _render(op) -> str:
    stream = io.StringIO()
    Printer(stream=stream).print_op(op)
    stream.write("\n")
    return stream.getvalue()


def _write(path: Path, text: str) -> None:
    path.write_text(text)
    print(f"  -> {path.relative_to(OUTPUT_DIR.parent)}")


def run_linalg_kernel(name: str) -> None:
    print(f"{name}:")
    module = KERNEL_BUILDERS[name]()
    module.verify()
    _write(LINALG_DIR / f"{name}.mlir", _render(module))

    kernel = lower_module(module)
    kernel.verify()
    _write(CNM_DIR / f"{name}.mlir", _render(kernel))

    _write(LLVCNM_DIR / f"{name}.llvcnm", lower_kernel(kernel))


def run_vecadd() -> None:
    from examples.build_vecadd import build_kernel

    print("vecadd:")
    kernel = build_kernel()
    kernel.verify()
    _write(CNM_DIR / "vecadd.mlir", _render(kernel))
    _write(LLVCNM_DIR / "vecadd.llvcnm", lower_kernel(kernel))


def main() -> None:
    for d in (LINALG_DIR, CNM_DIR, LLVCNM_DIR):
        d.mkdir(parents=True, exist_ok=True)

    run_vecadd()
    for name in KERNEL_BUILDERS:
        run_linalg_kernel(name)


if __name__ == "__main__":
    main()
