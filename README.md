# CoMoNM Cost Model

CoMoNM is a cost-modeling framework for Compute-Near-Memory (CNM) systems
(UPMEM, Samsung HBM-PIM). It estimates
execution time without running the time-consuming system-level simulators or the hardware.
Validated against real UPMEM hardware
and Samsung's HBM-PIM simulator (within ±2.5%/±2.99% error), it produces
estimates orders of magnitude faster than simulation.

## Structure

The cost model itself lives in `Predictor/`. Each kernel is benchmarked two ways:
- `benchmarks_<kernel>_xdsl.py` predicts from the high-level assembly generated via `CnmGen`.
- `benchmarks_<kernel>.py` predicts from the assembly parsed out of the real C implementation.

For Samsung HBM-PIM, `benchmarks_hbmpim.py` does the same. All of these
compare the prediction against real measurements from the `UPMEM/` and
`Samsung/` folders, and write a comparison CSV to `Predictor/output/`.

### `UPMEM/`

Each benchmark folder (`Vecadd/`, `ReduceOp/`, `Scan/`, `Hst/`, `Sel/`,
`Gemv/`, `Gemm/`, `2mm/`, `3mm/`) has a `run_script/` script that runs the
kernel on real UPMEM hardware across a sweep of configurations and stores
the results as a CSV in its `output/` folder. Rerun that script if you want
fresh numbers.

### `Samsung/`

`runner/` drives Samsung's PIMSimulator (a git submodule) the same way:
`run_code.py <kernel>` runs the kernel on the simulator and stores the
results in `runner/results/`. Rerun it if you want fresh numbers.

## Running it

Requires Python 3.10+ and `matplotlib`/`numpy` for `Predictor/`; `CnmGen/`
needs `xdsl`.

Every `output/`/`results/` directory already has committed results, so
cloning the repo is enough to inspect predictions. To regenerate a kernel's
comparison:

```bash
cd cnm-cost-model/Predictor
python3 benchmarks_vecadd.py
python3 benchmarks_vecadd_xdsl.py
```

or everything at once:

```bash
python3 run_full_comparison.py
```

For Samsung HBM-PIM (needs the `Samsung/PIMSimulator` submodule, a C++17
toolchain, `scons`, and `googletest`):

```bash
git submodule update --init Samsung/PIMSimulator
cd Samsung/runner
python3 build.py               # patches + builds PIMSimulator and the runner binary
python3 run_code.py add        # or relu / gemv
cd ../../Predictor
python3 benchmarks_hbmpim.py
```
