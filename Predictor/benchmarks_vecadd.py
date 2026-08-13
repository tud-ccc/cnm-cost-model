import csv
import multiprocessing
from pathlib import Path

from configuration import vector_op_file
from kernels.VectorOp import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict, loadFuncInstructionsDict
from support import LUTType, data_type_from_string, operation_from_string

_OUTPUT_FILE = Path(__file__).resolve().parent / "output" / "vecadd_comparison.csv"

# Number of worker processes used to run predictions in parallel. Each row's
# TestOp call is an independent, CPU-bound pure-Python simulation with no
# shared state -- multiprocessing (not threading, which the GIL would
# serialize back to one core) scales close to linearly with this.
PARALLEL_FACTOR = 48

_luts = None


def _init_worker():
    global _luts
    _luts = {
        LUTType.BaseInsLUT: loadBaseInstructionsDict(),
        LUTType.DmaLUT: loadDmaDict(),
        LUTType.FuncInsLut: loadFuncInstructionsDict(),
    }


def _predict_row(row):
    # A failed/faulted hardware run has no numeric latency to compare
    # against -- skip it rather than crash.
    if row["latency"] == "NA" or row["correct"] != "True":
        return None

    measured = float(row["latency"])
    # vecadd.csv's "iteration" column is already the per-tasklet chunk count
    # (local_iter_count), not multiplied by buffer_size like ReduceOp/Gemv --
    # TestOp's `iterations` arg is [chunk_count, buffer_size] directly.
    predicted = TestOp(
        _luts,
        operation_from_string(row["operation"]),
        int(row["thread"]),
        data_type_from_string(row["data_type"]),
        [int(row["iteration"]), int(row["buffer_size"])],
    )
    error_percent = ((predicted - measured) / measured) * 100

    return (
        f'{row["operation"]},{row["data_type"]},{row["thread"]},'
        f'{row["iteration"]},{row["buffer_size"]},'
        f"{measured:.6f},{predicted:.6f},{error_percent:.2f}"
    )


def main():
    with open(vector_op_file, newline="") as result_file:
        rows = list(csv.DictReader(result_file))

    header = (
        "operation,data_type,thread,iteration,buffer_size,"
        "measured,predicted,error_percent"
    )
    print(header)

    _OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(_OUTPUT_FILE, "w") as out_file:
        out_file.write(header + "\n")
        out_file.flush()
        with multiprocessing.Pool(PARALLEL_FACTOR, initializer=_init_worker) as pool:
            for line in pool.imap(_predict_row, rows):
                if line is not None:
                    print(line)
                    out_file.write(line + "\n")
                    out_file.flush()


if __name__ == "__main__":
    main()
