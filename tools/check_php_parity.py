"""Compares each packet's wire operations against the PHP original, field by field.

check_symmetry.py compares this port's decode against its own encode, so it cannot see a mistake
made consistently on both sides: swap two fields in decode *and* encode and the packet still
round-trips to identical bytes. The same blindness applies to using the wrong integer width
everywhere for one field. Only a comparison against the reference implementation exposes those.

This tool reduces both the PHP body and the C++ body to a sequence of (operation, field) pairs and
reports the first divergence. It reads the sources; it never runs PHP.

    python tools/check_php_parity.py --php <path-to-bedrock-protocol/src>
    python tools/check_php_parity.py --php <path> TextPacket AddPlayerPacket

A report is a prompt to look, not a proven defect: the two languages spell some constructs
differently and the normalisation below is a heuristic. Divergences that have been examined and
found correct belong in ACCEPTED, with the reason.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

CPP_DIR = Path(__file__).resolve().parent.parent / "src" / "protocol"

# Divergences examined by hand and found correct. Keyed by "<PacketName> <encode|decode>".
#
# Every entry so far is the same shape: PHP picks a branch with a `match` expression, which is an
# expression and so has no C++ equivalent - this port spells it as an if/else chain. The wire
# operations and their order are identical; only the branching keyword differs.
ACCEPTED: dict[str, str] = {
    "RequestAbilityPacket encode": "match(true) on the value's type -> holds_alternative if/else",
    "InventoryTransactionPacket decode": "match on the transaction type id -> if/else over ::ID",
    "PlayerAuthInputPacket decode": "match(true) on the block action type -> if/else",
    "LevelEventGenericPacket decode": "PHP reads the NBT out of the raw buffer and restores the "
                                      "reader's offset by hand; this port reads through the reader",
}

# Bodies whose operations and their order match, but where the field name cannot be recovered from
# one side. The operation sequence is still compared for these; only the name check is skipped.
ACCEPTED_FIELDS: dict[str, str] = {
    "CommandOutputPacket encode": "the per-message helper is a method call, not a field reference",
    "CorrectPlayerMovePredictionPacket decode": "the two floats are read into locals first, because "
                                                "C++ leaves constructor argument order unspecified",
    "DisconnectPacket encode": "PHP assigns $skipMessage inside the call; this port hoists it",
    "FeatureRegistryPacket decode": "PHP clears the vector in the for-init, this port before the loop",
    "SubChunkPacket encode": "PHP's polymorphic entry list is a variant visited with std::visit here",
}

# The same, for value types. Keyed by type name; each is explained in that type's source as well.
ACCEPTED_TYPES: dict[str, str] = {
    "AttributeEnvironment": "read/writeOptional expanded by hand - AttributeValue is polymorphic, so "
                            "it is held by pointer rather than by optional",
    "DataStoreChange": "PHP's ?-> null-safe calls become an explicit null check",
    "DataStoreUpdate": "PHP's match on the value type is a helper function here",
    "ItemStackExtraData": "PHP decodes NBT from a buffer copy and restores the offset in a finally; "
                          "this port reads straight through the reader",
    "PacketShapeData": "PHP's ?-> null-safe calls become an explicit null check",

    # serializer/ - the shared helpers. Every one of these was read side by side against the PHP.
    "CommonTypes::getUUID": "PHP reads the two reversed halves as two statements; this port loops",
    "CommonTypes::putUUID": "PHP writes the two reversed halves as two statements; this port loops",
    "CommonTypes::getNbtRoot": "PHP decodes from a buffer copy and restores the reader's offset",
    "CommonTypes::getNetworkItemStackDescriptor": "PHP's if/else assigns both branches; this port "
                                                  "initialises the locals and fills only the if",
    "CommonTypes::putNetworkItemStackDescriptor": "PHP assigns $hasNetId inside the putBool call",
    "CommonTypes::getRecipeIngredient": "PHP's match over the descriptor type is ItemDescriptor::read "
                                        "here, whose dispatch the types/ pass compares",
    "CommonTypes::putRecipeIngredient": "the descriptor writes itself through the base class",
    "CommonTypes::getEntityMetadata": "PHP's readMetadataProperty is MetadataProperty::read here",
    "NetworkNbtSerializer::readString": "binaryutils' get() is a raw byte-array read",
    "NetworkNbtSerializer::writeString": "binaryutils' put() is a raw byte-array write",
    "PacketBatch::decodePackets": "PHP yields from a generator; this port fills a vector",
}

# Calls that carry no wire operation and would otherwise show up as noise.
IGNORED_CALLS = {
    "count", "size", "empty", "reserve", "push_back", "emplace_back", "resize", "clear",
    "static_cast", "make_unique", "move", "for", "if", "while", "switch", "sizeof",
    "value", "has_value", "at", "begin", "end", "data", "length",
    "getMessage", "what",  # exception text, not a wire operation
}

# The two languages name a handful of equivalent operations differently.
#
# The NBT serialisers are the bulk of this: PHP writes them against pocketmine/binaryutils, whose
# BinaryStream spells the same primitives differently from this port's encoding layer. Each line
# below asserts an equivalence - `getVarInt` is a zigzag signed 32-bit varint, `getLShort` is a
# little-endian unsigned 16-bit, and so on - so it is the one table in this file to re-check by hand
# rather than trust.
ALIASES = {
    "encodeBody": "encodePayload",
    "decodeBody": "decodePayload",
    "getByte": "readUnsigned",
    "putByte": "writeUnsigned",
    "getLShort": "readUnsignedShort",
    "putLShort": "writeUnsignedShort",
    "getSignedLShort": "readSignedShort",
    "getVarInt": "readSignedInt",
    "putVarInt": "writeSignedInt",
    "getUnsignedVarInt": "readUnsignedInt",
    "putUnsignedVarInt": "writeUnsignedInt",
    "getVarLong": "readSignedLong",
    "putVarLong": "writeSignedLong",
    "getLFloat": "readFloat",
    "putLFloat": "writeFloat",
    "getLDouble": "readDouble",
    "putLDouble": "writeDouble",
}

# Identifiers that appear alongside a field name in C++ but are not the field.
CPP_NOISE = IGNORED_CALLS | {
    "out", "in", "const", "auto", "std", "this", "return", "throw", "uint8_t", "uint16_t",
    "uint32_t", "uint64_t", "int8_t", "int16_t", "int32_t", "int64_t", "size_t", "float",
    "double", "bool", "char", "unsigned", "signed", "long", "short", "int", "string",
    "encoding", "serializer", "types", "nbt",
    "true", "false", "nullptr", "nullopt",
    # The reader/writer names this port gives its serialisation lambdas.
    "writer", "reader",
}

CALL_RE = re.compile(r"(?:(\w+)\s*(?:::|->|\.)\s*)?(\w+)\s*\(")
PHP_FIELD_RE = re.compile(r"\$this->(\w+)")
CPP_ARG_FIELD_RE = re.compile(r"\b([a-z_]\w*)\b")
# Matched anywhere on the line rather than at its start: PHP writes `}elseif($x){` on one line where
# this port writes the brace and the `else if` on two, so anchoring on the brace style would report
# every branching packet. Closing braces carry no operation and are ignored on both sides.
#
# `case` and `default` are deliberately absent. PHP selects a branch with a `match` expression whose
# arms have no keyword at all, so including them would report every polymorphic packet. The wire
# operations inside each branch are still compared in order, which is what decides the bytes; what
# this cannot see is two branches whose bodies emit the identical operations being mapped to each
# other's discriminator.
STRUCTURE_RE = re.compile(r"\b(elseif|else\s+if|else|if|for|foreach|while|switch|match)\b")


def extract_body(source: str, *signatures: str) -> str | None:
    """Returns the body of the first signature that appears, or None."""
    for signature in signatures:
        start = source.find(signature)
        if start < 0:
            continue
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


def cpp_method_body(source: str, method: str, owner: str = "") -> str | None:
    """The body of a C++ method definition named `method`.

    Matching on the bare name would find the word in a doc comment or in a call site and return
    whatever block came next, so a candidate only counts when its parameter list is followed by an
    opening brace - that is, when it is a definition rather than a mention or a declaration.
    """
    # Out-of-line definitions are qualified (`CommonTypes::putString(`); inline ones are bare but
    # must not be preceded by a member access, which would make them a call site.
    qualified = rf"{re.escape(owner)}::{re.escape(method)}\s*\(" if owner else ""
    pattern = rf"(?:{qualified}|(?<![\w:.>]){re.escape(method)}\s*\()" if owner else rf"(?<![\w:.>]){re.escape(method)}\s*\("
    for match in re.finditer(pattern, source):
        depth = 0
        for i in range(match.end() - 1, len(source)):
            if source[i] == "(":
                depth += 1
            elif source[i] == ")":
                depth -= 1
                if depth == 0:
                    tail = source[i + 1 : i + 200]
                    # A trailing return type sits between the parameter list and the body.
                    if re.match(r"\s*(const\s+|noexcept\s+|override\s+)*(->[^{;]+)?\{", tail):
                        return extract_body(source[i:], "{")
                    break
    return None


def field_of(line: str, is_php: bool) -> str:
    """Best-effort name of the packet field this line touches.

    Comparing these is what catches a permutation: two adjacent fields of the same type can be
    swapped in both decode and encode without changing a single byte, so the operation sequence
    alone cannot see it.
    """
    if is_php:
        found = PHP_FIELD_RE.findall(line)
        return found[-1].lower() if found else "?"

    # Decode assigns into the member: `field = CommonTypes::getX(in);`
    assign = line.find("=")
    call = line.find("(")
    if 0 < assign < call:
        names = cpp_identifiers(line[:assign])
        if names:
            return names[-1]

    # Encode passes it as the last argument: `CommonTypes::putX(out, field);`
    names = cpp_identifiers(line[call + 1 :] if call >= 0 else line)
    return names[-1] if names else "?"


def cpp_identifiers(text: str) -> list[str]:
    """Identifiers in `text` that could name a member, with call names and qualifiers removed."""
    # Optionals and collections take a lambda whose parameters would otherwise look like the field:
    # `putOptional(out, field, [](ByteBufferWriter &writer, const T &v) { ... })`. The field is
    # always ahead of the capture list, so nothing after it can be the one.
    lambda_at = text.find("[")
    if lambda_at >= 0:
        text = text[:lambda_at]

    text = re.sub(r"\w+\s*\(", "(", text)  # method and function names
    text = re.sub(r"\w+\s*::", "", text)  # namespace and class qualifiers
    text = re.sub(r"<[^<>]*>", "", text)  # template arguments
    return [n.lower() for n in CPP_ARG_FIELD_RE.findall(text) if n not in CPP_NOISE]


def operations(body: str, is_php: bool) -> list[tuple[str, str]]:
    """Reduces a method body to its ordered wire operations, each tagged with a field name."""
    ops: list[tuple[str, str]] = []
    for raw in body.splitlines():
        line = raw.split("//")[0].strip()
        if not line or line.startswith("*") or line.startswith("/*"):
            continue

        # Calls come first: a loop or condition header runs its own calls before the block is
        # entered, and decode reads a collection's element count inside the for-init where encode
        # writes it on the line above.
        for scope, name in CALL_RE.findall(line):
            if name in IGNORED_CALLS or (scope or "") in IGNORED_CALLS:
                continue
            name = ALIASES.get(name, name)
            if not (
                name.startswith(("get", "put", "read", "write"))
                or name in ("encodePayload", "decodePayload")
            ):
                continue
            ops.append((name, field_of(line, is_php)))

        for keyword in STRUCTURE_RE.findall(line):
            marker = keyword.replace(" ", "")
            # PHP iterates collections with foreach where this port uses a range-for, and spells
            # `elseif` as one word. Neither difference touches the wire.
            marker = {"elseif": "else if", "foreach": "for", "match": "switch"}.get(marker, keyword)
            ops.append(("<" + marker + ">", ""))
    return ops


def compare(name: str, php_src: Path, cpp_src: Path, decode: bool) -> list[str]:
    side = "decode" if decode else "encode"
    php_body = extract_body(php_src.read_text(encoding="utf-8"), f"function {side}Payload")
    cpp_body = extract_body(cpp_src.read_text(encoding="utf-8"), f"void {name}::{side}Payload")
    if php_body is None or cpp_body is None:
        return [] if php_body is None and cpp_body is None else [f"{name} {side}: present on only one side"]

    php_ops = operations(php_body, True)
    cpp_ops = operations(cpp_body, False)

    if [o for o, _ in php_ops] != [o for o, _ in cpp_ops]:
        problems = [f"{name} {side}: operation sequence differs from PHP"]
        for i in range(max(len(php_ops), len(cpp_ops))):
            p = php_ops[i] if i < len(php_ops) else ("<missing>", "")
            c = cpp_ops[i] if i < len(cpp_ops) else ("<missing>", "")
            if p[0] != c[0]:
                problems.append(f"    step {i}:  php {p[0]}({p[1]})   cpp {c[0]}({c[1]})")
                break
        return problems

    # Same operations in the same order: check that each one carries the same field.
    if f"{name} {side}" in ACCEPTED_FIELDS:
        return []
    swaps = [
        f"    step {i}:  {php_ops[i][0]}  php field {php_ops[i][1]!r}  cpp field {cpp_ops[i][1]!r}"
        for i in range(len(php_ops))
        if php_ops[i][1] != cpp_ops[i][1] and "?" not in (php_ops[i][1], cpp_ops[i][1])
    ]
    if swaps:
        return [f"{name} {side}: same operations, different field at that step"] + swaps
    return []


TRAIT_USE_RE = re.compile(r"^\s*use\s+(\w*Trait)\s*;", re.MULTILINE)


def with_traits(php_src: Path) -> str:
    """The file's text plus the bodies of any traits it mixes in.

    PHP hoists read/write into a trait for the stack-request actions that share them. Without this
    the class looks like it serialises nothing and would be skipped without a word.
    """
    text = php_src.read_text(encoding="utf-8")
    for trait in TRAIT_USE_RE.findall(text):
        trait_file = php_src.parent / f"{trait}.php"
        if trait_file.exists():
            text += "\n" + trait_file.read_text(encoding="utf-8")
    return text


def compare_type(php_src: Path, cpp_src: Path) -> list[str]:
    """Same comparison for a value type, whose methods are named read/write rather than *Payload."""
    name = php_src.stem
    php_text = with_traits(php_src)
    cpp_text = cpp_src.read_text(encoding="utf-8")

    problems: list[str] = []
    for side, php_sig, cpp_sigs in (
        # Small types define read/write inline in the header, where the name is unqualified.
        ("write", "function write(", (f"{name}::write(", " write(")),
        ("read", "function read(", (f"{name}::read(", " read(")),
    ):
        php_body = extract_body(php_text, php_sig)
        cpp_body = extract_body(cpp_text, *cpp_sigs)
        if php_body is None or cpp_body is None:
            continue

        php_ops = operations(php_body, True)
        cpp_ops = operations(cpp_body, False)
        if [o for o, _ in php_ops] == [o for o, _ in cpp_ops]:
            continue

        problems.append(f"{name}::{side}: operation sequence differs from PHP")
        for i in range(max(len(php_ops), len(cpp_ops))):
            p = php_ops[i] if i < len(php_ops) else ("<missing>", "")
            c = cpp_ops[i] if i < len(cpp_ops) else ("<missing>", "")
            if p[0] != c[0]:
                problems.append(f"    step {i}:  php {p[0]}   cpp {c[0]}")
                break
    return problems


def run_types(php_root: Path) -> int:
    """Compares every PHP value type that serialises itself against its C++ counterpart."""
    repo = Path(__file__).resolve().parent.parent
    cpp_root = repo / "src" / "protocol" / "types"
    header_root = repo / "include" / "bedrock_protocol" / "protocol" / "types"

    compared = missing = reported = 0
    for php_src in sorted((php_root / "types").rglob("*.php")):
        text = with_traits(php_src)
        if "function read(" not in text and "function write(" not in text:
            continue
        if php_src.stem.endswith("Trait"):
            continue  # compared through each class that mixes it in

        # The port mirrors the PHP directory layout exactly, so the relative path is the mapping.
        relative = php_src.relative_to(php_root / "types")
        candidates = [cpp_root / relative.with_suffix(".cpp")]
        candidates += list(cpp_root.rglob(php_src.stem + ".cpp"))
        # Types small enough to be defined inline live only in the header.
        candidates += list(header_root.rglob(php_src.stem + ".h"))

        cpp_src = next((c for c in candidates if c.exists()), None)
        if cpp_src is None:
            missing += 1
            continue

        compared += 1
        if php_src.stem in ACCEPTED_TYPES:
            continue
        for line in compare_type(php_src, cpp_src):
            print(line)
            reported += 1

    print(f"\n{compared} value types compared against PHP")
    if missing:
        print(f"  {missing} without a C++ .cpp counterpart (header-only or inlined)")
    if reported == 0:
        print("[OK] every value type's wire operations match the PHP original")
        return 0
    print(f"[REVIEW] {reported} line(s) reported above")
    return 1


PHP_METHOD_RE = re.compile(r"function\s+(\w+)\s*\(")

# Files under serializer/ that never touch a byte buffer - they flatten and index the command tree
# in memory, and the packet that calls them is compared by the packet pass. Comparing them with a
# wire-operation tool reports ordinary getters and proves nothing.
NOT_SERIALISERS = {
    "AvailableCommandsPacketAssembler",
    "AvailableCommandsPacketDisassembler",
    "DisassembledAvailableCommandsData",
}


def run_serializer(php_root: Path) -> int:
    """Compares the shared serialisation helpers method by method.

    Every packet is built out of these, so a single wrong integer width here is wrong everywhere,
    and the deep round-trip cannot see it because both sides would use the same wrong width.
    """
    repo = Path(__file__).resolve().parent.parent
    cpp_root = repo / "src" / "protocol" / "serializer"
    header_root = repo / "include" / "bedrock_protocol" / "protocol" / "serializer"

    compared = reported = 0
    skipped: list[str] = []
    for php_src in sorted((php_root / "serializer").glob("*.php")):
        name = php_src.stem
        if name in NOT_SERIALISERS:
            continue
        # CommonTypes splits its methods between the header and the .cpp, so both have to be read.
        sources = [c for c in (cpp_root / f"{name}.cpp", header_root / f"{name}.h") if c.exists()]
        if not sources:
            print(f"{name}: no C++ counterpart")
            reported += 1
            continue

        php_text = php_src.read_text(encoding="utf-8")
        cpp_text = "\n".join(c.read_text(encoding="utf-8") for c in sources)
        for method in dict.fromkeys(PHP_METHOD_RE.findall(php_text)):
            php_body = extract_body(php_text, f"function {method}(")
            cpp_body = cpp_method_body(cpp_text, method, name)
            if php_body is None or cpp_body is None:
                skipped.append(f"{name}::{method}")
                continue

            compared += 1
            php_ops = [o for o, _ in operations(php_body, True)]
            cpp_ops = [o for o, _ in operations(cpp_body, False)]
            if php_ops == cpp_ops or f"{name}::{method}" in ACCEPTED_TYPES:
                continue

            print(f"{name}::{method}: operation sequence differs from PHP")
            for i in range(max(len(php_ops), len(cpp_ops))):
                p = php_ops[i] if i < len(php_ops) else "<missing>"
                c = cpp_ops[i] if i < len(cpp_ops) else "<missing>"
                if p != c:
                    print(f"    step {i}:  php {p}   cpp {c}")
                    break
            reported += 1

    print(f"\n{compared} serialiser methods compared against PHP")
    if skipped:
        # Named rather than counted: a silent skip is indistinguishable from a pass.
        print(f"  {len(skipped)} without a matching C++ method: {', '.join(skipped)}")
    if reported == 0:
        print("[OK] every serialiser method's wire operations match the PHP original")
        return 0
    print(f"[REVIEW] {reported} method(s) reported above")
    return 1


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--php", required=True, help="path to the PHP BedrockProtocol src/ directory")
    parser.add_argument("--types", action="store_true", help="compare types/ instead of packets")
    parser.add_argument("--serializer", action="store_true", help="compare serializer/ instead of packets")
    parser.add_argument("packets", nargs="*")
    args = parser.parse_args()

    php_dir = Path(args.php)
    if not (php_dir / "TextPacket.php").exists():
        print(f"not a BedrockProtocol src directory: {php_dir}")
        return 2

    if args.types:
        return run_types(php_dir)
    if args.serializer:
        return run_serializer(php_dir)

    names = args.packets or sorted(
        p.stem for p in CPP_DIR.glob("*Packet.cpp") if p.stem != "DataPacket"
    )

    reported = accepted = missing = 0
    for name in names:
        cpp_src = CPP_DIR / f"{name}.cpp"
        php_src = php_dir / f"{name}.php"
        if not php_src.exists():
            print(f"{name}: no PHP counterpart at {php_src.name}")
            missing += 1
            continue
        for decode in (False, True):
            if f"{name} {'decode' if decode else 'encode'}" in ACCEPTED:
                accepted += 1
                continue
            for line in compare(name, php_src, cpp_src, decode):
                print(line)
                reported += 1

    print(f"\n{len(names)} packets compared against PHP")
    if accepted:
        print(f"  {accepted} accepted divergence(s) skipped")
    if missing:
        print(f"  {missing} without a PHP counterpart")
    if reported == 0:
        print("[OK] every packet's wire operations match the PHP original")
        return 0
    print(f"[REVIEW] {reported} line(s) reported above")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
