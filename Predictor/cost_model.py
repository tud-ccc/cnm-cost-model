from support import *
from configuration import *
from load_lut import *


class Pipeline_ins:
    def __init__(self, ins, thread):
        self.ins = ins
        self.thread = thread


class ThreadState:
    def __init__(self, id):
        self.last_cycle = -1
        self.blocked_until = -1
        self.id = id

    def exec(self, placed_cycle, instruction_latency):
        self.last_cycle = placed_cycle + instruction_latency

    def available(self, cycle):
        return self.last_cycle < cycle


class Kernel:
    def __init__(self, ins_list):
        self.ins_list = ins_list
        self.flatten_list = self.flatIns(ins_list[0], ins_list[1])
        self.all_ins_met_index = self.calcAllMetIndex(ins_list[0], 1)

    def flatIns(self, input_list, iteration):
        flatten_list = []
        for i in range(iteration):
            for ins in input_list:
                if type(ins) == tuple:
                    flatten_list += self.flatIns(ins[0], ins[1])
                else:
                    flatten_list.append(ins)
        return flatten_list

    def calcAllMetIndex(self, input_list, iteration):
        ans = 1
        for ins in input_list:
            if type(ins) == tuple:
                ans += self.calcAllMetIndex(ins[0], ins[1])
            else:
                ans += 1
        return ans * iteration


class Pipeline:
    def __init__(self, number_of_stage):
        self.number_of_stage = number_of_stage
        self.threads_inside = {}
        self.curr_cycle = 0

    def can_put(self, thread_id):
        if thread_id in self.threads_inside:
            if len(self.threads_inside[thread_id]) == 2:
                return False
            else:
                if len(self.threads_inside[thread_id]) == 0:
                    return True
                if self.threads_inside[thread_id][0] <= 11:
                    return False
                else:
                    return True
        else:
            return True

    def put(self, thread_id):
        if thread_id not in self.threads_inside:
            self.threads_inside[thread_id] = []
        self.threads_inside[thread_id].append(1)

    def cycle(self):
        self.curr_cycle += 1
        for thread_id in self.threads_inside:
            for x in range(len(self.threads_inside[thread_id])):
                self.threads_inside[thread_id][x] += 1
                if self.threads_inside[thread_id][x] > 14:
                    del self.threads_inside[thread_id][0]
                    break


class DMA:
    def __init__(self):
        self.read_busy = False
        self.write_busy = False
        self.read_process_req = []
        self.write_process_req = []

    def read_available(self):
        if len(self.read_process_req) <= 1:
            return True
        else:
            return False

    def write_available(self):
        if len(self.read_process_req) <= 1:
            return True
        else:
            return False

    def add_read(self, time, thread):
        self.read_process_req.append((time, thread))

    def add_write(self, time, thread):
        self.read_process_req.append((time, thread))

    def cycle(self):
        new_read_list = []
        for read_req in self.read_process_req:
            time = read_req[0] - 1
            thread = read_req[1]
            if time != 0:
                new_read_list.append((time, thread))
            else:
                thread.dma_done()
        self.read_process_req = new_read_list

        new_write_list = []
        for write_req in self.write_process_req:
            time = write_req[0] - 1
            thread = write_req[1]
            if time != 0:
                new_read_list.append((time, thread))
            else:
                thread.dma_done()
        self.write_process_req = new_write_list


class Thread:
    def __init__(self, id, thread_count, kernel, pipeline, dma):
        self.id = id
        self.thread_count = thread_count
        self.kernel = kernel
        self.pipeline = pipeline
        self.index = 0
        self.done = False
        self.dma_blocked = False
        self.dma = dma
        self.add_dma_lut = createCustomAddDMADict()

    def exec(self):
        ins = self.kernel.flatten_list[self.index]
        if not self.dma_blocked:
            if ins.op_type == InsType.LDMA:
                dma_lat = self.add_dma_lut[(ins.op_type, 1, 1, ins.size)]
                if self.dma.read_available():
                    self.dma.add_read(int(dma_lat + 44), self)
                    self.dma_blocked = True
                    return True
                else:
                    return False
            elif ins.op_type == InsType.SDMA:
                dma_lat = self.add_dma_lut[(ins.op_type, 1, 1, ins.size)]
                if self.dma.write_available():
                    self.dma.add_write(int(dma_lat * 3), self)
                    self.dma_blocked = True
                    return True
                else:
                    return False

            else:
                if self.pipeline.can_put(self.id):
                    self.pipeline.put(self.id)
                    self.index += 1
                    if (self.index) >= (self.kernel.all_ins_met_index):
                        self.done = True
                    return True
                else:
                    return False
        return False

    def dma_done(self):
        self.dma_blocked = False
        self.index += 1

    def thread_is_done(self):
        return self.done


class CostModel:

    def __init__(self, thread_count, kernel):
        self.kernel = kernel
        self.thread_count = thread_count
        self.pipeline = Pipeline(14)
        self.dma = DMA()
        self.threads = [
            Thread(x, thread_count, kernel, self.pipeline, self.dma)
            for x in range(thread_count)
        ]

        self.add_dma_lut = createCustomAddDMADict()

        self.last_thread_called = 0

    def cycle(self):
        curr_thread = self.threads[self.last_thread_called]
        prev = self.last_thread_called
        while True:
            if not curr_thread.done:
                res = curr_thread.exec()
                if res:
                    return
            self.last_thread_called = (self.last_thread_called + 1) % self.thread_count
            curr_thread = self.threads[self.last_thread_called]
            if self.last_thread_called == prev:
                return

    def all_threads_done(self):
        for thread in self.threads:
            if not thread.thread_is_done():
                return False
        return True

    def start(self):
        while not self.all_threads_done():
            self.cycle()
            self.pipeline.cycle()
            self.dma.cycle()

        return (self.pipeline.curr_cycle * (self.kernel.ins_list[1])) / freq
