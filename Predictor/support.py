from enum import Enum


class DataType(Enum):
    INT16 = 1
    INT32 = 2
    INT64 = 3
    FLOAT = 4
    DOUBLE = 5


class InsType(Enum):
    NOP = 0
    L = 1
    S = 2
    LDMA = 4  # load
    SDMA = 5  # store
    ADD = 6
    J = 7  # different types of jump, they have same latency
    MOV = 8
    MUL = 9
    AND = 10
    SUB = 12
    DIV = 13
    LSL = 14
    LSR = 15
    BARRIER = 16  # cross-tasklet barrier_wait(); cost depends on thread_count


def data_type_size(data_type):
    if data_type == DataType.INT16:
        return 2
    elif data_type == DataType.INT32:
        return 4
    elif data_type == DataType.INT64:
        return 8
    elif data_type == DataType.FLOAT:
        return 4
    elif data_type == DataType.DOUBLE:
        return 8


def operation_from_string(string):
    if string == "ADD":
        return InsType.ADD
    elif string == "SUB":
        return InsType.SUB
    elif string == "MUL":
        return InsType.MUL
    elif string == "DIV":
        return InsType.DIV
    elif string == "LOAD":
        return InsType.L
    elif string == "STORE":
        return InsType.S
    elif string == "DMA_LOAD":
        return InsType.LDMA
    elif string == "DMA_STORE":
        return InsType.SDMA


def data_type_from_string(data_type):
    if data_type == "uint16_t":
        return DataType.INT16
    if data_type == "uint32_t":
        return DataType.INT32
    if data_type == "uint64_t":
        return DataType.INT64
    if data_type == "float":
        return DataType.FLOAT
    if data_type == "double":
        return DataType.DOUBLE


class Instruction:
    def __init__(self, op_type, latency):
        self.op_type = op_type
        self.operand = None
        self.latency = latency
        self.service_interval = latency
        # When True, `latency` already reflects the full, measured
        # completion time for this instruction under a specific thread
        # count's contention (e.g. a per-thread-count DMA lookup), so the
        # Simulator should not additionally model it through the shared
        # DMA-queue/service_interval mechanism.
        self.pre_contended = False

    def setDataType(self, data_type):
        self.data_type = data_type

    def setLatency(self, latency):
        self.latency = latency

    def setOperand(self, operand):
        self.operand = operand

    def setSize(self, size):
        self.size = size

    def setServiceInterval(self, service_interval):
        self.service_interval = service_interval


class CustomDMAInstruction:
    def __init__(self, op_type, size, thread_count, dma_count, latency):
        self.size = size
        self.op_type = op_type
        self.thread_count = thread_count
        self.dma_count = dma_count
        self.latency = latency
        self.service_interval = latency
        self.pre_contended = False

    def setLatency(self, latency):
        self.latency = latency

    def setServiceInterval(self, service_interval):
        self.service_interval = service_interval


class CommunicationDirection(Enum):
    HostRank = 1
    RankHost = 2


class LUTType(Enum):
    BaseInsLUT = 1
    FuncInsLut = 2
    DmaLUT = 3
    CustomDMALUT = 4
    CoefCustomDMALUT = 5
    CustomADDDMALUT = 6
    DmaContentionLUT = 7
    BarrierLUT = 8


class WRAMAccessType(Enum):
    WRAMRead = 1
    WRAMWrite = 2


class ThreadState(Enum):
    Blocked = 1
    Free = 2
