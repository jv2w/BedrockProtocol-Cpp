"""Compares every packet's decode path against the Endstone protocol documentation for 1.26.40.

The other gates in this directory ask whether the port agrees with itself (round-trip), whether it
survives hostile bytes (malformed), and whether a handful of hand-written encodings are exact (wire
format). None of them can say whether a field the port has never questioned is the field the game
actually sends. This one can: it reduces both the published spec and the C++ decode path to the same
sequence of wire primitives and diffs them, for all 229 packets at once.

    set BP_SPEC_DIR=...\\protocol-docs-r26_u4
    python tools\\check_spec_census.py

BP_SPEC_DIR must point at a checkout of the r26_u4 branch of EndstoneMC/protocol-docs, i.e. a
directory holding packets/, types/ and enums/:

    https://codeload.github.com/EndstoneMC/protocol-docs/tar.gz/refs/heads/r26_u4

A difference is a question, not a verdict. Both documentation sources model several constructs in
ways that do not survive this reduction, and those show up here as differences even though the port
is right; SPEC_CENSUS_2168.md records which ones and why. Read a new difference as "go and check
this against a third source", and settle it against gophertunnel at the matching version before
changing anything.

Exits 0 always: the count is a measurement to track, not a pass/fail line.
"""
import difflib
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

if "BP_SPEC_DIR" not in os.environ:
    sys.exit(__doc__)

import spec_census_code as code_side  # noqa: E402
import spec_census_spec as spec_side  # noqa: E402

# Structural markers carry no bytes; the reduction keeps only what lands on the wire. Branch shape
# is dropped on purpose, because a regex read of C++ cannot recover it reliably enough to compare.
NOISE = {"(", ")", "{", "}", "|", "if{", "sw{", "opt{", "arr{", "case{", "map{"}

# Fixed-width signedness does not change the bytes, only the C++ type chosen to hold them, so it is
# folded away. Varint signedness DOES change the bytes (zigzag) and is kept.
FOLD = {}
for _w in ("8", "16le", "32le", "64le", "16be", "32be", "64be"):
    FOLD["u" + _w] = "x" + _w
    FOLD["i" + _w] = "x" + _w


def norm(seq):
    out = []
    for t in seq:
        if t in NOISE:
            continue
        if t.startswith("sw("):
            out.append(t[3:-1])
        elif t.startswith("arr("):
            inner = t[4:t.index(")")]
            out.append(inner if not inner.startswith("n=") else "FIXED")
        elif t.startswith("<") or t == ">":
            continue
        else:
            out.append(t)
    return [FOLD.get(t, t) for t in out]


def main():
    same, diff, report = [], [], []
    spec_tokens = matched_tokens = 0

    for name in sorted(spec_side.PACKETS):
        ops = code_side.packet_ops(name)
        if ops is None:
            report.append(f"{name}: no decodePayload found")
            continue
        cs = norm(ops)

        # Endstone's single "optional" flag covers both the presence-byte form and the
        # conditional-on-an-earlier-field form. Try both readings and keep the closer one.
        best = None
        for mode in (True, False):
            spec_side.OPT_BYTE[0] = mode
            cand = norm(spec_side.flatten_packet(name))
            sm = difflib.SequenceMatcher(a=cand, b=cs, autojunk=False)
            eq = sum(b.size for b in sm.get_matching_blocks())
            rank = (cand == cs, sm.ratio())
            if best is None or rank > best[0]:
                best = (rank, eq, cand, sm)
        _, eq, ss, sm = best
        spec_tokens += len(ss)
        matched_tokens += min(eq, len(ss))

        if ss == cs:
            same.append(name)
            continue
        diff.append((sm.ratio(), name))
        report.append("=" * 78)
        report.append(f"{name}   similarity={sm.ratio():.3f}  spec={len(ss)} code={len(cs)}")
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == "equal":
                continue
            report.append(f"  @{i1:<4} {tag:<8} spec[{i1}:{i2}]={ss[i1:i2]}")
            report.append(f"  {'':<5} {'':<8} code[{j1}:{j2}]={cs[j1:j2]}")

    total = len(same) + len(diff)
    out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "build", "spec_census.txt")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w", encoding="utf-8") as f:
        f.write("\n".join(report))

    print(f"Spec census against Endstone r26_u4 (Minecraft 1.26.40, protocol 2168)\n")
    print(f"  packets compared         {total}")
    print(f"  identical token sequence {len(same)}  ({100.0 * len(same) / total:.1f}%)")
    print(f"  differing                {len(diff)}")
    print(f"  spec wire tokens         {spec_tokens}")
    print(f"  tokens aligned with code {matched_tokens}  ({100.0 * matched_tokens / spec_tokens:.1f}%)")
    print(f"\n  per-packet differences written to {os.path.normpath(out)}")
    if diff:
        print("\n  closest agreement last:")
        for r, n in sorted(diff):
            print(f"    {r:.3f}  {n}")


main()
