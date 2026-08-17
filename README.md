# BedrockProtocol-Cpp

A C++20 library that lets server plugins work with Minecraft Bedrock packets as typed objects.

A server hands plugins nothing but raw bytes, so on their own they cannot tell what a packet says.
This library knows the layout of **all 229 packets**, so you can **read, modify, cancel, and inject**
them.

An injected packet reaches only the player you send it to. The server world is untouched, which is
what makes per-player blocks, entities, and UI possible.

The supported runtime is **[Endstone](https://github.com/EndstoneMC/endstone)** (developed against
0.11.8). The core library itself depends on nothing but the standard library — only the optional
bridge target needs Endstone — so it also works for proxies, packet analysers, and custom servers.

## Continuing the original

This began as a 1:1 port of [PocketMine-MP's BedrockProtocol](https://github.com/pmmp/BedrockProtocol),
written in PHP. That repository is archived and stops at Minecraft 1.26.30 (protocol 1001).
**This repository carries it forward** from there, and will keep following new Minecraft versions.

Class, field, and constant names still match the original, so its documentation and examples apply to
everything it covers. Past 1.26.30 there is no original left to match, so the wire formats for newer
versions are taken from [gophertunnel](https://github.com/Sandertv/gophertunnel), pinned to the
version [dragonfly](https://github.com/df-mc/dragonfly) ships against, and the naming conventions of
the PHP original are carried over by hand.

Where a Minecraft update deletes a format outright, the class that represented it goes too — 1.26.40
removed eleven of them, among them `RecipeWithTypeId`, `IntIdMetaItemDescriptor` and the
`SubChunkPacketEntry` cache variants. Formats with no PHP counterpart get a new class named in the
original's style.

- Targets Minecraft **1.26.44** (protocol 2168), and still reads and writes the 1.26.40–1.26.43
  layout of the one packet that changed inside that protocol number — see below
- LGPL-3.0, as a derivative work of the PocketMine Team's original

## Getting started

Add this to your plugin's `CMakeLists.txt`, **after** Endstone is declared:

```cmake
include(FetchContent)
FetchContent_Declare(bedrock_protocol
        GIT_REPOSITORY https://github.com/jv2w/BedrockProtocol-Cpp.git GIT_TAG main)
FetchContent_MakeAvailable(bedrock_protocol)

target_link_libraries(my_plugin PRIVATE bedrock_protocol_bridge)
```

Register the packet types you care about and your callback runs each time one passes. Packets coming
into the server go through `onReceive`, packets the server sends go through `onSend`.

```cpp
auto &interceptor = bedrock_protocol::bridge::PacketInterceptor::get();
interceptor.enable(*this);   // once, from onEnable

interceptor.onSend<bedrock_protocol::TextPacket>([](auto &event) {
    // runs whenever a TextPacket goes out
});
```

Inside the callback you have four options:

| | |
|---|---|
| `event.view()` | Read the packet. Nothing is changed. |
| `event.mutate()` | Read and change it. The packet is re-encoded from what you leave behind. |
| `event.cancel()` | Drop the packet. |
| `interceptor.sendPacket(player, packet)` | Send a packet you built yourself to one player. |

`view()` and `mutate()` are separate so that reading does not pay for re-encoding. Use `view()` when
you are not changing anything.

## Two versions, one protocol number

Protocol 2168 covers Minecraft 1.26.40 through 1.26.44. Normally a protocol number pins one wire
format, but 1.26.44 changed `SetScorePacket` and left the number alone, so a 1.26.43 client and a
1.26.44 client both complete the handshake against the same server and then disagree about the bytes.

The difference is one byte, in the `TYPE_REMOVE` entry only: 1.26.44 wraps the optional objective
name in a second optional, written present. 1.26.40–1.26.43 are identical to each other.

| | 1.26.40 – 1.26.43 | 1.26.44 |
|---|---|---|
| name absent | `00` | `01 00` |
| name present | `01 03 "obj"` | `01 01 03 "obj"` |

Nothing on the wire tells the two apart, so the layout is selected from the version each end reports,
never inferred. `encoding::ProtocolDialect` is that selection, carried by the reader and the writer:

```cpp
namespace bp = bedrock_protocol;

const auto dialect = bp::encoding::dialectFromGameVersion(player.getGameVersion())
                         .value_or(bp::encoding::CURRENT_DIALECT);
bp::encoding::ByteBufferWriter out(payload.size() + 16, dialect);
```

`PacketInterceptor` reads and re-encodes in the **server's** layout, resolved once from
`Server::getMinecraftVersion()` and available as `getServerDialect()`. That is what keeps several
plugins chaining on the same payload correct — a payload does not say which layout it is already in,
so translating it twice corrupts it. `sendPacket()` is the exception: an injected packet has one
recipient and nothing downstream re-reads it, so it goes out in that client's layout.

Rewriting the server's own traffic for a client on the other side of the split is therefore **not**
done here. It has to happen exactly once per server, in one component that owns the decision:

```cpp
bp::SetScorePacket packet;
bp::encoding::ByteBufferReader in(event.getPayload(), 0, serverDialect);
packet.decodeBody(in);
if (in.getUnreadLength() == 0) {
    bp::encoding::ByteBufferWriter out(event.getPayload().size() + 16, clientDialect);
    packet.encodeBody(out);
    event.setPayload(out.getData());
}
```

## deliberate fixes

The port follows the original everywhere except two places where the original cannot read back what
it writes. Both are documented at the point of change, with a reproduction.

- `BitSet::write` sign-extends once bit 63 of a part is set, which corrupts the following part. This
  no longer affects `PlayerAuthInputPacket`, whose input flags stopped being a bitset in 1.26.40, but
  the fix stands for the packets that still use one.
- `EducationSettingsExternalLinkSettings::read` passes its two strings to the constructor in the
  opposite order to the one it read them in, transposing them on every decode.

## Verification

`tools/` holds the gates.

- **Deep round-trip over 812 fields** — all 223 packets in the suite, plus truncation and corruption
  fuzzing (28,673 truncation prefixes, 14,272 corruption passes)
- **Hostile decodes** — all 229 packets against truncated and corrupted input, no crashes
- **Byte-level wire assertions for 1.26.40 and 1.26.44** — expected bytes written out by hand from
  the gophertunnel reference, covering what a round-trip cannot see, including both layouts of the
  packet that differs between them and the translation from each into the other
- **Spec census** — every packet's decode path reduced to its wire primitives and diffed against
  Mojang's published documentation, which is the only gate that can see a field the port has never
  questioned. 189 of 229 packets reduce to an identical sequence; `tools/SPEC_CENSUS_2168.md`
  records the numbers, the six defects the census found, and what it cannot see.

```
powershell -File tools\run_deep_roundtrip.ps1
powershell -File tools\run_malformed_test.ps1
powershell -File tools\run_wire_format_test.ps1
python tools\check_spec_census.py
```

### The PHP wire-parity gate no longer covers this version

The gate that used to matter most fed this port's encoded packets to the PHP original and required
PHP to reproduce them byte for byte. PocketMine's BedrockProtocol is archived and stops at an older
protocol, so for every packet whose format changed in 1.26.40 it can no longer be an authority: a
disagreement there now means "PHP is older", not "this port is wrong".

`tools/check_php_parity.py` is kept for the packets that did not change, and
`tests/WireFormat2168Test.cpp` replaces the coverage that was lost, asserting the exact bytes of the
encodings that did.

```
python tools\check_php_parity.py --php <path-to-bedrock-protocol/src>
```

## Credits

The protocol work is the [PocketMine Team](https://github.com/pmmp/BedrockProtocol)'s. This is a port
of it, distributed under the same LGPL-3.0 licence.
