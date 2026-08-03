"""Builds a control-flow graph (basic blocks + edges) from parsed assembly
lines, generically -- no hardcoded mnemonic list for "which instructions
branch". Works like this:

1. Collect every label actually DEFINED in the function.
2. For every instruction, check whether any of its operands textually
   matches one of those defined labels. If so, this instruction transfers
   control there -- regardless of mnemonic. This one rule covers both:
     - plain branches: `jump .LBB0_9`, `jeq r1, 0, .LBB0_4`
     - UPMEM's fused compare/move+branch forms, where the target label is
       just the trailing operand of an otherwise-ordinary-looking
       instruction: `add r6, r6, -1, nz, .LBB0_9`, `move r2, 0, true, .LBB0_2`
   An instruction is treated as unconditional (no fall-through edge) only
   if its mnemonic is exactly "jump", or one of its operands is the literal
   token "true" (UPMEM's asm printer's way of marking an always-taken
   fused branch) -- otherwise it's conditional and gets BOTH a branch edge
   and a fall-through edge.
3. Split into basic blocks at every referenced label and after every
   branching instruction, so no block has a branch/label in its interior.
"""

from dataclasses import dataclass, field


@dataclass
class BasicBlock:
    id: int
    label: str  # the real label this block starts at, or a synthetic name
    instructions: list = field(default_factory=list)  # list of Instruction
    successors: set = field(default_factory=set)  # block ids
    predecessors: set = field(default_factory=set)  # block ids

    @property
    def is_synthetic_label(self):
        return self.label.startswith("__bb")


UNCONDITIONAL_MNEMONICS = {"jump"}
UNCONDITIONAL_MARKER_OPERAND = "true"


def _branch_targets(instruction, defined_labels):
    return [op for op in instruction.operand_tokens() if op in defined_labels]


def _is_unconditional(instruction):
    if instruction.mnemonic in UNCONDITIONAL_MNEMONICS:
        return True
    return UNCONDITIONAL_MARKER_OPERAND in instruction.operand_tokens()


def build_cfg(lines, entry_label=None):
    """lines: list of Line from asm_parser.parse_lines(), already scoped to one
    function body. Returns (blocks: dict[int, BasicBlock], entry_id: int,
    label_to_block: dict[str, int])."""
    defined_labels = {ln.label for ln in lines if ln.label is not None}

    # Pass 1: figure out which labels are actually branch targets (as
    # opposed to labels that exist only for debug/exception-table purposes
    # and are never referenced as an operand anywhere) -- only those force
    # a block split.
    referenced_labels = set()
    for ln in lines:
        if ln.instruction is not None:
            referenced_labels.update(_branch_targets(ln.instruction, defined_labels))

    blocks = {}
    label_to_block = {}
    entry_id = None
    synth_counter = 0

    def new_block(label):
        nonlocal synth_counter
        bid = len(blocks)
        if label is None:
            label = f"__bb{synth_counter}"
            synth_counter += 1
        blk = BasicBlock(id=bid, label=label)
        blocks[bid] = blk
        label_to_block[label] = bid
        return blk

    current = None
    # Non-referenced labels (e.g. debug-only .Ltmp markers) seen since the
    # last block boundary -- nothing branches to them directly, but they
    # sit at the same code position as whatever block starts next, so they
    # need to resolve to that block's id once it exists.
    alias_queue = []

    def flush_aliases(bid):
        for alias in alias_queue:
            label_to_block[alias] = bid
        alias_queue.clear()

    for ln in lines:
        if ln.label is not None:
            if ln.label in referenced_labels:
                # A real branch target forces a block boundary here,
                # whether or not `current` already has instructions --
                # falling through into a labeled block is itself a real
                # edge (handled by pass 2's adjacency fallback below).
                current = new_block(ln.label)
                flush_aliases(current.id)
                if entry_id is None:
                    entry_id = current.id
                    if entry_label is None:
                        entry_label = ln.label
            else:
                # Nothing branches here -- just remember it so it aliases
                # to whichever block ends up starting at this position.
                alias_queue.append(ln.label)
            continue

        if current is None:
            current = new_block(None)
            flush_aliases(current.id)
            if entry_id is None:
                entry_id = current.id
        else:
            flush_aliases(current.id)

        current.instructions.append(ln.instruction)

        targets = _branch_targets(ln.instruction, defined_labels)
        if targets:
            # This instruction ends the current block. Successors are
            # resolved to block ids in a second pass (labels may point
            # forward to blocks not created yet).
            current.successors.update(("label", t) for t in targets)
            if not _is_unconditional(ln.instruction):
                current.successors.add(("fallthrough", None))
            current = None  # next line starts a brand-new block

    # Pass 2: resolve deferred successor markers into real block ids, and
    # fill in "fallthrough" markers using file order once every block's
    # start-of-file position is known.
    ordered_ids = list(blocks.keys())
    for idx, bid in enumerate(ordered_ids):
        blk = blocks[bid]
        resolved = set()
        for kind, val in blk.successors:
            if kind == "label":
                resolved.add(label_to_block[val])
            elif kind == "fallthrough":
                if idx + 1 < len(ordered_ids):
                    resolved.add(ordered_ids[idx + 1])
        blk.successors = resolved
        # A block with instructions but no branch at all still falls
        # through to the next block (e.g. the last block before a
        # `return`/epilogue, where the "branch" is really just running off
        # the end into the next label with no explicit jump -- rare in
        # this dialect but handled for robustness).
        if not blk.successors and blk.instructions and idx + 1 < len(ordered_ids):
            last = blk.instructions[-1]
            if not _branch_targets(last, defined_labels):
                blk.successors.add(ordered_ids[idx + 1])

    for bid, blk in blocks.items():
        for succ in blk.successors:
            blocks[succ].predecessors.add(bid)

    return blocks, entry_id, label_to_block
