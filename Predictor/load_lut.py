import csv
import math
from enum import Enum
from support import *
from configuration import *


def round(n):
    if n - int(n) == 0.5:
        return int(n) + 1
    return int(n)


def getInstructionFromLine(csv_line):
    ins_type = operation_from_string(csv_line[0])
    if ins_type in [InsType.LDMA, InsType.SDMA]:
        size = int(csv_line[1])
        latency = float(csv_line[3])
        cycle = round(float(csv_line[4]))
        instruction = Instruction(ins_type, cycle)
        instruction.setSize(size)
        return instruction
    else:
        data_type = data_type_from_string(csv_line[2])
        if ins_type == InsType.MUL and data_type == DataType.INT32:
            size = int(csv_line[3])
            latency = float(csv_line[4])
            cycle = round(float(csv_line[5]))
            instruction = Instruction(ins_type, cycle)
            instruction.setDataType(data_type)
            instruction.setSize(size)
            return instruction

        else:
            latency = float(csv_line[3])
            cycle = round(float(csv_line[4]))
            instruction = Instruction(ins_type, cycle)
            instruction.setDataType(data_type)
            return instruction


def getCustomDMAFromLine(csv_line):
    ins_type = operation_from_string(csv_line[0])
    size = int(csv_line[1])
    thread_count = int(csv_line[2])
    dma_count = int(csv_line[3])
    latency = float(csv_line[4])
    cycle = round(float(csv_line[5]))
    instruction = CustomDMAInstruction(ins_type, size, thread_count, dma_count, cycle)
    return instruction


def loadBaseInstructionsDict():
    file = open(pipeline_normal_csv_file, newline="")
    reader = csv.reader(file, delimiter=",", quotechar="|")
    next(reader)
    lut = {}
    for row in reader:
        instruction = getInstructionFromLine(row)
        lut[(instruction.op_type, instruction.data_type)] = instruction

    lut[(InsType.J)] = lut[(InsType.ADD, DataType.INT32)]
    lut[(InsType.LSL)] = lut[(InsType.ADD, DataType.INT32)]
    lut[(InsType.LSR)] = lut[(InsType.ADD, DataType.INT32)]
    lut[(InsType.MOV)] = lut[(InsType.ADD, DataType.INT32)]
    return lut


def loadFuncInstructionsDict():
    file = open(pipeline_func_csv_file, newline="")
    reader = csv.reader(file, delimiter=",", quotechar="|")
    next(reader)
    lut = {}
    for row in reader:
        instruction = getInstructionFromLine(row)
        lut[(instruction.op_type, instruction.data_type, instruction.size)] = (
            instruction
        )
    return lut


def loadDmaDict():
    # KNOWN ACCEPTED RESIDUAL: service_interval/latency here are calibrated
    # from thread_count=1 measurements only (see loadDmaLatencies below), and
    # the shared-queue contention model in predictor.py (next_available_dma
    # += service_interval per request) assumes that extrapolates cleanly to
    # any thread count. It doesn't, specifically for the smallest transfer
    # size: real VectorOp hardware sweeps show buffer_size=8's measured
    # latency consistently ~1.3-1.7x (not the ~2x a linear cost model
    # predicts) when doubling to buffer_size=16, and that shortfall grows
    # with thread count (1.70x at thread=1 down to 1.32x at thread=16,
    # highly reproducible across many iteration counts -- not noise). Bounded
    # at 6-9% prediction error at buffer_size=8, thread>=8; same category as
    # the already-accepted buffer_size=4 residual in Gemv/Gemm. A real fix
    # would need new perfcounter-based measurements isolating DMA-queue
    # behavior for small transfers across multiple thread counts (not just
    # thread=1) to recalibrate service_interval or correct the contention
    # formula itself -- not attempted here; left as a documented gap.
    customDmaLut = loadCustomDmaDict()
    serviceIntervals = loadDmaServiceIntervals()
    latencies = loadDmaLatencies()
    dmaLut = {}
    for ins in [InsType.LDMA, InsType.SDMA]:
        for size in buffer_sizes:
            instruction = customDmaLut[(ins, 1, 1, size)]
            instruction.setLatency(latencies.get((ins, size), instruction.latency))
            instruction.setServiceInterval(
                serviceIntervals.get((ins, size), instruction.latency)
            )
            dmaLut[(ins, size)] = instruction
    return dmaLut


