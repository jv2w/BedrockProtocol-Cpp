#!/usr/bin/env python3
"""Flags expressions that read the wire twice without a sequence point between the reads.

Why this check exists
---------------------
PHP evaluates function arguments strictly left to right, so

    $this->vehicleRotation = new Vector2(LE::readFloat($in), LE::readFloat($in));

reads X then Y. Transcribed verbatim into C++ the same expression is UNSEQUENCED: the standard lets
the compiler evaluate the two arguments in either order, and MSVC picks right-to-left. That silently
transposed X and Y on every decode of CorrectPlayerMovePredictionPacket, and nothing but a
byte-level round-trip would ever have noticed - both fields are floats of the same width, so the
packet decoded, re-encoded and looked entirely healthy.

The fix is to sequence the reads into named locals:

    const float x = LE::readFloat(in);
    const float y = LE::readFloat(in);
    vehicleRotation = math::Vector2(x, y);

Operators that DO sequence their operands - ?: , && , || , the comma operator - are excluded, since
those are well-defined and appear legitimately in the port.

Usage:  python tools/check_eval_order.py [path ...]     (defaults to src/)
Exit code 0 when clean, 1 when something needs sequencing.
"""

import pathlib
import re
import sys

# A call argument list holding two reads from the same reader, with a comma between them and no
# statement boundary in between. Restricted to a single statement by excluding ; { }.
UNSEQUENCED = re.compile(r"\w+\s*\([^;{}]*\(\s*(?:in|reader)\s*\)[^;{}]*,[^;{}]*\(\s*(?:in|reader)\s*\)", re.S)

# Sequenced by the language; a hit inside one of these is fine.
SEQUENCED_OPERATORS = ("?", "&&", "||")


def statement_is_sequenced(text: str) -> bool:
    return any(op in text for op in SEQUENCED_OPERATORS)


def scan(path: pathlib.Path) -> list[tuple[int, str]]:
    source = path.read_text(encoding="utf-8", errors="replace")
    findings = []
    for match in UNSEQUENCED.finditer(source):
        snippet = match.group(0)
        if statement_is_sequenced(snippet):
            continue
        line = source.count("\n", 0, match.start()) + 1
        findings.append((line, " ".join(snippet.split())[:140]))
    return findings


def main(argv: list[str]) -> int:
    roots = [pathlib.Path(a) for a in argv[1:]] or [pathlib.Path("src")]
    total = 0
    for root in roots:
        files = sorted(root.rglob("*.cpp")) if root.is_dir() else [root]
        for path in files:
            for line, snippet in scan(path):
                total += 1
                print(f"{path}:{line}: unsequenced multi-read -- {snippet}")

    if total == 0:
        print("[OK] check_eval_order: no unsequenced multi-read expressions")
        return 0
    print(f"\n[FAIL] check_eval_order: {total} expression(s) read the wire twice without sequencing.")
    print("Split the reads into named locals so the order is defined.")
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
