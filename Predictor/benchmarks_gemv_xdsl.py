import csv
import multiprocessing
from pathlib import Path

from configuration import gemv_file
from kernels.Gemv_xdsl import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict, loadFuncInstructionsDict
from support import LUTType, data_type_from_string

# Path B of the comparison started for VectorOp/ReduceOp/Scan/Hst/Sel: same
# real hardware measurements (gemv_file), same Simulator, but predicted from
# kernels/Gemv_xdsl.py's hand-transcribed CnmGen/output/llvcnm/gemv.llvcnm
# instruction list instead of gemv.py's hand-transcribed real assembly.
#
# NOTE: mirrors benchmarks_gemv.py's own full sweep, which is known to be
# slow on this machine -- validate on a sample first (see the
# random.sample-based spot check used when this file was built) before
# running this in full.
_OUTPUT_FILE = Path(__file__).resolve().parent / "output" / "gemv_comparison_xdsl.csv"

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
            for line in pool.imap(_predict_row, rows):
                if line is not None:
                    print(line)
                    out_file.write(line + "\n")
                    out_file.flush()


if __name__ == "__main__":
    main()
