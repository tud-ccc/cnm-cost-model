"""VecAdd kernel (C[i] = A[i] + B[i] for i in 0..N), built with the real
two-level chunk/element structure every kernel in this codebase has: an
outer loop DMAs a whole chunk from MRAM into WRAM scratch buffers once, an
inner loop does the per-element compute purely on those WRAM buffers (no
MRAM access inside it at all), then the outer loop DMAs the result chunk
back out -- MRAM can't be touched economically at per-scalar granularity,
which is why `cnm.alloc`/`cnm.dma_load`/`cnm.dma_store` exist as distinct
ops from `cnm.load`/`cnm.store`."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from xdsl.builder import Builder
from xdsl.dialects.arith import ConstantOp
from xdsl.dialects.builtin import IndexType, IntegerAttr, MemRefType, i32
from xdsl.ir import Block, Region
from xdsl.printer import Printer
from xdsl.rewriter import InsertPoint

from dialects.cnm import (
    AllocOp,
    ComputeOp,
    DmaLoadOp,
    DmaStoreOp,
    ForOp,
    KernelOp,
    LoadOp,
    OperandOp,
    OutputOp,
    ReturnOp,
    StoreOp,
    YieldOp,
)

N = 1024
CHUNK = 128
NUM_CHUNKS = N // CHUNK

array_t = MemRefType(i32, [N])
chunk_t = MemRefType(i32, [CHUNK])
index_t = IndexType()


def _const(builder: Builder, value: int, ty=index_t):
    return builder.insert(ConstantOp(IntegerAttr(value, ty))).result


def build_kernel() -> KernelOp:
    kernel_block = Block()
    kb = Builder(InsertPoint.at_end(kernel_block))

    a_op = kb.insert(OperandOp("A", array_t))
    b_op = kb.insert(OperandOp("B", array_t))
    c_op = kb.insert(OutputOp("C", array_t))

    cache_a = kb.insert(AllocOp(chunk_t))
    cache_b = kb.insert(AllocOp(chunk_t))
    cache_c = kb.insert(AllocOp(chunk_t))

    num_chunks_const = _const(kb, NUM_CHUNKS)

    # --- outer (chunk) loop body ---
    chunk_block = Block(arg_types=[index_t])
    cb = Builder(InsertPoint.at_end(chunk_block))
    chunk_idx = chunk_block.args[0]

    chunk_size_const = _const(cb, CHUNK)
    offset = cb.insert(ComputeOp("mul", [chunk_idx, chunk_size_const], index_t)).res

    cb.insert(DmaLoadOp(a_op, offset, cache_a, size=CHUNK))
    cb.insert(DmaLoadOp(b_op, offset, cache_b, size=CHUNK))

    # --- inner (per-element) loop body: pure WRAM traffic, no MRAM access ---
    elem_block = Block(arg_types=[index_t])
    eb = Builder(InsertPoint.at_end(elem_block))
    j = elem_block.args[0]

    a_val = eb.insert(LoadOp(cache_a, [j], level="WRAM", result_type=i32)).res
    b_val = eb.insert(LoadOp(cache_b, [j], level="WRAM", result_type=i32)).res
    sum_val = eb.insert(ComputeOp("add", [a_val, b_val], i32)).res
    eb.insert(StoreOp(sum_val, cache_c, [j], level="WRAM"))
    eb.insert(YieldOp())

    elem_ub = _const(cb, CHUNK)
    cb.insert(ForOp(ub=elem_ub, iter_args=[], body=Region(elem_block)))

    cb.insert(DmaStoreOp(cache_c, c_op, offset, size=CHUNK))
    cb.insert(YieldOp())

    kb.insert(ForOp(ub=num_chunks_const, iter_args=[], body=Region(chunk_block)))
    kb.insert(ReturnOp([c_op]))

    return KernelOp("vecadd", Region(kernel_block))


if __name__ == "__main__":
    kernel = build_kernel()
    kernel.verify()
    Printer().print_op(kernel)
    print()
