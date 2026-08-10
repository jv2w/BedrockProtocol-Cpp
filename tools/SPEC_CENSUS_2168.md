# Spec census, protocol 2168 (Minecraft 1.26.40)

What `tools/check_spec_census.py` measures, what it found, and what it cannot see. Re-run it after
any change to a decode path; the numbers below are the baseline to compare against.

## Sources

| Source | Version | Used for |
|---|---|---|
| [EndstoneMC/protocol-docs](https://github.com/EndstoneMC/protocol-docs) `r26_u4` | Minecraft 1.26.40.31, network 2168 | field order, types, endianness, enum values |
| [Mojang/bedrock-protocol-docs](https://github.com/Mojang/bedrock-protocol-docs) `automated/1.26.40` `json/` only | protocol 2168 | meaning, constraints, cross-check of the above |
| [gophertunnel](https://github.com/Sandertv/gophertunnel) `master` | `CurrentVersion = "1.26.40"` | tie-breaker; procedural, so it reads like the port does |

Both documentation sets list 229 packets and the two name sets match exactly, as does this port's.

## Coverage

| | |
|---|---|
| Packets compared | 229 |
| Reduced token sequence identical to spec | **189 (82.5%)** |
| Differing | 40 |
| Spec wire tokens | 2941 |
| Tokens aligned with the code | **2092 (71.1%)** |
| Packet IDs registered, 1:1, no gaps or duplicates | 229 / 229 |

The 40 differences were read by hand against all three sources. Six were real and are fixed; the
rest are listed below as reduction artefacts or as documentation the sources model differently from
the game. The token figure is the weaker of the two: a packet whose branches the reduction cannot
line up loses every token after the first mismatch even when the fields are right.

## Defects this found

All six were wire-format defects invisible to a round-trip, because the port was consistently wrong
in both directions.

| Packet / type | Field | Spec | Was |
|---|---|---|---|
| `MovementEffectPacket` | Effect ID, Effect Duration | `varint32` | unsigned varint - different bytes for every non-zero value |
| `ArmorSlotAndDamagePair` | Armor Slot | `varint32` | one raw byte |
| `PresenceInfo` | whole structure | one optional string | two extra optional strings, and the last one non-optional |
| `PlayerUpdateEntityOverridesPacket` | after the type | the type's name as a string | one raw byte, leaving the rest of the name on the stream |
| `PlayerLocationPacket` | second varint | the type repeated | a constant 0 |
| `PacketShapeData` | Attached To Entity ID | `ActorUniqueID`, signed | `ActorRuntimeID`, unsigned |

## Known modelling differences - not defects

Confirmed against gophertunnel at 1.26.40 and left alone. These are why the count is 82.5% and not
higher; each one makes several packets differ.

- **`BlockPos`.** Bedrock has two block-position encodings, one with an unsigned Y. Neither
  documentation source distinguishes them; both flatten to three signed varints. The census assumes
  the unsigned-Y form, which is what most packets carry and what this port and gophertunnel use.
- **`optional`.** Endstone marks the presence-byte form and the conditional-on-an-earlier-field form
  with the same flag. The census tries both readings per packet and keeps the closer one, so a
  packet mixing the two still differs.
- **Union expansion.** Where the game has one union-typed field, the doc generator emits one field
  per variant, each carrying the full case list - `ClientboundUpdateSoundDataPacket` and
  `PlayerVideoCapturePacket` are the clearest examples. gophertunnel confirms the port.
- **Repeated discriminators.** Several switches declare the discriminator both on the switch and
  again inside every case; the port reads it once.
- **Discriminator width.** Endstone types several discriminators `uint8` where the game uses a
  varint (`InventorySlotPacket` window id, `ResourcePackClientResponsePacket` response,
  `SetScorePacket` entry type). Identical bytes below 128.
- **Maps.** The census does not model the count prefix in front of a map, so packets carrying one
  show a spare `uvarint32`.

## Left unresolved

- **`ClientMovementPredictionSyncPacket::FLAG_LENGTH`** is 112 where Endstone says `bitset<131>`.
  The divergence is deliberate and documented at the constant, resting on a capture in which a
  1.26.40 client sent 16 bytes rather than the 19 that 131 requires. Captured bytes outrank the
  docs, so it stands.
- **`EditorNetworkPacket`** carries an NBT compound in this port and in gophertunnel; Endstone lists
  two strings. Two of three sources agree with the port, so it stands.
- **`AbilitiesData`** reads its layer count as one byte; Endstone gives `uvarint32`. Identical bytes
  for any real layer count, and the byte matches the PHP original this port tracks. Not changed.
- **Six packets whose decode dispatches through a registry** - `CraftingDataPacket`,
  `InventoryTransactionPacket`, `ItemStackRequestPacket`, `LegacyTelemetryEventPacket`,
  `PlayerAuthInputPacket`, `StartGamePacket` - cannot be followed by the census reader, so their low
  scores measure the tool, not the port. They are covered by `tests/WireFormat2168Test.cpp`.
- **Opaque payloads** - NBT blobs, chunk payloads, skin and resource-pack binaries - are checked for
  length and boundary only. Their contents are the round-trip suite's job.
- **No live client.** Everything above is offline. Nothing here has been confirmed against a real
  1.26.40 session.
