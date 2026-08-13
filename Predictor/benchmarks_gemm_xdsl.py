import csv
import multiprocessing
from pathlib import Path

from configuration import gemm_file
from kernels.Gemm_xdsl import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict, loadFuncInstructionsDict
from support import LUTType, data_type_from_string

# Path B of the comparison started for VectorOp/ReduceOp/Scan/Hst/Sel/Gemv:
# same real hardware measurements (gemm_file), same Simulator, but predicted
# from kernels/Gemm_xdsl.py's hand-transcribed CnmGen/output/llvcnm/gemm.llvcnm
# instruction list instead of Gemm.py's hand-transcribed real assembly.
_OUTPUT_FILE = Path(__file__).resolve().parent / "output" / "gemm_comparison_xdsl.csv"

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
        int(row["n_length"]),
        int(row["buffer_size"]),
    )
    error_percent = ((predicted - measured) / measured) * 100

    return (
        f'{row["operation"]},{row["data_type"]},{row["thread"]},'
        f'{row["iteration"]},{row["vec_length"]},{row["n_length"]},'
        f'{row["buffer_size"]},{measured:.6f},{predicted:.6f},{error_percent:.2f}'
    )


def main():
    with open(gemm_file, newline="") as result_file:
        rows = list(csv.DictReader(result_file))

    header = (
        "operation,data_type,thread,iteration,vec_length,n_length,buffer_size,"
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
