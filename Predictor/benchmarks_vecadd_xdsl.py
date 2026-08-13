import csv
import multiprocessing
from pathlib import Path

from configuration import vector_op_file
from kernels.VectorOp_xdsl import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict, loadFuncInstructionsDict
from support import LUTType, data_type_from_string, operation_from_string

# Path B of the comparison started in benchmarks_vecadd.py: same real
# hardware measurements (vector_op_file), same Simulator, but predicted from
# kernels/VectorOp_xdsl.py's hand-transcribed CnmGen/output/llvcnm/vecadd.llvcnm
# instruction list instead of VectorOp.py's hand-transcribed real assembly --
# writes to its own CSV so the two predictions can be diffed side by side.
_OUTPUT_FILE = Path(__file__).resolve().parent / "output" / "vecadd_comparison_xdsl.csv"

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
