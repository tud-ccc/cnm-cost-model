"""Reads the JSON program format emitted by the C++ cost model
(upmem-cost-model's ProgramBuilder::emitJson) and prices it with this model.

The point of the exchange is that the JSON carries no latencies: it names
opcodes, dtypes, DMA byte counts and loop trip counts, and every cost is
resolved here, from this model's own calibrated LUTs. Feeding the same
program through both simulators therefore isolates a *scheduling* difference
between the two engines from a *table* difference, which is the whole reason
the format exists -- a dump carrying the C++ side's latencies would only ever
re-measure the C++ side's tables.

Schema:
  {"kernel": str, "tasklets": int, "body": <seg>}

  <seg> = {"leaf":   [<ins>, ...]}
        | {"repeat": {"count": int, "body": <seg>}}
        | {"seq":    [<seg>, ...]}
        | {"ifthread": {"tids": [int], "body": <seg>}}   -- rejected, see below

  <ins> = {"op": "ADD"|"SUB"|"MUL"|"DIV"|"LOAD"|"STORE", "dtype": <ty>}
        | {"op": "MOV"|"J"|"LSL"|"LSR"}          -- untyped, all 11 cycles
        | {"op": "MUL", "dtype": "uint32_t", "imm": <pow2>}
        | {"op": "DMA_LOAD"|"DMA_STORE", "bytes": int}
        | {"op": "BARRIER"}

  Every instruction may carry a "note": a free-text label naming what the
  emitter generated it for. Ignored here; it exists so a dump can be diffed
  against real DPU disassembly by eye.

`ifthread` (the C++ builder's per-tasklet predication) has no counterpart in
this model, so it is rejected rather than approximated -- silently pricing a
predicated region as if every tasklet ran it would corrupt exactly the
comparison this module exists to make.

Usage:
  python3 cnmprog.py prog.json            # extrapolating engine (runSegment)
  python3 cnmprog.py prog.json --exact    # fully-unrolled oracle (runProgram)
"""

import argparse
import json

from configuration import buffer_sizes
from load_lut import (
    getBarrierCost,
    loadBarrierCostModel,
    loadBaseInstructionsDict,
    loadDmaDict,
    loadFuncInstructionsDict,
)
from predictor import (
    Leaf,
    Repeat,
    Seq,
    Simulator,
    _deepest_leaf_instructions,
)
from support import InsType, Instruction, data_type_from_string

_TYPED = {
    "ADD": InsType.ADD,
    "SUB": InsType.SUB,
    "MUL": InsType.MUL,
    "DIV": InsType.DIV,
    "LOAD": InsType.L,
    "STORE": InsType.S,
}
_UNTYPED = {"MOV": InsType.MOV, "J": InsType.J, "LSL": InsType.LSL, "LSR": InsType.LSR}
_DMA = {"DMA_LOAD": InsType.LDMA, "DMA_STORE": InsType.SDMA}

# Guards against a malformed or accidentally enormous trip count turning
# --exact into an OOM instead of an error. runProgram materialises one entry
# per dispatched instruction, so this is a memory bound, not a time bound.
_MAX_UNROLL = 50_000_000