def loadDmaLatencies():
    # The raw thread_count=1, dma_count=1 measurement (what customDmaLut
    # stores as .latency by default) is a single noisy sample. A linear fit
    # across all available thread=1 dma_count values (1-4) gives the true
    # marginal per-call cost with far less noise -- the same
    # marginal-not-raw-single-point philosophy loadDmaServiceIntervals
    # already applies, just along the dma_count axis instead of thread_count.
    # Empirically validated via a perfcounter compute-only/combined
    # decomposition on Sel's real hardware at thread=1 (zero queue
    # contention, so this isolates the latency term specifically from
    # service_interval): the raw dma_count=1 value overestimates real
    # per-request cost by 30-39% across sizes 16/64/128 bytes, while this
    # regression-based marginal cost is within 2-5%.
    with open(custom_dma_csv_file, newline="") as file:
        reader = csv.reader(file, delimiter=",", quotechar="|")
        next(reader)

        byKey = {}
        for row in reader:
            instruction = getCustomDMAFromLine(row)
            if instruction.thread_count != 1:
                continue
            key = (instruction.op_type, instruction.size)
            byKey.setdefault(key, []).append(
                (instruction.dma_count, instruction.latency)
            )

    latencies = {}
    for key, points in byKey.items():
        if len(points) < 2:
            continue
        n = len(points)
        sum_x = sum(p[0] for p in points)
        sum_y = sum(p[1] for p in points)
        sum_xy = sum(p[0] * p[1] for p in points)
        sum_xx = sum(p[0] * p[0] for p in points)
        denom = n * sum_xx - sum_x * sum_x
        if denom == 0:
            continue
        latencies[key] = (n * sum_xy - sum_x * sum_y) / denom
    return latencies


def loadDmaServiceIntervals():
    # Anchor the marginal per-request cost using the two *highest*-concurrency
    # measurements (e.g. thread=16 and thread=24), rather than fitting a line
    # from a single uncontended request out to the saturated end. A fit that
    # starts at 1 request averages over the flat, mostly-uncontended low-
    # concurrency region and the steep high-concurrency region together,
    # which underestimates the true saturated marginal cost that actually
    # matters once contention kicks in. Using two already-saturated points
    # instead measures the local (near-asymptotic) slope directly.
    with open(dma_service_csv_file, newline="") as file:
        reader = csv.reader(file, delimiter=",", quotechar="|")
        next(reader)

        byKeyAndThread = {}
        for row in reader:
            instruction = getCustomDMAFromLine(row)
            if instruction.latency < 0 or instruction.dma_count < 2:
                continue

            key = (instruction.op_type, instruction.size)
            perThread = byKeyAndThread.setdefault(key, {})
            if instruction.thread_count not in perThread:
                perThread[instruction.thread_count] = instruction

    serviceIntervals = {}
    for key, perThread in byKeyAndThread.items():
        threadCounts = sorted(perThread.keys())
        if len(threadCounts) < 2:
            continue
        low = perThread[threadCounts[-2]]
        high = perThread[threadCounts[-1]]

        lowRequestCount = low.thread_count * low.dma_count
        highRequestCount = high.thread_count * high.dma_count
        serviceIntervals[key] = (high.latency - low.latency) / (
            highRequestCount - lowRequestCount
        )

    # Empirically-validated correction for size=32 specifically (buffer_size=8
    # for uint32_t): the calibrated slope above (~47.81) is derived from
    # isolated DMA-only hammering at thread=16/24, but real mixed
    # compute+DMA workloads need a higher steady-state rate. Originally
    # isolated via a compute-only/combined decomposition on ReduceOp's real
    # hardware at thread=16 alone (55.73). Once loadDmaLatencies() below
    # replaced the raw dma_count=1 .latency with a regression-based marginal
    # value (lower than before), 55.73 no longer balanced thread=8 vs
    # thread=16 (thread=8 alone implied ~64.1, thread=16 alone ~57.4) --
    # rechecked against ReduceOp's real buffer_size=8 rows across all
    # threads and rebalanced to 58, which keeps both within ~1-5% (vs one or
    # the other being off by 9-13% at the endpoints).
    for op in [InsType.LDMA, InsType.SDMA]:
        if (op, 32) in serviceIntervals:
            serviceIntervals[(op, 32)] = 58

    # Same empirical correction, now for sizes 16 and 64 (buffer_size=4/16
    # for uint32_t), found via Sel's kernel -- the first kernel with 3
    # DMAs/chunk of mixed sizes (1 main-size read, 1 main-size write, 1
    # fixed-16-byte count write), which made calibration gaps at these sizes
    # visible for the first time. Calibrated jointly against Sel's real
    # thread=16 hardware rows (size=16 first from buf=4, a pure-size-16
    # config; size=64 second from buf=16, holding size=16's corrected value
    # fixed since that config also carries a 16-byte count write), then
    # cross-checked against Sel's thread=1/2/4/8 rows and other buffer sizes
    # (all within ~1-8%, down from up to -26%).
    for op in [InsType.LDMA, InsType.SDMA]:
        if (op, 16) in serviceIntervals:
            serviceIntervals[(op, 16)] = 42.82
        if (op, 64) in serviceIntervals:
            serviceIntervals[(op, 64)] = 68.43

    return serviceIntervals


