#!/usr/bin/env python3
"""Generic DPU assembly main-loop extractor.

Given a compiled .s file, finds the function's control-flow graph, detects
every natural loop in it (standard DFS back-edge analysis -- see
loop_detect.py), and picks out the "main" loop: the innermost, highest
instruction-count loop, which is the same thing we've been finding by hand
all session (the hot per-element loop nested inside chunk/DMA loops).

This is a standalone analysis tool, not wired into the Predictor pipeline --
it just extracts and writes out what it finds.

Usage:
    python3 extract_loop.py path/to/file.s [-o output/dir] [--function main]
    python3 extract_loop.py path/to/directory [-o output/dir]   # batch mode, recurses for *.s
"""

import argparse
import sys
from pathlib import Path

from asm_parser import parse_lines, extract_function_body
from cfg import build_cfg
from loop_detect import find_loops, nesting_depth, instruction_count, pick_main_loop


def analyze_file(path, function_name=None):
    text = path.read_text()
    resolved_name, body_text = extract_function_body(text, function_name)
    lines = parse_lines(body_text)
    blocks, entry_id, label_to_block = build_cfg(lines)
    loops = find_loops(blocks, entry_id)
    main_loop = pick_main_loop(blocks, loops)
    return {
        "path": path,
        "function": resolved_name,
        "blocks": blocks,
        "entry_id": entry_id,
        "loops": loops,
        "main_loop": main_loop,
    }


def render_block(blocks, bid):
    blk = blocks[bid]
    out = [f"{blk.label}:"]
    for ins in blk.instructions:
        out.append(f"\t{ins.mnemonic} {', '.join(ins.operands)}".rstrip())
    return out


def render_loop_summary(result):
    blocks = result["blocks"]
    loops = result["loops"]
    lines = [
        f"# Source: {result['path']}",
        f"# Function analyzed: {result['function']}",
        f"# Total basic blocks: {len(blocks)}",
        f"# Loops detected: {len(loops)}",
        "",
    ]
    for loop in sorted(loops, key=lambda l: nesting_depth(l, loops)):
        depth = nesting_depth(loop, loops)
        header_label = blocks[loop.header].label
        ins_count = instruction_count(blocks, loop)
        is_main = loop is result["main_loop"]
        marker = "  <-- extracted as main loop" if is_main else ""
        lines.append(
            f"# Loop header={header_label} nesting_depth={depth} "
            f"blocks={sorted(loop.nodes)} instructions={ins_count}{marker}"
        )
    return "\n".join(lines)


def render_main_loop(result):
    blocks = result["blocks"]
    main_loop = result["main_loop"]
    if main_loop is None:
        return "# No loop detected in this function.\n"

    out = [render_loop_summary(result), "", "# --- extracted main loop body ---", ""]
    for bid in sorted(main_loop.nodes):
        out.extend(render_block(blocks, bid))
        out.append("")
    return "\n".join(out)


def process_one(path, out_dir, function_name=None):
    try:
        result = analyze_file(path, function_name)
    except Exception as e:
        print(f"[skip] {path}: {e}", file=sys.stderr)
        return False

    out_dir.mkdir(parents=True, exist_ok=True)
    out_path = out_dir / f"{path.stem}_mainloop.txt"
    out_path.write_text(render_main_loop(result))

    n_loops = len(result["loops"])
    main = result["main_loop"]
    main_desc = (
        f"header={result['blocks'][main.header].label} "
        f"instructions={instruction_count(result['blocks'], main)}"
        if main
        else "none found"
    )
    print(f"{path} -> {out_path}  ({n_loops} loop(s), main: {main_desc})")
    return True


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("input", help="A .s file, or a directory to scan for *.s files")
    ap.add_argument("-o", "--output", default="output", help="Output directory (default: ./output)")
    ap.add_argument("--function", default=None, help="Function name to analyze (default: auto-detect)")
    args = ap.parse_args()

    in_path = Path(args.input)
    out_dir = Path(args.output)

    if in_path.is_dir():
        s_files = sorted(in_path.rglob("*.s"))
        if not s_files:
            print(f"No .s files found under {in_path}", file=sys.stderr)
            sys.exit(1)
        ok = 0
        for f in s_files:
            if process_one(f, out_dir, args.function):
                ok += 1
        print(f"\n{ok}/{len(s_files)} files processed successfully.")
    else:
        if not process_one(in_path, out_dir, args.function):
            sys.exit(1)


if __name__ == "__main__":
    main()