class Luts:
    """The LUT bundle, loaded once. Construction reads several CSVs, so hold
    on to one instance when pricing many programs (e.g. a sweep)."""

    def __init__(self):
        self.base = loadBaseInstructionsDict()
        self.dma = loadDmaDict()
        self.func = loadFuncInstructionsDict()
        self.barrier = loadBarrierCostModel()

    def _dma(self, op_type, size_bytes):
        # Round up to the nearest calibrated size, matching both
        # kernels/gemv.py's _nearest_dma_instruction and the C++ model's
        # lookupDmaLatency -- so an off-table transfer size is priced the same
        # way on both sides instead of becoming a spurious difference.
        for size in sorted(buffer_sizes):
            if size >= size_bytes:
                return self.dma[(op_type, size)]
        return self.dma[(op_type, max(buffer_sizes))]

    def resolve(self, ins, thread_count):
        op = ins["op"]
        if op in _UNTYPED:
            return self.base[_UNTYPED[op]]
        if op == "BARRIER":
            return Instruction(
                InsType.BARRIER, getBarrierCost(self.barrier, thread_count)
            )
        if op in _DMA:
            return self._dma(_DMA[op], ins["bytes"])
        if op in _TYPED:
            dtype = data_type_from_string(ins["dtype"])
            if dtype is None:
                raise ValueError(f"unknown dtype {ins['dtype']!r} in {ins}")
            if "imm" in ins:
                key = (_TYPED[op], dtype, ins["imm"])
                if key not in self.func:
                    raise KeyError(
                        f"no func-LUT entry for {op} {ins['dtype']} imm={ins['imm']}"
                    )
                return self.func[key]
            key = (_TYPED[op], dtype)
            if key not in self.base:
                # Notably (MUL, INT32): this model has no single-instruction
                # cost for a general 32-bit multiply because the hardware has
                # no such instruction -- it is a __mulsi3 library call. The
                # emitter must expand it rather than ask for a price here.
                raise KeyError(
                    f"no base-LUT entry for {op}/{ins['dtype']}; it needs an "
                    f"explicit expansion, not a single instruction"
                )
            return self.base[key]
        raise KeyError(f"unknown op {op!r} in {ins}")


def build_segment(node, luts, thread_count):
    """Turn one JSON segment node into predictor.py's Leaf/Repeat/Seq tree."""
    if "leaf" in node:
        return Leaf([luts.resolve(i, thread_count) for i in node["leaf"]])
    if "repeat" in node:
        rep = node["repeat"]
        return Repeat(rep["count"], build_segment(rep["body"], luts, thread_count))
    if "seq" in node:
        return Seq([build_segment(p, luts, thread_count) for p in node["seq"]])
    if "ifthread" in node:
        raise NotImplementedError(
            "this program uses ifthread (per-tasklet predication), which this "
            "model cannot express; it cannot be cross-checked against the C++ "
            "simulator"
        )
    raise KeyError(f"unknown segment node with keys {sorted(node)}")


def flatten(segment, out=None):
    """Fully unroll a segment tree into the flat instruction list runProgram
    wants. Only for --exact: the list has one entry per dispatched
    instruction, so it is only viable on small configurations."""
    out = [] if out is None else out
    if isinstance(segment, Leaf):
        out.extend(segment.instructions)
    elif isinstance(segment, Seq):
        for part in segment.parts:
            flatten(part, out)
    elif isinstance(segment, Repeat):
        for _ in range(segment.count):
            flatten(segment.body, out)
            if len(out) > _MAX_UNROLL:
                raise MemoryError(
                    f"unrolled program exceeds {_MAX_UNROLL:,} instructions; "
                    f"use the extrapolating engine instead of --exact"
                )
    else:
        raise TypeError(f"unknown segment type {type(segment)}")
    return out


def run(program, luts=None, exact=False):
    """Price `program` (a parsed JSON dict) in ms.

    exact=False uses runSegment, the extrapolating engine -- what this model
    normally runs. exact=True fully unrolls and uses runProgram, which
    approximates nothing; that is the oracle to hold a C++ port against, but
    it is only tractable for small trip counts.
    """
    luts = luts or Luts()
    tasklets = program["tasklets"]
    segment = build_segment(program["body"], luts, tasklets)
    if not exact:
        return Simulator(tasklets).runSegment(segment)
    innermost = _deepest_leaf_instructions(segment)[1]
    return Simulator(tasklets).runProgram(flatten(segment), innermost)


def load(path):
    with open(path) as f:
        return json.load(f)


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("program", help="JSON file written by emitJson")
    parser.add_argument(
        "--exact",
        action="store_true",
        help="fully unroll and use runProgram (no extrapolation)",
    )
    args = parser.parse_args()

    program = load(args.program)
    ms = run(program, exact=args.exact)
    engine = "exact" if args.exact else "extrapolated"
    print(f"{program['kernel']}: {ms:.6f} ms  ({engine})")


if __name__ == "__main__":
    main()
