from configuration import *
import csv
from support import *
from load_lut import *
import threading


class Kernel:
    def __init__(self, ins_list):
        self.ins_list = ins_list
        self.flatten_list = self.flatIns(ins_list[0], ins_list[1])

    def flatIns(self, input_list, iteration):
        flatten_list = []
        for i in range(iteration):
            for ins in input_list:
                if type(ins) == tuple and type(ins[0]) != InsType:
                    flatten_list += self.flatIns(ins[0], ins[1])
                else:
                    flatten_list.append(ins)
        return flatten_list


def createKernel(iterations):
    luts = loadLUTs()
    BUFFER_SIZE = iterations[1] * data_type_size(DataType.INT32)
    funcInsLUT = luts[LUTType.FuncInsLut]
    baseInsLUT = luts[LUTType.BaseInsLUT]
    dmaLUT = luts[LUTType.DmaLUT]
    ins_list = (
        [
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.MOV],
            baseInsLUT[InsType.LSR],
            baseInsLUT[InsType.LSL],
            baseInsLUT[InsType.MOV],
            (InsType.LDMA, BUFFER_SIZE),
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.MOV],
            baseInsLUT[(InsType.ADD, DataType.INT32)],
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.J],
            baseInsLUT[InsType.J],
            (
                [
                    baseInsLUT[InsType.MOV],
                    baseInsLUT[(InsType.ADD, DataType.INT32)],
                    baseInsLUT[InsType.LSL],
                    baseInsLUT[(InsType.ADD, DataType.INT32)],
                    baseInsLUT[(InsType.S, DataType.INT32)],
                    baseInsLUT[InsType.J],
                ],
                iterations[1],
            ),
        ],
        iterations[0],
    )
    kernel = Kernel(ins_list)
    return kernel


def loadTable():
    file = open(custom_dma_file, newline="")
    reader = csv.reader(file, delimiter=",", quotechar="|")
    next(reader)
    lut = {}
    for row in reader:
        custom_range = int(row[0])
        buffer_size = int(row[1])
        thread = int(row[2])
        latency = -1
        cycle = -1
        if len(row[3]) > 1:
            latency = float(row[3])
            cycle = latency * freq
        if custom_range not in lut:
            lut[custom_range] = {}
        if buffer_size not in lut[custom_range]:
            lut[custom_range][buffer_size] = {}
        if thread not in lut[custom_range][buffer_size]:
            lut[custom_range][buffer_size][thread] = {}

        lut[custom_range][buffer_size][thread] = cycle
    return lut


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
        self.busy = False
        self.process_req = []

    def available(self):
        if len(self.process_req) <= 1:
            return True
        else:
            return False

    def add(self, time, thread):
        self.process_req.append((time, thread))

    def cycle(self):
        new_list = []
        for req in self.process_req:
            time = req[0] - 1
            thread = req[1]
            if time != 0:
                new_list.append((time, thread))
            else:
                thread.dma_done()
        self.process_req = new_list


class Thread:
    def __init__(self, id, thread_count, kernel, pipeline, dma, dma_time):
        self.id = id
        self.thread_count = thread_count
        self.kernel = kernel
        self.pipeline = pipeline
        self.index = 0
        self.done = False
        self.dma_blocked = False
        self.dma = dma
        self.dma_time = dma_time

    def exec(self):
        ins = self.kernel.flatten_list[self.index]
        if not self.dma_blocked:
            if type(ins) == tuple:
                dma_lat = self.dma_time

                if self.dma.available():
                    self.dma.add(dma_lat, self)
                    self.dma_blocked = True
                    self.index += 1
                    return True
                else:
                    return False

            else:
                if self.pipeline.can_put(self.id):
                    self.pipeline.put(self.id)
                    self.index += 1
                    if (self.index) >= len(self.kernel.flatten_list):
                        self.done = True
                    return True
                else:
                    return False
        return False

    def dma_done(self):
        self.dma_blocked = False


class Simulator:

    def __init__(
        self,
        thread_count,
        kernel,
        dma_time,
        custom_range,
        buffer_size,
        threads_lock,
        result_dict,
    ):
        self.thread_count = thread_count
        self.pipeline = Pipeline(14)
        self.dma = DMA()
        self.threads = [
            Thread(x, thread_count, kernel, self.pipeline, self.dma, dma_time)
            for x in range(thread_count)
        ]
        self.last_thread_called = 0
        self.threads_lock = threads_lock
        self.result_dict = result_dict
        self.custom_range = custom_range
        self.buffer_size = buffer_size

    def cycle(self):
        done_threads = []
        curr_thread = self.threads[self.last_thread_called]
        prev = self.last_thread_called
        while True:
            if curr_thread.done:
                done_threads.append(curr_thread)
            else:
                res = curr_thread.exec()
                if res:
                    return False
            self.last_thread_called = (self.last_thread_called + 1) % self.thread_count
            curr_thread = self.threads[self.last_thread_called]
            if self.last_thread_called == prev:
                return len(done_threads) == self.thread_count

    def start(self):
        while not self.cycle():
            self.pipeline.cycle()
            self.dma.cycle()
        with self.threads_lock:
            self.result_dict[self.custom_range][self.thread_count][
                self.buffer_size
            ] = self.pipeline.curr_cycle
        return self.pipeline.curr_cycle


buffer_size = 8
thread_count = 1


def find_best_cycle(
    in_kernel, thread, real_time, custom_range, buffer_size, threads_lock, result_dict
):
    start = 10
    end = 1000
    while abs(start - end) > 2:
        mid = int((start + end) / 2)
        simulator = Simulator(
            thread, in_kernel, mid, custom_range, buffer_size, threads_lock, result_dict
        )
        time = simulator.start()

        if real_time > time:
            start = mid
        else:
            end = mid
        print("real time", real_time, "calculated", time)
    print(
        "Done",
    )
    return start


executionTable = loadTable()
calculated_lut = {}

allocated_threads = []
threads_lock = threading.Lock()

for custom_range in custom_ranges:
    calculated_lut[custom_range] = {}
    for thread in thread_range:
        calculated_lut[custom_range][thread] = {}
        for buffer_size in buffer_sizes:
            real_time = executionTable[custom_range][buffer_size][thread]
            if real_time != -1:
                calculated_lut[custom_range][thread][buffer_size] = 0
                iterations = [iteration, custom_range]
                instructionKernel = createKernel(iterations)
                allocated_thread = threading.Thread(
                    target=find_best_cycle,
                    args=[
                        instructionKernel,
                        thread,
                        real_time,
                        custom_range,
                        buffer_size,
                        threads_lock,
                        calculated_lut,
                    ],
                )
                allocated_thread.start()
                allocated_threads.append(allocated_thread)

for allocated_thread in allocated_threads:
    allocated_thread.join()

print(calculated_lut)
