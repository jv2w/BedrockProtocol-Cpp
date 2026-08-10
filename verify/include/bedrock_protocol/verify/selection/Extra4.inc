/*
 * Selection fragment: camera, registry and deeply-nested collection packets that had no deep filler.
 * Owned by verify/src/fillers/Fillers_Extra4.cpp. See PacketSelection.inc for the column meanings.
 *
 * EducationSettingsPacket was excluded here for a while, because the port faithfully reproduced an
 * upstream PocketMine bug: EducationSettingsExternalLinkSettings::read builds
 * `new self($displayName, $url)` from a stream written url-first, transposing the two strings on
 * every decode. That was the correct 1:1 reading, and it also meant the packet could never re-encode
 * to the same bytes.
 *
 * The port now deliberately diverges from upstream on that one function and the packet is covered
 * again. See src/protocol/types/EducationSettingsExternalLinkSettings.cpp for the reasoning.
 */

BP_SELECTED_PACKET(CameraAimAssistActorPriorityPacket, Rare, "T", 1, 1, "1 field; vector<CameraAimAssistActorPriorityData>, four signed varints each")
BP_SELECTED_PACKET(CameraAimAssistPresetsPacket, Rare, "FT", 3, 1, "3 fields; nested category priorities and preset exclusion lists")
BP_SELECTED_PACKET(CameraSplinePacket, Rare, "T", 1, 1, "1 field; spline curve, progress and rotation keyframe lists")
BP_SELECTED_PACKET(ClientCacheMissResponsePacket, Common, "RT", 1, 1, "1 field; vector<ChunkCacheBlob>, LE64 hash plus payload")
BP_SELECTED_PACKET(ClientCameraAimAssistPacket, Rare, "R", 3, 1, "3 fields; validated action-type enum")
BP_SELECTED_PACKET(ClientboundDebugRendererPacket, Rare, "T", 2, 1, "2 fields; optional<DebugMarkerData> with an ARGB colour")
BP_SELECTED_PACKET(CommandBlockUpdatePacket, Common, "RFT", 13, 1, "13 fields; isBlock selects the block branch, which writes four of the six position fields")
BP_SELECTED_PACKET(EducationSettingsPacket, Rare, "FT", 10, 1, "10 fields; three engaged optionals, incl. the link settings whose upstream transposition this port fixes")
BP_SELECTED_PACKET(FeatureRegistryPacket, Rare, "T", 1, 1, "1 field; vector<FeatureRegistryPacketEntry>")
BP_SELECTED_PACKET(GraphicsOverrideParameterPacket, Rare, "FT", 7, 1, "7 fields; three engaged optionals plus a validated enum")
BP_SELECTED_PACKET(LegacyTelemetryEventPacket, Common, "R", 3, 1, "3 fields; runtime id, signed varint and byte")
BP_SELECTED_PACKET(LocatorBarPacket, Common, "RT", 1, 1, "1 field; waypoints with seven engaged optionals each")
BP_SELECTED_PACKET(ServerPresenceInfoPacket, Rare, "T", 1, 1, "1 field; optional<PresenceInfo> holding one more optional")
BP_SELECTED_PACKET(ServerStoreInfoPacket, Rare, "T", 1, 1, "1 field; optional<ClientStoreEntrypointConfig>")
BP_SELECTED_PACKET(ServerboundDataStorePacket, Rare, "T", 1, 1, "1 field; DataStoreUpdate with a polymorphic value")
BP_SELECTED_PACKET(TrimDataPacket, Common, "RT", 2, 1, "2 fields; two vectors of different arity")
BP_SELECTED_PACKET(UpdateClientOptionsPacket, Common, "RT", 2, 1, "2 fields; both optionals engaged, one a validated enum")
BP_SELECTED_PACKET(VoxelShapesPacket, Rare, "FT", 3, 1, "3 fields; voxel cell storage, a string/LE16 name map and a trailing LE16")
