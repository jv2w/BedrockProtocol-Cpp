# BedrockProtocol-Cpp

A C++20 library that lets server plugins work with Minecraft Bedrock packets as typed objects.

A server hands plugins nothing but raw bytes, so on their own they cannot tell what a packet says.
This library knows the layout of **all 229 packets**, so you can **read, modify, cancel, and inject**
them.

An injected packet reaches only the player you send it to. The server world is untouched, which is
what makes per-player blocks, entities, and UI possible.

The supported runtime is **[Endstone](https://github.com/EndstoneMC/endstone)** (developed against
0.11.6). The core library itself depends on nothing but the standard library — only the optional
bridge target needs Endstone — so it also works for proxies, packet analysers, and custom servers.

## Continuing the original

This is a 1:1 port of [PocketMine-MP's BedrockProtocol](https://github.com/pmmp/BedrockProtocol),
which is written in PHP and is now archived and no longer updated. **This repository carries it
forward**, and will keep following new Minecraft versions.

Class, field, and constant names match the original, so its documentation and examples still apply.

- Targets Minecraft **1.26.30** (protocol 1001)
- LGPL-3.0, as a derivative work of the PocketMine Team's original

## Getting started

Add this to your plugin's `CMakeLists.txt`, **after** Endstone is declared:

```cmake
include(FetchContent)
FetchContent_Declare(bedrock_protocol
        GIT_REPOSITORY https://github.com/<owner>/BedrockProtocol-Cpp.git GIT_TAG main)
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

## Two deliberate fixes

The port follows the original everywhere except two places where the original cannot read back what
it writes. Both are documented at the point of change, with a reproduction.

- `BitSet::write` sign-extends once bit 63 of a part is set, which corrupts the following part. Bit
  63 is the sneak flag in `PlayerAuthInputPacket`.
- `EducationSettingsExternalLinkSettings::read` passes its two strings to the constructor in the
  opposite order to the one it read them in, transposing them on every decode.

## Verification

`tools/` holds the gates. The one that matters most feeds this port's own encoded packets to the PHP
original and requires PHP to reproduce them byte for byte:

- **24,976 packet round-trips through the PHP original** — 223 packets × 112 value seeds, byte-identical
- **24,274 hostile decodes** — all 229 packets against truncated and corrupted input, no crashes
- Source-level parity with the original: 229 packets, 198 value types, 95 serialiser methods
- Deep round-trip over 794 fields, plus truncation and corruption fuzzing

```
powershell -File tools\run_deep_roundtrip.ps1
powershell -File tools\run_malformed_test.ps1
python tools\check_php_parity.py --php <path-to-bedrock-protocol/src>
```

## Credits

The protocol work is the [PocketMine Team](https://github.com/pmmp/BedrockProtocol)'s. This is a port
of it, distributed under the same LGPL-3.0 licence.
