/*
 * Selection fragment: Misc packets.
 * Owned by verify/src/fillers/Fillers_Misc.cpp. See PacketSelection.inc for the column meanings.
 *
 * Every line here needs a matching fill<Type> in that file, or the build fails at link time - which
 * is exactly the enforcement this layout is for.
 */

BP_SELECTED_PACKET(ServerboundDiagnosticsPacket, Rare, "FT", 13, 1, "13 fields; nine floats plus four vectors of timing structs")
BP_SELECTED_PACKET(ClientboundMapItemDataPacket, Rare, "FT", 12, 1, "12 fields; no create() - `type` is a bitmask derived from three populated collections")
BP_SELECTED_PACKET(BookEditPacket, Rare, "F", 9, 1, "9 fields; no create() - type selects the branch, pinned to TYPE_REPLACE_PAGE")
BP_SELECTED_PACKET(AvailableCommandsPacket, Core, "RFT", 8, 4, "8 parallel vectors of raw command structs, four levels deep")
BP_SELECTED_PACKET(ResourcePacksInfoPacket, Core, "RF", 7, 1, "7 fields; vector<ResourcePackInfoEntry> with a LE16 count")
BP_SELECTED_PACKET(ResourcePackDataInfoPacket, Common, "RF", 7, 1, "7 fields; LE32/LE64 widths a varint port would get wrong")
BP_SELECTED_PACKET(CommandOutputPacket, Common, "RFT", 5, 1, "5 fields; no create() - CommandOriginData plus optional<string> data")
BP_SELECTED_PACKET(ResourcePackStackPacket, Core, "RF", 5, 1, "5 fields; stack entries plus Experiments")
BP_SELECTED_PACKET(UpdateAdventureSettingsPacket, Common, "R", 5, 1, "5 fields; five consecutive bools - only ordering can be tested")
BP_SELECTED_PACKET(CommandRequestPacket, Core, "RT", 4, 1, "4 fields; CommandOriginData")
BP_SELECTED_PACKET(ResourcePackChunkDataPacket, Common, "R", 4, 1, "4 fields; LE32 index and LE64 offset")
BP_SELECTED_PACKET(StructureBlockUpdatePacket, Rare, "T", 4, 1, "4 fields; StructureEditorData nests StructureSettings")
BP_SELECTED_PACKET(StructureTemplateDataRequestPacket, Rare, "T", 4, 1, "4 fields; StructureSettings")
BP_SELECTED_PACKET(PlayerSkinPacket, Common, "RFT", 4, 1, "4 fields; full SkinData - images, animations, persona pieces")
BP_SELECTED_PACKET(UpdateSoftEnumPacket, Rare, "F", 3, 1, "3 fields; vector<string> values")
BP_SELECTED_PACKET(StructureTemplateDataResponsePacket, Rare, "T", 3, 1, "3 fields; optional CacheableNbt written under a bool")
BP_SELECTED_PACKET(PlayerListPacket, Core, "RFT", 2, 1, "2 fields; pinned to TYPE_ADD - TYPE_REMOVE writes only UUIDs")
BP_SELECTED_PACKET(MapInfoRequestPacket, Rare, "T", 2, 1, "2 fields; client pixels pack x/y into one LE16 index")
BP_SELECTED_PACKET(SettingsCommandPacket, Common, "R", 2, 1, "2 fields; string plus bool")
BP_SELECTED_PACKET(ResourcePackChunkRequestPacket, Common, "R", 2, 1, "2 fields; string plus LE32")
BP_SELECTED_PACKET(ResourcePackClientResponsePacket, Core, "R", 2, 1, "2 fields; LE16-counted vector<string>")
BP_SELECTED_PACKET(RequestAbilityPacket, Common, "RT", 2, 1, "2 fields; variant<bool,float> - both are always written, the type tag picks one")
BP_SELECTED_PACKET(BiomeDefinitionListPacket, Common, "FT", 2, 4, "2 fields; BiomeDefinitionData with an optional tag-index list")
BP_SELECTED_PACKET(UpdateAbilitiesPacket, Core, "RT", 1, 1, "1 field; AbilitiesData - ability bits and three optional speeds share one flag word")
BP_SELECTED_PACKET(AvailableActorIdentifiersPacket, Core, "RT", 1, 1, "1 field; CacheableNbt compound root")
