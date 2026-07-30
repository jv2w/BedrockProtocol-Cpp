"""Checks that a packet's decode and encode paths perform mirrored operations.

The most damaging porting mistake is an asymmetry between decodePayload() and encodePayload():
the packet still compiles and often still decodes, but re-encoding it silently corrupts the
stream. This tool normalises both bodies to a sequence of canonical operation names and reports
where they diverge.

It is a heuristic, not a proof - conditional branches and loops are compared structurally, so a
report is a prompt to look, not necessarily a defect.

Usage:
    python tools/check_symmetry.py                 # every packet
    python tools/check_symmetry.py TextPacket ...  # named packets only
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

SRC_DIR = Path(__file__).resolve().parent.parent / "src" / "protocol"

# Reader call -> canonical operation, and the writer call that must mirror it.
READ_TO_OP = {
    "readUnsigned": "byte",
    "readSigned": "sbyte",
    "readUnsignedShort": "u16",
    "readSignedShort": "i16",
    "readUnsignedInt": "u32",
    "readSignedInt": "i32",
    "readUnsignedLong": "u64",
    "readSignedLong": "i64",
    "readFloat": "f32",
    "readDouble": "f64",
}
WRITE_TO_OP = {
    "writeUnsigned": "byte",
    "writeSigned": "sbyte",
    "writeUnsignedShort": "u16",
    "writeSignedShort": "i16",
    "writeUnsignedInt": "u32",
    "writeSignedInt": "i32",
    "writeUnsignedLong": "u64",
    "writeSignedLong": "i64",
    "writeFloat": "f32",
    "writeDouble": "f64",
}

CALL_RE = re.compile(r"(?:(\w+)::)?(\w+)\s*\(")
# Structural keywords are kept in the sequence so that branches must match up too.
STRUCTURE_RE = re.compile(r"^\s*(if|else if|else|for|while|switch|case|default|\})")


def extract_body(source: str, signature: str) -> str | None:
    """Returns the body of the named method, or None when it is absent."""
    start = source.find(signature)
    if start < 0:
        return None
    brace = source.find("{", start)
    depth = 0
    for i in range(brace, len(source)):
        if source[i] == "{":
            depth += 1
        elif source[i] == "}":
            depth -= 1
            if depth == 0:
                return source[brace + 1 : i]
    return None


def canonical_ops(body: str, is_decode: bool) -> list[str]:
    """Reduces a method body to the sequence of wire operations and control-flow markers it performs."""
    table = READ_TO_OP if is_decode else WRITE_TO_OP
    ops: list[str] = []
    for line in body.splitlines():
        stripped = line.strip()
        if not stripped or stripped.startswith("//"):
            continue

        structure = STRUCTURE_RE.match(line)
        marker = "<" + structure.group(1).strip() + ">" if structure else None

        # A closing brace precedes nothing; every other marker follows the calls in its own
        # condition or loop header, which run before the block is entered. This matters because
        # decode reads a loop's element count inside the for-init while encode writes it beforehand.
        if marker == "<}>":
            ops.append(marker)
            marker = None

        for scope, name in CALL_RE.findall(stripped):
            if name in table:
                ops.append(table[name])
            elif name == ("read" if is_decode else "write"):
                # A self-serializing type: Foo::read(in) mirrors value.write(out).
                ops.append("obj")
            elif is_decode and (name.startswith("get") or name.startswith("read")):
                ops.append(normalise_composite(name, is_decode))
            elif not is_decode and (name.startswith("put") or name.startswith("write")):
                ops.append(normalise_composite(name, is_decode))

        if marker is not None:
            ops.append(marker)
    return ops


# Operations that are deliberately asymmetric: NBT is decoded into a tag tree but written back from
# CacheableNbt's memoised bytes, so the two sides name different functions for the same field.
EQUIVALENT_OPS = {
    "nbtroot": "nbt",
    "nbtcompoundroot": "nbt",
    "bytearray": "nbt",
    # getVector3 is paired with putVector3Nullable wherever the field is optional on write only.
    "vector3nullable": "vector3",
}


def normalise_composite(name: str, is_decode: bool) -> str:
    """Strips the get/put (or read/write) prefix so both sides reduce to the same token."""
    for prefix in ("get", "put", "read", "write"):
        if name.startswith(prefix):
            stripped = name[len(prefix) :].lower()
            return EQUIVALENT_OPS.get(stripped, stripped)
    lowered = name.lower()
    return EQUIVALENT_OPS.get(lowered, lowered)


def check_file(path: Path) -> list[str]:
    source = path.read_text(encoding="utf-8")
    name = path.stem

    decode = extract_body(source, f"void {name}::decodePayload")
    encode = extract_body(source, f"void {name}::encodePayload")
    if decode is None or encode is None:
        return [f"{name}: could not locate decodePayload/encodePayload"]

    decode_ops = canonical_ops(decode, True)
    encode_ops = canonical_ops(encode, False)
    if decode_ops == encode_ops:
        return []

    problems = [f"{name}: decode/encode sequences differ"]
    for i in range(max(len(decode_ops), len(encode_ops))):
        d = decode_ops[i] if i < len(decode_ops) else "<missing>"
        e = encode_ops[i] if i < len(encode_ops) else "<missing>"
        if d != e:
            problems.append(f"    step {i}: decode {d!r} vs encode {e!r}")
            break
    return problems


def main() -> int:
    targets = sys.argv[1:]
    files = (
        [SRC_DIR / f"{t}.cpp" for t in targets]
        if targets
        else sorted(p for p in SRC_DIR.glob("*Packet.cpp") if p.stem != "DataPacket")
    )

    reported = 0
    for path in files:
        if not path.exists():
            print(f"missing: {path}")
            reported += 1
            continue
        for line in check_file(path):
            print(line)
            reported += 1

    total = len(files)
    if reported == 0:
        print(f"[OK] {total} packets: decode and encode are symmetric")
        return 0
    print(f"\n[REVIEW] {total} packets checked, {reported} lines reported above")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
