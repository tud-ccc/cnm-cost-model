
def RED(ins_lut, memory_lut, ls_ins_lut, thread_count, data_type, iterations):
    ins_list = []
    BUFFER_SIZE = iterations[1] * data_type_size(data_type)
    ins_list = [
        [
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.LSL),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.LSR),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.MOV),
            Instruction(
                memory_lut[WRAMAccessType.WRAMRead][1][data_type][BUFFER_SIZE],
                InsType.LDMA,
            ),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.ADD),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.ADD),
            # Instruction(ins_lut['ADD'][thread_count]['uint32_t'], InsType.MOV),
            # Instruction(ins_lut['ADD'][thread_count]['uint32_t'], InsType.MOV),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.ADD),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.JEQ),
        ],
        [
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.LSL),
            Instruction(ls_ins_lut["LOAD"][thread_count][data_type], InsType.L),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.ADD),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.ADD),
            Instruction(ins_lut["ADD"][thread_count]["uint32_t"], InsType.JEQ),
        ],
    ]
    return predict(iterations, ins_list, thread_count)