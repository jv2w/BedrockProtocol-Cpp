"""Extract canonical wire-op token sequences from BedrockProtocol-Cpp decode paths.

Part of the spec census gate. Run tools/check_spec_census.py; see its docstring for the
Endstone protocol-docs checkout it needs.
"""

import os, re, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(REPO, "src")

PRIM = {
    ("Byte", "readUnsigned"): "u8", ("Byte", "readSigned"): "i8",
    ("LE", "readUnsignedShort"): "u16le", ("LE", "readSignedShort"): "i16le",
    ("LE", "readUnsignedInt"): "u32le", ("LE", "readSignedInt"): "i32le",
    ("LE", "readUnsignedLong"): "u64le", ("LE", "readSignedLong"): "i64le",
    ("LE", "readFloat"): "f32le", ("LE", "readDouble"): "f64le",
    ("BE", "readUnsignedShort"): "u16be", ("BE", "readSignedShort"): "i16be",
    ("BE", "readUnsignedInt"): "u32be", ("BE", "readSignedInt"): "i32be",
    ("BE", "readUnsignedLong"): "u64be", ("BE", "readSignedLong"): "i64be",
    ("BE", "readFloat"): "f32be", ("BE", "readDouble"): "f64be",
    ("VarInt", "readUnsignedInt"): "uvar32", ("VarInt", "readSignedInt"): "var32",
    ("VarInt", "readUnsignedLong"): "uvar64", ("VarInt", "readSignedLong"): "var64",
}
COMMON = {
    "getString": ["str"], "getBool": ["u8"], "getUUID": ["u64le", "u64le"],
    "getVector3": ["f32le", "f32le", "f32le"], "getVector2": ["f32le", "f32le"],
    "getBlockPosition": ["var32", "uvar32", "var32"],
    "getActorUniqueId": ["var64"], "getActorRuntimeId": ["uvar64"],
    "getRotationByte": ["i8"],
    "getNbtCompoundRoot": ["nbt"], "getNbtRoot": ["nbt"],
    "readRecipeNetId": ["uvar32"], "readCreativeItemNetId": ["uvar32"],
    "readItemStackRequestId": ["var32"], "readLegacyItemStackRequestId": ["var32"],
    "readServerItemStackId": ["var32"],
}
# CommonTypes composites: expanded from their own bodies in CommonTypes.cpp
COMMON_INLINE = {
    "getSkin", "getItemStackWithoutStackId", "getNetworkItemStackDescriptor",
    "getStackRequestItem", "getRecipeIngredient", "getStackRequestRecipeIngredient",
    "getEntityMetadata", "getGameRules", "getEntityLink", "getCommandOriginData",
    "getStructureSettings", "getStructureEditorData",
}


def read(p):
    return open(p, encoding="utf-8", errors="replace").read()


def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    s = re.sub(r"//[^\n]*", " ", s)
    return s


def body_after(text, idx):
    """Return the {...} body starting at/after idx."""
    i = text.find("{", idx)
    if i < 0:
        return ""
    depth = 0
    for j in range(i, len(text)):
        c = text[j]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return text[i + 1:j]
    return text[i + 1:]


# --- index every decode-capable function body -------------------------------
ALL = {}   # "Class::fn" -> body
FILES = []
for root, _, files in os.walk(SRC):
    for fn in files:
        if fn.endswith(".cpp"):
            FILES.append(os.path.join(root, fn))
FREE = {}   # file-local helper "getFoo" -> body
for p in FILES:
    t = strip_comments(read(p))
    for m in re.finditer(r"\b([A-Za-z_]\w*)::(\w+)\s*\(([^;{)]*)\)\s*(?:const\s*)?\{", t):
        cls, fn, args = m.group(1), m.group(2), m.group(3)
        if "ByteBufferReader" not in args:
            continue
        ALL.setdefault(cls + "::" + fn, body_after(t, m.end() - 1))
    # anonymous-namespace helpers: "Type name(encoding::ByteBufferReader &in) {"
    for m in re.finditer(r"(?<![:\w])([a-z]\w*)\s*\(([^;{)]*ByteBufferReader[^;{)]*)\)\s*\{", t):
        FREE.setdefault(m.group(1), body_after(t, m.end() - 1))

