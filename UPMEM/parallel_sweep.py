"""Shared worker-pool dispatcher for running hardware benchmark sweeps in parallel.

Each kernel's run_script/*.py sweeps a real UPMEM board serially by default (one
make clean && make && ./bin/host cycle per parameter combo). This module lets a
sweep script dispatch its combos across a bounded pool of worker processes
instead, while keeping each worker's build artifacts isolated from every other
concurrently-running worker.

Each worker process claims a persistent integer "slot" id (0..parallel_instances-1)
once, at startup, via get_slot(). A kernel's runKernelOnADPUS should read that slot
and build into bin_slot{N} / generated_headers_slot{N} so no two concurrent workers
ever touch the same build directory.

Slot ids are claimed via an exclusive, non-blocking flock on a lock file in
.parallel_sweep_locks/ (relative to the kernel's own root directory, since each
sweep script os.chdir()s there before calling run_parallel_sweep). This makes
slot uniqueness a filesystem-visible property enforced by the OS -- NOT just an
in-process counter -- so it stays correct even if two independent invocations of
the same kernel's sweep script end up running at once (e.g. a stray/duplicate
launch): a worker from the second invocation will simply wait for a slot to free
up rather than colliding with a worker from the first. flock locks are released
automatically by the OS the moment the holding process exits, crash or not, so
no explicit cleanup is needed.
"""
import csv
import fcntl
import os
import time
from concurrent.futures import ProcessPoolExecutor, as_completed

_slot_id = None
_slot_lock_fd = None

LOCK_DIR = ".parallel_sweep_locks"


def _acquire_slot(parallel_instances):
    global _slot_id, _slot_lock_fd
    os.makedirs(LOCK_DIR, exist_ok=True)
    while True:
        for i in range(parallel_instances):
            path = os.path.join(LOCK_DIR, f"slot{i}.lock")
            fd = os.open(path, os.O_CREAT | os.O_RDWR)
            try:
                fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError:
                os.close(fd)
                continue
            _slot_id = i
            _slot_lock_fd = fd  # kept open+locked for this worker's whole lifetime
            return
        time.sleep(0.5)  # every slot currently claimed -- wait and retry


def get_slot():
    """Returns this worker process's claimed slot id, or 0 outside a pool."""
    return 0 if _slot_id is None else _slot_id


def _load_existing_keys(output_path, header):
    """Reads output_path's data rows (if the file exists and its header
    matches) and returns the set of already-completed identifying keys --
    each row's columns up to (but excluding) the trailing latency/correct
    pair, as a tuple of strings. Returns None if the file doesn't exist, is
    empty, or its header doesn't match (caller should start fresh in that
    case rather than risk mixing incompatible schemas)."""
    if not os.path.exists(output_path) or os.path.getsize(output_path) == 0:
        return None
    with open(output_path, newline="") as f:
        rows = list(csv.reader(f))
    if not rows or rows[0] != header:
        return None
    key_len = len(header) - 2  # drop trailing latency, correct
    return {tuple(row[:key_len]) for row in rows[1:] if row}


def run_parallel_sweep(
    combos,
    worker_fn,
    header,
    output_path,
    parallel_instances,
    kernel_name="",
    combo_key_fn=None,
):
    """Runs worker_fn(combo) for every combo, across a pool of parallel_instances workers.

    Rows are written to output_path (CSV) as each combo finishes -- completion
    order, not sweep order, since every row is self-describing via its own
    parameter columns. A combo whose worker_fn raises is logged and skipped
    rather than aborting the rest of the sweep.

    If combo_key_fn is given (combo -> tuple of the same identifying values
    that appear as worker_fn's returned row, up to but excluding the trailing
    latency/correct columns) and output_path already holds a completed run
    with a matching header, combos already present there are skipped and new
    rows are appended rather than the file being truncated -- resuming a run
    that was previously stopped partway through.
    """
    tag = f"[{kernel_name}] " if kernel_name else ""

    existing_keys = _load_existing_keys(output_path, header) if combo_key_fn else None
    if existing_keys:
        original_count = len(combos)
        combos = [c for c in combos if tuple(str(x) for x in combo_key_fn(c)) not in existing_keys]
        skipped = original_count - len(combos)
        if skipped:
            print(f"{tag}resuming: {skipped}/{original_count} combos already in {output_path}, "
                  f"{len(combos)} remaining", flush=True)

    mode = "a" if existing_keys else "w"
    total = len(combos)

    with open(output_path, mode, newline="") as result_file:
        writer = csv.writer(result_file)
        if mode == "w":
            writer.writerow(header)
            result_file.flush()

        completed = 0
        with ProcessPoolExecutor(
            max_workers=parallel_instances,
            initializer=_acquire_slot,
            initargs=(parallel_instances,),
        ) as executor:
            futures = {executor.submit(worker_fn, combo): combo for combo in combos}
            for future in as_completed(futures):
                combo = futures[future]
                completed += 1
                try:
                    row = future.result()
                except Exception as exc:
                    print(f"{tag}[{completed}/{total}] FAILED combo={combo}: {exc}", flush=True)
                    continue
                if row is None:
                    continue
                writer.writerow(row)
                result_file.flush()
                print(f"{tag}[{completed}/{total}] done combo={combo}", flush=True)
