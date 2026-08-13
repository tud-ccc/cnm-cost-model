from enum import Enum


## UPMEM Specific operations
class InsType(Enum):
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


## DSL Abstract operations


class DSLInsType(Enum):
    ADD = "ADD"
    MUL = "MUL"
    MOV = "MOV"


class MEM_TYPE(Enum):
    MRAM = "MRAM"
    WRAM = "WRAM"


class MEM_OP_TYPE(Enum):
    LOAD = "LOAD"
    STORE = "STORE"
    INCREMENT_OFFSET = "INC"


class OperandType(Enum):
    SCALAR = 1
    ARRAY = 2
    CONST = 3


class Index:
    def __init__(self, name, range):
        self.name = name
        self.range = range


class OffsetIndex:
    def __init__(self, amount):
        self.amount = amount


class Operand:
    def __init__(self, name, type, data_type):
        self.name = name
        self.type = type
        self.data_type = data_type

    def setOffset(self, offset):
        self.offset = offset


class Operation:
    def __init__(self, BaseDSLInsType, operands, output, data_type):
        self.type = BaseDSLInsType
        self.operands = operands
        self.output = output
        self.data_type = data_type


class MEM_OP:
    def __init__(self, operand, mem_type, req_type):
        self.operand = operand
        self.mem_type = mem_type
        self.req_type = req_type

    def addSize(self, size):
        self.size = size

    def setIndexVar(self, indexVar):
        self.index = indexVar


class ForDSLIns:
    def __init__(self, ins, index):
        self.insList = ins
        self.index = index


class Kernel:
    def __init__(self, dslInstructions):
        self.ins = dslInstructions
