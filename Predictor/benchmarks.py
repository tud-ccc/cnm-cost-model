import csv

from configuration import vector_op_file
from kernels.VectorOp import TestOp
from load_lut import loadBaseInstructionsDict, loadDmaDict
from support import (
    LUTType,
    data_type_from_string,
    operation_from_string,
)


def main():
    luts = {
        LUTType.BaseInsLUT: loadBaseInstructionsDict(),
        LUTType.DmaLUT: loadDmaDict(),
    }

    print(
        "operation,data_type,thread,iteration,buffer_size,"
        "measured,predicted,error_percent"
    )

    with open(vector_op_file, newline="") as result_file:
        for row in csv.DictReader(result_file):
            measured = float(row["latency"])
            predicted = TestOp(
                luts,
                operation_from_string(row["operation"]),
                int(row["thread"]),
                data_type_from_string(row["data_type"]),
                [int(row["iteration"]), int(row["buffer_size"])],
            )
            error_percent = ((predicted - measured) / measured) * 100

            print(
                f'{row["operation"]},{row["data_type"]},{row["thread"]},'
                f'{row["iteration"]},{row["buffer_size"]},'
                f"{measured:.6f},{predicted:.6f},{error_percent:.2f}"
            )


if __name__ == "__main__":
    main()
