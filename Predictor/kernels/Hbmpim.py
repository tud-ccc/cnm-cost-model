"""HBM-PIM cost model -- the Samsung-side counterpart to this repo's UPMEM
kernels.

Latency is built from a small table of measured (Instruction, BANK_LOC, ROW,
COL) sequence patterns (getLatency), not a flat per-instruction sum -- e.g.
back-to-back accesses to the same bank cost differently than alternating
banks, which is why the same instruction type appears multiple times in
the table, keyed by its neighboring context. calcAdd/calcRelu/calcGemv
compose these per-tile costs into a full-kernel estimate, including DRAM
refresh overhead (periodic refresh cycles that interrupt execution once
total latency crosses 2000, recurring every 4000 units after that).
"""

from __future__ import annotations

from enum import Enum


class Instruction(Enum):
    ACT = 1
    READ = 2
    PRE = 3
    WRITE = 4


class BANK_LOC(Enum):
    A = 1
    B = 2


class ROW(Enum):
    A = 1
    B = 2
    C = 3
    D = 4
    N = -1  # unknown/don't-care address


class COL(Enum):
    A = 1
    B = 2
    C = 3
    D = 4
    E = 5
    F = 6
    G = 7
    H = 8
    I = 8
    N = -1  # unknown/don't-care address


def getLatency(seq):
    """Looks up the measured latency for a specific instruction sequence
    pattern -- a literal table, not a formula, since real HBM-PIM timing
    depends on same-bank vs. cross-bank adjacency in ways not captured by a
    simple per-instruction cost."""
    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
    ]:
        return 32

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.B),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.B),
    ]:
        return 78

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
    ]:
        return 137

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
    ]:
        return 201

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.B),
        (Instruction.READ, BANK_LOC.B),
    ]:
        return 196

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.B),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.B),
    ]:
        return 90

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.B),
        (Instruction.READ, BANK_LOC.B),
        (Instruction.READ, BANK_LOC.A),
    ]:
        return 211

    if seq == [
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.B),
        (Instruction.READ, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.B),
    ]:
        return 164

    if seq == [
        (Instruction.READ, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.A),
        (Instruction.WRITE, BANK_LOC.A),
        (Instruction.READ, BANK_LOC.B),
        (Instruction.READ, BANK_LOC.B),
        (Instruction.WRITE, BANK_LOC.B),
    ]:
        return 268

    # GEMV pattern on a single bank
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.N),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.B),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.C),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.D),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.E),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.F),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.G),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.H),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.I),
    ]:
        return 380

    # Addition pattern
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.B, ROW.A, COL.A),
    ]:
        return 306

    # relu pattern
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.B, ROW.A, COL.A),
    ]:
        return 204

    # Read after an unknown-address read
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
    ]:
        return 59

    # Write after an unknown-address read
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
    ]:
        return 78

    # Write after an unknown-address read to a different bank
    if seq == [
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.B, ROW.N, COL.N),
    ]:
        return 23

    # Write after a known-address write to the same bank
    if seq == [
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.B, ROW.N, COL.N),
    ]:
        return 4

    if seq == [
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
    ]:
        return 4

    raise ValueError(f"No latency entry for sequence {seq!r}")


def _withRefresh(lat: float) -> float:
    """Periodic DRAM refresh interrupts execution once cumulative latency
    crosses 2000 (a fixed +364 cycle penalty), and recurs every 4000 units
    of latency after that -- both constants are direct measurements, not
    derived."""
    if lat > 2000:
        lat += 364
    refresh_count = int((lat - 2000) / 4000)
    return refresh_count * 364 + lat


def calcGemv(output_tile: int, input_tile: int) -> float:
    base_time = 367
    gemv_bank_seq = [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.N),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.B),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.C),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.D),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.E),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.F),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.G),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.H),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.I),
    ]
    compute_lat = getLatency(gemv_bank_seq)

    write_back_seq_worst = [
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
    ]
    write_back_seq_best = [
        (Instruction.READ, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.B, ROW.N, COL.N),
    ]
    write_back_lat_best = getLatency(write_back_seq_best)
    write_back_lat_worst = getLatency(write_back_seq_worst)

    lat = compute_lat * input_tile * output_tile
    lat += (
        write_back_lat_worst * output_tile
        if (input_tile % 2 == 0)
        else write_back_lat_best * output_tile
    )
    lat += base_time
    return _withRefresh(lat)


def calcAdd(tile_count: int) -> float:
    base_time = 413
    add_bank_seq = [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.B, ROW.A, COL.A),
    ]
    compute_lat = getLatency(add_bank_seq)

    dummy_seq = [  # used in the simulator!!
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.B, ROW.N, COL.N),
    ]
    dummy_lat = getLatency(dummy_seq)

    lat = compute_lat * tile_count + dummy_lat * tile_count
    lat += base_time
    return _withRefresh(lat)


def calcRelu(tile_count: int) -> float:
    base_time = 413
    relu_bank_seq = [
        (Instruction.READ, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.A, ROW.A, COL.A),
        (Instruction.READ, BANK_LOC.B, ROW.A, COL.A),
        (Instruction.WRITE, BANK_LOC.B, ROW.A, COL.A),
    ]
    compute_lat = getLatency(relu_bank_seq)

    dummy_seq = [  # used in the simulator!!
        (Instruction.WRITE, BANK_LOC.A, ROW.N, COL.N),
        (Instruction.WRITE, BANK_LOC.B, ROW.N, COL.N),
    ]
    dummy_lat = getLatency(dummy_seq)

    lat = compute_lat * tile_count + dummy_lat * tile_count
    lat += base_time
    return _withRefresh(lat)