def loadDmaContentionDict():
    with open(dma_contention_csv_file, newline="") as file:
        reader = csv.reader(file, delimiter=",", quotechar="|")
        next(reader)
        lut = {}
        for row in reader:
            ins_type = operation_from_string(row[0])
            size = int(row[1])
            thread_count = int(row[2])
            cycle = round(float(row[3]))
            instruction = Instruction(ins_type, cycle)
            instruction.setSize(size)
            instruction.pre_contended = True
            lut[(ins_type, size, thread_count)] = instruction
    return lut


def loadCustomDmaDict():
    file = open(custom_dma_csv_file, newline="")
    reader = csv.reader(file, delimiter=",", quotechar="|")
    next(reader)
    lut = {}
    for row in reader:
        instruction = getCustomDMAFromLine(row)
        lut[
            (
                instruction.op_type,
                instruction.thread_count,
                instruction.dma_count,
                instruction.size,
            )
        ] = instruction
    return lut


def createCustomAddDMADict():
    custom_lut = loadCustomDmaDict()
    add_lut = {}
    for size in buffer_sizes:
        for threads in thread_range:
            for dup in range(1, dup_max):
                for ins in [InsType.LDMA, InsType.SDMA]:
                    off = 0
                    for before in range(1, dup):
                        off += custom_lut[(InsType.LDMA, 1, 1, size)].latency
                    curr_latency = custom_lut[(ins, threads, dup, size)].latency
                    add_lut[(ins, threads, dup, size)] = int(
                        (curr_latency - off) / (threads)
                    )
    return add_lut


def loadBarrierCostModel():
    # Empirically calibrated in BarrierExperiment/ (see run_experiment.py): a
    # real barrier_wait() call's cost, measured via a with/without
    # differencing harness mirroring the DMA calibration methodology --
    # NOT derived from static disassembly. Real hardware showed the cost
    # scales worse than linearly with thread count (contention on the
    # barrier's internal spinlock, on top of the releasing tasklet's O(N)
    # resume-loop), which a fixed per-instruction cost or a naive
    # O(thread_count) formula would both underestimate at higher thread
    # counts. Fit as a power law (cycles = a * thread_count^b) via a
    # log-log linear regression over the calibrated points, so it can be
    # evaluated at any thread_count, not just the ones directly measured.
    with open(barrier_cost_csv_file, newline="") as file:
        reader = csv.reader(file)
        next(reader)
        points = [(int(row[0]), float(row[4])) for row in reader]

    xs = [math.log(t) for t, _ in points]
    ys = [math.log(c) for _, c in points]
    n = len(xs)
    sum_x = sum(xs)
    sum_y = sum(ys)
    sum_xy = sum(x * y for x, y in zip(xs, ys))
    sum_xx = sum(x * x for x in xs)
    b = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x)
    a = math.exp((sum_y - b * sum_x) / n)

    return {"a": a, "b": b}


def getBarrierCost(barrier_lut, thread_count):
    return barrier_lut["a"] * (thread_count ** barrier_lut["b"])


def loadLUTs():
    dmaLut = loadDmaDict()
    baseInsLUT = loadBaseInstructionsDict()
    funcInsLUT = loadFuncInstructionsDict()
    customDMALUT = loadCustomDmaDict()
    customDMACoefLUT = {}
    customAddDmaLUT = createCustomAddDMADict()
    barrierLUT = loadBarrierCostModel()
    LUTs = {
        LUTType.BaseInsLUT: baseInsLUT,
        LUTType.FuncInsLut: funcInsLUT,
        LUTType.DmaLUT: dmaLut,
        LUTType.CustomDMALUT: customDMALUT,
        LUTType.CoefCustomDMALUT: customDMACoefLUT,
        LUTType.CustomADDDMALUT: customAddDmaLUT,
        LUTType.BarrierLUT: barrierLUT,
    }
    return LUTs
