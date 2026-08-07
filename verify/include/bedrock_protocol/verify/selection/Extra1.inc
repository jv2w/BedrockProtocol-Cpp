/*
 * Selection fragment: scalar and string-only packets that had no deep filler.
 * Owned by verify/src/fillers/Fillers_Extra1.cpp. See PacketSelection.inc for the column meanings.
 */

BP_SELECTED_PACKET(AddBehaviorTreePacket, Rare, "R", 1, 1, "1 field; bare string")
BP_SELECTED_PACKET(AgentActionEventPacket, Rare, "F", 3, 1, "3 fields; LE32 action between two strings")
BP_SELECTED_PACKET(AgentAnimationPacket, Rare, "R", 2, 1, "2 fields; byte plus actor runtime id")
BP_SELECTED_PACKET(AnvilDamagePacket, Rare, "R", 1, 1, "1 field; a bare BlockPosition")
BP_SELECTED_PACKET(AutomationClientConnectPacket, Rare, "R", 1, 1, "1 field; bare string")
BP_SELECTED_PACKET(AwardAchievementPacket, Rare, "R", 1, 1, "1 field; LE32 not varint")
BP_SELECTED_PACKET(BlockPickRequestPacket, Common, "R", 3, 1, "3 fields; BlockPosition plus bool and byte")
BP_SELECTED_PACKET(CameraPacket, Rare, "R", 2, 1, "2 fields; two actor unique ids a swap would hide at zero")
BP_SELECTED_PACKET(ClientCacheBlobStatusPacket, Common, "RT", 2, 1, "2 fields; two u64 lists written in reverse declaration order")
BP_SELECTED_PACKET(ClientCacheStatusPacket, Core, "R", 1, 1, "1 field; single bool")
BP_SELECTED_PACKET(ClientboundUpdateSoundDataPacket, Rare, "RFT", 8, 1, "8 fields; LE64 handle plus seven optional<SoundDataUpdate> union slots")
BP_SELECTED_PACKET(CodeBuilderPacket, Rare, "R", 2, 1, "2 fields; string plus bool")
BP_SELECTED_PACKET(CodeBuilderSourcePacket, Rare, "R", 3, 1, "3 fields; three adjacent bytes a swap would transpose")
BP_SELECTED_PACKET(CreatePhotoPacket, Rare, "R", 3, 1, "3 fields; LE64 unique id, not the varint actor form")
BP_SELECTED_PACKET(CurrentStructureFeaturePacket, Rare, "R", 1, 1, "1 field; bare string")
BP_SELECTED_PACKET(DeathInfoPacket, Common, "RT", 2, 1, "2 fields; vector<string> parameters")
BP_SELECTED_PACKET(DebugInfoPacket, Rare, "R", 2, 1, "2 fields; actor unique id plus string")
BP_SELECTED_PACKET(GameTestRequestPacket, Rare, "RF", 7, 1, "7 fields; mixed varint/byte/bool/BlockPosition")
BP_SELECTED_PACKET(GameTestResultsPacket, Rare, "R", 3, 1, "3 fields; bool then two strings")
BP_SELECTED_PACKET(GuiDataPickItemPacket, Rare, "R", 3, 1, "3 fields; LE32 hotbar slot")
BP_SELECTED_PACKET(HurtArmorPacket, Common, "R", 3, 1, "3 fields; unsigned varint64 armour bitflags")
BP_SELECTED_PACKET(LabTablePacket, Rare, "R", 3, 1, "3 fields; BlockPosition between two bytes")
BP_SELECTED_PACKET(LecternUpdatePacket, Rare, "R", 3, 1, "3 fields; two adjacent bytes then BlockPosition")
BP_SELECTED_PACKET(LessonProgressPacket, Rare, "R", 3, 1, "3 fields; two signed varints then a string")
