/*
 * Selection fragment: World packets.
 * Owned by verify/src/fillers/Fillers_World.cpp. See PacketSelection.inc for the column meanings.
 *
 * Every line added here needs a matching fill<Type> in that file, or the build fails at link time -
 * which is exactly the enforcement this layout is for.
 */

BP_SELECTED_PACKET(UpdateBlockPacket, Core, "RF", 4, 4, "4 fields; three unsigned varints after a BlockPosition")
BP_SELECTED_PACKET(UpdateBlockSyncedPacket, Rare, "F", 6, 1, "6 fields, 4 inherited; no create() - the only derived packet in the suite")
BP_SELECTED_PACKET(LevelChunkPacket, Core, "RFT", 7, 4, "7 fields; optional sub-chunk limit plus an unconditional varuint32-counted vector<u64> of blob hashes")
BP_SELECTED_PACKET(SubChunkPacket, Common, "RFT", 4, 4, "4 fields; varuint32-counted entry list, 256-entry heightmaps behind presence bytes")
BP_SELECTED_PACKET(SubChunkRequestPacket, Common, "RT", 3, 4, "3 fields; vector<SubChunkPositionOffset> written before basePosition")
BP_SELECTED_PACKET(UpdateSubChunkBlocksPacket, Common, "RT", 3, 1, "3 fields; two independent entry vectors")
BP_SELECTED_PACKET(NetworkChunkPublisherUpdatePacket, Core, "RT", 3, 4, "3 fields; LE32-counted vector<ChunkPosition>")
BP_SELECTED_PACKET(LevelEventPacket, Core, "RFT", 3, 4, "3 fields; optional<Vector3> always written, never absent on decode")
BP_SELECTED_PACKET(LevelEventGenericPacket, Rare, "T", 2, 1, "2 fields; headless compound NBT payload")
BP_SELECTED_PACKET(BlockEventPacket, Common, "RF", 3, 1, "3 fields; BlockPosition plus two signed varints")
BP_SELECTED_PACKET(BlockActorDataPacket, Core, "RT", 2, 1, "2 fields; CacheableNbt compound root")
BP_SELECTED_PACKET(SpawnParticleEffectPacket, Common, "RFT", 5, 1, "5 fields; optional<string> molang variables behind a presence bool")
BP_SELECTED_PACKET(AddVolumeEntityPacket, Rare, "FT", 8, 1, "8 fields; CacheableNbt, two BlockPositions and three strings")
BP_SELECTED_PACKET(RemoveVolumeEntityPacket, Rare, "F", 2, 1, "2 fields; unsigned and signed varint pair")
BP_SELECTED_PACKET(SetTimePacket, Core, "R", 1, 4, "1 field; signed varint, exercises zigzag")
BP_SELECTED_PACKET(SetDifficultyPacket, Common, "R", 1, 1, "1 field; unsigned varint")
BP_SELECTED_PACKET(GameRulesChangedPacket, Common, "RT", 1, 1, "1 field; vector<pair<string, unique_ptr<GameRule>>> across all three rule types")
BP_SELECTED_PACKET(ChunkRadiusUpdatedPacket, Core, "R", 1, 1, "1 field; signed varint radius")
BP_SELECTED_PACKET(RequestChunkRadiusPacket, Core, "R", 2, 1, "2 fields; varint radius plus a raw byte")
BP_SELECTED_PACKET(DimensionDataPacket, Rare, "T", 1, 1, "1 field; keyed definitions validated against DimensionNameIds")
