"""Standard, textbook natural-loop detection over a CFG (DFS back-edges +
reverse-reachability), the same technique any compiler uses -- nothing
kernel-specific here.

1. DFS from the entry block, coloring nodes white/gray/black. An edge
   u -> v where v is gray (still on the DFS stack, i.e. an ancestor of u)
   is a back-edge; back-edges are exactly what defines a natural loop in a
   reducible CFG (which straight-line compiled C loops always are).
2. For a back-edge (tail -> head), the loop body is {head} union every
   node that can reach tail via edges that stay inside the graph without
   passing back OUT through head -- computed by walking predecessors
   backward from tail, stopping at head.
3. Multiple back-edges sharing the same header describe one loop (e.g. a
   loop with more than one continue/latch) -- merged by header.
4. Nesting: loop A is nested inside loop B if A's node set is a strict
   subset of B's. "Innermost" loops are the ones with no other loop
   strictly nested inside them.
"""

from dataclasses import dataclass, field


@dataclass
class Loop:
    header: int
    nodes: set
    back_edge_tails: set = field(default_factory=set)

    @property
    def size(self):
        return len(self.nodes)


def find_back_edges(blocks, entry_id):
    WHITE, GRAY, BLACK = 0, 1, 2
    color = {bid: WHITE for bid in blocks}
    back_edges = []  # list of (tail, head)

    def dfs(u):
        color[u] = GRAY
        for v in sorted(blocks[u].successors):
            if color[v] == WHITE:
                dfs(v)
            elif color[v] == GRAY:
                back_edges.append((u, v))
            # BLACK v => forward/cross edge, not a loop indicator here
        color[u] = BLACK

    dfs(entry_id)
    # Any block unreachable from entry (shouldn't normally happen for a
    # real function, but be defensive) is simply never visited/back-edged.
    return back_edges


def natural_loop_nodes(blocks, tail, head):
    """Nodes reachable backward from `tail` without leaving through `head`,
    i.e. the standard natural-loop-body construction for back-edge tail->head.

    Critically, `head` itself is never expanded (its own predecessors are
    entry edges INTO the loop from outside, not loop-body members) -- only
    `tail`, and transitively whatever else is reachable from it, gets
    walked. For a tight self-loop (head == tail) this correctly yields just
    {head} with nothing pulled in from outside."""
    nodes = {head, tail}
    stack = [] if tail == head else [tail]
    while stack:
        n = stack.pop()
        for pred in blocks[n].predecessors:
            if pred not in nodes:
                nodes.add(pred)
                stack.append(pred)
    return nodes


def find_loops(blocks, entry_id):
    back_edges = find_back_edges(blocks, entry_id)
    loops_by_header = {}
    for tail, head in back_edges:
        nodes = natural_loop_nodes(blocks, tail, head)
        if head in loops_by_header:
            loops_by_header[head].nodes |= nodes
            loops_by_header[head].back_edge_tails.add(tail)
        else:
            loops_by_header[head] = Loop(header=head, nodes=nodes, back_edge_tails={tail})
    return list(loops_by_header.values())


def nesting_depth(loop, all_loops):
    """1 = outermost. Counts how many OTHER loops strictly contain this one."""
    return 1 + sum(
        1
        for other in all_loops
        if other is not loop and loop.nodes < other.nodes
    )


def innermost_loops(loops):
    return [
        loop
        for loop in loops
        if not any(other is not loop and other.nodes < loop.nodes for other in loops)
    ]


def instruction_count(blocks, loop):
    return sum(len(blocks[n].instructions) for n in loop.nodes)


# UPMEM's two MRAM<->WRAM DMA transfer mnemonics -- the only two
# instructions on this ISA that move bulk data, as opposed to the
# register-level ALU/load/store ops every other mnemonic represents.
DMA_MNEMONICS = {"ldma", "sdma"}


def loop_has_dma(blocks, loop):
    """True if this loop's own body -- or a loop nested inside it, whose
    blocks are already included in `loop.nodes` -- issues a DMA transfer
    directly. A loop nested k levels deep inside a chunk loop still
    contributes its blocks to every ancestor loop's node set, so checking
    `loop.nodes` alone already covers "anywhere in this loop's transitive
    body", no separate recursion into child loops needed."""
    return any(
        ins.mnemonic in DMA_MNEMONICS
        for n in loop.nodes
        for ins in blocks[n].instructions
    )


def _pick_by_depth(blocks, candidates, prefer_min):
    depths = {id(l): nesting_depth(l, candidates) for l in candidates}
    target_depth = min(depths.values()) if prefer_min else max(depths.values())
    tied = [l for l in candidates if depths[id(l)] == target_depth]
    tied.sort(key=lambda l: instruction_count(blocks, l), reverse=True)
    return tied[0]


def pick_main_loop(blocks, loops):
    """Among all detected loops, pick the one most likely to be "the" main
    loop of the kernel -- the periodic unit of work someone means when they
    say "the main loop", not just any loop that happens to be deeply nested.

    Every kernel in this codebase follows the same chunk-loop-contains-
    element-loop shape: an outer loop does one mram_read/mram_write (DMA)
    per iteration, wrapping an inner, DMA-free loop that does the actual
    per-element arithmetic. Naively preferring the deepest nesting picks
    the compute-only fragment and silently drops the DMA -- correct as A
    loop, but not a meaningful stand-alone "main loop" for a kernel whose
    cost is data-movement-plus-compute.

    So: among loops that contain a DMA instruction anywhere in their
    transitive body, prefer the OUTERMOST one (smallest nesting depth) --
    the smallest loop that still captures both the transfer and whatever's
    nested inside it. Ties (disjoint top-level loops that both have DMA,
    e.g. Scan's separate reduce/scan phases) are broken by total
    instruction count, preferring the structurally richer one.

    Only falls back to "deepest nesting, no DMA requirement" for a
    pure-compute function with no memory traffic at all (so the tool still
    returns something sensible rather than nothing)."""
    if not loops:
        return None
    dma_loops = [l for l in loops if loop_has_dma(blocks, l)]
    if dma_loops:
        return _pick_by_depth(blocks, dma_loops, prefer_min=True)
    return _pick_by_depth(blocks, loops, prefer_min=False)
