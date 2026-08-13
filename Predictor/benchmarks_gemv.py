import csv
import multiprocessing
from pathlib import Path

from configuration import gemv_file
from kernels.gemv import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict, loadFuncInstructionsDict
from support import LUTType, data_type_from_string

_OUTPUT_FILE = Path(__file__).resolve().parent / "output" / "gemv_comparison.csv"

# Number of worker processes used to run predictions in parallel. Each row's
# TestOp call is an independent, CPU-bound pure-Python simulation with no
# shared state -- multiprocessing (not threading, which the GIL would
# serialize back to one core) scales close to linearly with this.
PARALLEL_FACTOR = 48

_luts = None


def _init_worker():
    # Runs once per worker process (not once per row), so the LUTs are
    # loaded PARALLEL_FACTOR times total, not once per CSV row.
    global _luts
    _luts = {
        LUTType.BaseInsLUT: loadBaseInstructionsDict(),
        LUTType.DmaLUT: loadDmaDict(),
        LUTType.FuncInsLut: loadFuncInstructionsDict(),
    }


def _predict_row(row):
    # A failed/faulted hardware run (e.g. the WRAM-overflow case at
    # buffer_size=256, thread=16) has no numeric latency to compare
    # against -- skip it rather than crash.
    if row["latency"] == "NA" or row["correct"] != "True":
        return None

    measured = float(row["latency"])
    predicted = TestOp(
        _luts,
        int(row["thread"]),
        data_type_from_string(row["data_type"]),
        int(row["iteration"]),
        int(row["vec_length"]),
        int(row["buffer_size"]),
    )
    error_percent = ((predicted - measured) / measured) * 100

    return (
        f'{row["operation"]},{row["data_type"]},{row["thread"]},'
        f'{row["iteration"]},{row["vec_length"]},{row["buffer_size"]},'
        f"{measured:.6f},{predicted:.6f},{error_percent:.2f}"
    )


def main():
    with open(gemv_file, newline="") as result_file:
        rows = list(csv.DictReader(result_file))

    header = (
        "operation,data_type,thread,iteration,vec_length,buffer_size,"
        "measured,predicted,error_percent"
    )
    print(header)

    _OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    with open(_OUTPUT_FILE, "w") as out_file:
        out_file.write(header + "\n")
        out_file.flush()
        with multiprocessing.Pool(PARALLEL_FACTOR, initializer=_init_worker) as pool:
            # imap (not imap_unordered) keeps output in the same row order as
            # the input CSV, at negligible extra cost.
            for line in pool.imap(_predict_row, rows):
                if line is not None:
                    print(line)
                    out_file.write(line + "\n")
                    out_file.flush()


if __name__ == "__main__":
    main()
