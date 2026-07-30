/*
 * Selection fragment: small transport, telemetry and education packets that had no deep filler.
 * Owned by verify/src/fillers/Fillers_Extra2.cpp. See PacketSelection.inc for the column meanings.
 */

BP_SELECTED_PACKET(ClientboundControlSchemeSetPacket, Rare, "R", 1, 1, "1 field; enum validated on decode")
BP_SELECTED_PACKET(ClientboundDataDrivenUICloseScreenPacket, Rare, "T", 1, 1, "1 field; optional<u32> engaged")
BP_SELECTED_PACKET(ClientboundDataDrivenUIShowScreenPacket, Rare, "T", 3, 1, "3 fields; LE32 formId plus optional<u32>")
BP_SELECTED_PACKET(ClientboundTextureShiftPacket, Rare, "FT", 8, 1, "8 fields; vector<string> plus two unsigned varint64 lengths")
BP_SELECTED_PACKET(ContainerRegistryCleanupPacket, Common, "RT", 1, 1, "1 field; vector<FullContainerName>")
BP_SELECTED_PACKET(EditorNetworkPacket, Rare, "RT", 2, 1, "2 fields; CacheableNbt payload after a bool")
BP_SELECTED_PACKET(EduUriResourcePacket, Rare, "RT", 1, 1, "1 field; nested EducationUriResource")
BP_SELECTED_PACKET(JigsawStructureDataPacket, Rare, "RT", 1, 1, "1 field; raw CacheableNbt")
BP_SELECTED_PACKET(MapCreateLockedCopyPacket, Rare, "R", 2, 1, "2 fields; two actor unique ids a swap would hide at zero")
BP_SELECTED_PACKET(MultiplayerSettingsPacket, Rare, "R", 1, 1, "1 field; signed varint")
BP_SELECTED_PACKET(NetworkStackLatencyPacket, Common, "R", 2, 1, "2 fields; LE64 timestamp")
BP_SELECTED_PACKET(PacketViolationWarningPacket, Common, "RF", 4, 1, "4 fields; three signed varints then a string")
BP_SELECTED_PACKET(PartyChangedPacket, Rare, "R", 2, 1, "2 fields; string plus bool")
BP_SELECTED_PACKET(PartyDestinationCookieResponsePacket, Rare, "R", 2, 1, "2 fields; string plus bool")
BP_SELECTED_PACKET(PhotoTransferPacket, Rare, "RF", 7, 1, "7 fields; four strings, two bytes and an LE64 id")
BP_SELECTED_PACKET(PlayerActionPacket, Core, "RF", 5, 1, "5 fields; two BlockPositions a swap would transpose")
BP_SELECTED_PACKET(PlayerStartItemCooldownPacket, Common, "R", 2, 1, "2 fields; string plus signed varint")
BP_SELECTED_PACKET(PositionTrackingDBClientRequestPacket, Rare, "R", 2, 1, "2 fields; byte action plus signed varint")
BP_SELECTED_PACKET(PositionTrackingDBServerBroadcastPacket, Rare, "RT", 3, 1, "3 fields; CacheableNbt tail")
BP_SELECTED_PACKET(PurchaseReceiptPacket, Rare, "RT", 1, 1, "1 field; vector<string>")
BP_SELECTED_PACKET(RequestNetworkSettingsPacket, Core, "R", 1, 1, "1 field; BIG-endian u32, unique in the protocol")
BP_SELECTED_PACKET(RequestPermissionsPacket, Rare, "R", 3, 1, "3 fields; LE64, varint and LE16 in one packet")
BP_SELECTED_PACKET(ScriptMessagePacket, Common, "R", 2, 1, "2 fields; two strings a swap would exchange")
BP_SELECTED_PACKET(SendPartyDestinationCookiePacket, Rare, "R", 3, 1, "3 fields; three strings a swap would exchange")
