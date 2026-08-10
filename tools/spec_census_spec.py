"""Flatten Endstone r26_u4 packet specs into canonical wire-op token sequences.

Part of the spec census gate. Run tools/check_spec_census.py; see its docstring for the
Endstone protocol-docs checkout it needs.
"""

import json, os

SPEC = os.environ["BP_SPEC_DIR"]

OPT_BYTE = [True]

TYPES = {}
PACKETS = {}
ENUMS = {}
for d, store in (("types", TYPES), ("packets", PACKETS), ("enums", ENUMS)):
    p = os.path.join(SPEC, d)
    for fn in os.listdir(p):
        j = json.load(open(os.path.join(p, fn), encoding="utf-8"))
        store[j["name"]] = j

# Endstone primitive -> canonical token
PRIM = {
    "bool": "u8", "uint8": "u8", "int8": "i8",
    "uint16": "u16le", "int16": "i16le",
    "uint32": "u32le", "int32": "i32le",
    "uint64": "u64le", "int64": "i64le",
    "uint16_be": "u16be", "int16_be": "i16be",
    "uint32_be": "u32be", "int32_be": "i32be",
    "uint64_be": "u64be", "int64_be": "i64be",
    "float": "f32le", "double": "f64le",
    "float_be": "f32be", "double_be": "f64be",
    "uvarint32": "uvar32", "varint32": "var32",
    "uvarint64": "uvar64", "varint64": "var64",
    "string": "str",
}
# named spec types with a fixed, known wire shape -> token expansion
# Only for names the spec has no type file for. Everything with a file is expanded from the file.
ATOM = {
    "CompoundTag": ["nbt"],
    # Both doc sources collapse Bedrock's two block-position encodings into one "BlockPos" with
    # three signed varints. The game has a second form with an UNSIGNED Y (gophertunnel UBlockPos,
    # PMMP getBlockPosition), which is what most packets actually carry. The docs cannot tell the
    # two apart, so the majority form is assumed here and the minority sites show up as diffs.
    "BlockPos": ["var32", "uvar32", "var32"],
    "brstd::bitset<131>": ["bitset131"],
    "cereal::DynamicValue": ["opaque:DynamicValue"],
}


def prefix_token(p):
    return PRIM.get(p, "?" + str(p))


def expand_type(t, seen, out):
    """t: str, dict, or None (an empty switch case)."""
    if t is None:
        return
    if isinstance(t, dict):
        if "switch" in t:
            sw = t["switch"]
            out.append("sw(" + prefix_token(sw.get("type", "uint8")) + ")")
            for c in t["cases"]:
                out.append("case{")
                expand_type(c, seen, out)
                out.append("}")
            return
        if "key" in t:
            out.append("map{")
            expand_field({"name": "k", "type": t["key"]}, seen, out)
            expand_field({"name": "v", "type": t["value"]}, seen, out)
            out.append("}")
            return
        if "type" in t:  # nested repeat
            expand_field(t, seen, out)
            return
        out.append("?dict" + ",".join(sorted(t)))
        return
    if t in PRIM:
        out.append(PRIM[t])
        return
    if t in ATOM:
        out.extend(ATOM[t])
        return
    if t in TYPES:
        if t in seen:
            out.append("recurse:" + t)
            return
        for f in TYPES[t].get("fields", []) or []:
            expand_field(f, seen | {t}, out)
        return
    out.append("opaque:" + t)


def expand_field(f, seen, out):
    t = f.get("type")
    rep = f.get("repeat")
    opt = f.get("optional")
    if opt:
        # Endstone marks both "std::optional (presence byte + value)" and "conditional on an
        # earlier discriminator" with the same flag. The presence-byte reading is the common one,
        # so it is assumed here; sites that are really conditional surface as diffs.
        out.append("opt{")
        if OPT_BYTE[0]:
            out.append("u8")
    if rep:
        if "prefix" in rep:
            out.append("arr(" + prefix_token(rep["prefix"]) + "){")
        else:
            out.append("arr(n=%s){" % rep["count"])
    expand_type(t, seen, out)
    if rep:
        out.append("}")
    if opt:
        out.append("}")


def flatten_packet(name):
    out = []
    for f in PACKETS[name].get("fields", []) or []:
        expand_field(f, frozenset(), out)
    return out


if __name__ == "__main__":
    import sys
    for n in (sys.argv[1:] or sorted(PACKETS)):
        print(n, "=", " ".join(flatten_packet(n)))
