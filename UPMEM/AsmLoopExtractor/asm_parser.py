"""Generic parser for UPMEM DPU assembly (.s) files.

Turns a raw .s file into a flat list of Line objects (labels vs.
instructions vs. everything else we don't care about for control-flow
purposes), scoped to a single function's body. Nothing here is specific to
any one kernel -- it only relies on standard LLVM assembly-printer syntax
(labels end in ':', directives start with '.', comments start with '//'),
which every kernel's dpu.s / task.s in this repo shares.
"""

import re
from dataclasses import dataclass, field


LABEL_RE = re.compile(r"^([A-Za-z_$.][\w$.]*):\s*(//.*)?$")
DIRECTIVE_RE = re.compile(r"^\.")
COMMENT_ONLY_RE = re.compile(r"^//")
FUNC_TYPE_RE = re.compile(r"^\.type\s+([A-Za-z_$.][\w$.]*),\s*@function")
FUNC_SIZE_RE = re.compile(r"^\.size\s+([A-Za-z_$.][\w$.]*)\s*,")

# A bare token that "looks like" a local/global symbol reference -- used
# only as a last-resort candidate filter before we confirm it's an actual
# defined label in this function (see cfg.py). Deliberately permissive: we
# don't try to guess a naming convention, we just need "could plausibly be
# an identifier operand" so we don't waste time regex-matching operands
# that are obviously registers/immediates.
IDENT_OPERAND_RE = re.compile(r"^[A-Za-z_$.][\w$.]*$")


@dataclass
class Instruction:
    line_no: int
    raw: str
    mnemonic: str
    operands: list = field(default_factory=list)

    def operand_tokens(self):
        """Every operand, stripped of whitespace -- one string per comma-separated field."""
        return [op.strip() for op in self.operands]


@dataclass
class Line:
    line_no: int
    raw: str
    label: str = None  # set if this line is (only) a label
    instruction: Instruction = None  # set if this line is a real instruction


def _strip_comment(text):
    # Split on the first "//" that isn't inside the instruction's own
    # operand text -- for this assembly dialect "//" never legitimately
    # appears inside an operand, so a plain split is safe and simple.
    idx = text.find("//")
    if idx == -1:
        return text.rstrip()
    return text[:idx].rstrip()


def parse_lines(text):
    """First pass: classify every line as label / instruction / other (directive,
    blank, pure comment, debug pseudo-ops). Returns a list of Line objects for
    label + instruction lines only, in file order, with 1-based line numbers
    preserved from the original file for traceability."""
    lines = []
    for i, raw_line in enumerate(text.splitlines(), start=1):
        stripped = raw_line.strip()
        if not stripped:
            continue
        if COMMENT_ONLY_RE.match(stripped):
            continue
        m = LABEL_RE.match(stripped)
        if m:
            lines.append(Line(line_no=i, raw=raw_line, label=m.group(1)))
            continue
        if DIRECTIVE_RE.match(stripped):
            continue
        # Anything else on a non-empty, non-comment, non-directive,
        # non-label line is a real instruction.
        code = _strip_comment(stripped)
        if not code:
            continue
        parts = code.split(None, 1)
        mnemonic = parts[0]
        operands = []
        if len(parts) > 1:
            operands = [op.strip() for op in parts[1].split(",")]
        lines.append(
            Line(
                line_no=i,
                raw=raw_line,
                instruction=Instruction(
                    line_no=i, raw=raw_line, mnemonic=mnemonic, operands=operands
                ),
            )
        )
    return lines


def extract_function_body(text, function_name=None):
    """Slices the raw .s text down to just the body of one function, found via
    its `.type NAME,@function` / `.size NAME,...` bracketing (standard for
    every ELF assembly this toolchain emits) -- not by assuming the function
    is named "main", though that's what every kernel here happens to use.

    If function_name is None, uses the first `.type X,@function` found.
    Returns (function_name, body_text)."""
    lines = text.splitlines()
    start_idx = None
    end_idx = None
    resolved_name = function_name

    for i, raw_line in enumerate(lines):
        stripped = raw_line.strip()
        m = FUNC_TYPE_RE.match(stripped)
        if m and (function_name is None or m.group(1) == function_name):
            resolved_name = m.group(1)
            start_idx = i
            continue
        if resolved_name is not None and start_idx is not None and end_idx is None:
            m2 = FUNC_SIZE_RE.match(stripped)
            if m2 and m2.group(1) == resolved_name:
                end_idx = i
                break

    if start_idx is None:
        raise ValueError(
            f"Could not find a `.type X,@function` for "
            f"{'any function' if function_name is None else function_name!r}"
        )
    if end_idx is None:
        end_idx = len(lines)

    return resolved_name, "\n".join(lines[start_idx : end_idx + 1])