# header-defined inline readers too
for root, _, files in os.walk(os.path.join(REPO, "include")):
    for fn in files:
        if not fn.endswith(".h"):
            continue
        p = os.path.join(root, fn)
        t = strip_comments(read(p))
        cls = None
        for m in re.finditer(r"\b(?:class|struct)\s+(\w+)|static\s+[\w:<>,\s*&]+?\s(\w+)\(([^;{)]*)\)\s*\{", t):
            if m.group(1):
                cls = m.group(1)
            elif cls and "ByteBufferReader" in (m.group(3) or ""):
                key = cls + "::" + m.group(2)
                ALL.setdefault(key, body_after(t, m.end() - 1))

TOKRE = re.compile(r"""
    (?P<prim>(?:encoding::)?(Byte|LE|BE|VarInt)::(\w+))\s*\(
  | (?P<common>(?:serializer::)?CommonTypes::(?P<commonfn>\w+))\s*\(
  | (?P<opt>readOptional)\s*\(
  | (?P<call>(?:[A-Za-z_]\w*::)*(?P<callcls>[A-Z]\w*)::(?P<callfn>read\w*|decode\w*|fromBytes))\s*\(
  | (?P<free>(?<![:.\w])(?P<freefn>[a-z]\w*)\s*\(\s*(?:in|reader)\s*[,)])
  | (?P<for>\bfor\s*\()
  | (?P<while>\bwhile\s*\()
  | (?P<if>\bif\s*\()
  | (?P<else>\belse\b)
  | (?P<switch>\bswitch\s*\()
  | (?P<case>\bcase\s+[^:]+:)
  | (?P<lb>\{)
  | (?P<rb>\})
""", re.X)


def walk_body(body, depth, out, stack):
    if depth > 12:
        out.append("DEPTHCUT")
        return
    i = 0
    while True:
        m = TOKRE.search(body, i)
        if not m:
            break
        i = m.end()
        k = m.lastgroup
        if m.group("prim"):
            key = (m.group(2), m.group(3))
            out.append(PRIM.get(key, "?" + m.group("prim")))
        elif m.group("common"):
            fn = m.group("commonfn")
            if fn in COMMON:
                out.extend(COMMON[fn])
            elif fn in COMMON_INLINE:
                emit_body("CommonTypes::" + fn, fn, depth, out, stack)
            elif fn == "readOptional":
                out.append("opt{")
        elif m.group("opt"):
            out.append("opt{")
        elif m.group("call"):
            emit_call(m.group("callcls"), depth, out, stack, m.group("callfn"))
        elif m.group("free"):
            fn = m.group("freefn")
            if fn in COMMON:               # unqualified call from inside CommonTypes
                out.extend(COMMON[fn])
            elif fn in COMMON_INLINE:
                emit_body("CommonTypes::" + fn, fn, depth, out, stack)
            elif fn in FREE:
                if fn in stack:
                    out.append("recurse:" + fn)
                else:
                    walk_body(FREE[fn], depth + 1, out, stack | {fn})
        elif k in ("for", "while"):
            out.append("arr{")
        elif k == "if":
            out.append("if{")
        elif k == "switch":
            out.append("sw{")
        elif k == "case":
            out.append("|")
        elif k == "lb":
            out.append("(")
        elif k == "rb":
            out.append(")")


def emit_body(key, label, depth, out, stack):
    if key not in ALL:
        out.append("opaque:" + label)
        return
    if key in stack:
        out.append("recurse:" + label)
        return
    out.append("<" + label)
    walk_body(ALL[key], depth + 1, out, stack | {key})
    out.append(">")


def emit_call(cls, depth, out, stack, fn=None):
    names = ([cls + "::" + fn] if fn else []) + [cls + "::read", cls + "::readFrom",
                                                 cls + "::decode", cls + "::fromBytes"]
    for key in names:
        if key in ALL:
            emit_body(key, cls, depth, out, stack)
            return
    out.append("opaque:" + cls)


def packet_ops(name):
    key = name + "::decodePayload"
    if key not in ALL:
        return None
    out = []
    walk_body(ALL[key], 0, out, frozenset({key}))
    return out


if __name__ == "__main__":
    for n in sys.argv[1:]:
        print(n, "=", " ".join(packet_ops(n) or ["<NOT FOUND>"]))
