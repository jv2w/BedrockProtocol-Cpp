/*
 * Selection fragment: Entity packets.
 * Owned by verify/src/fillers/Fillers_Entity.cpp. See PacketSelection.inc for the column meanings.
 *
 * Every line added here needs a matching fill<Type> in that file, or the build fails at link time -
 * which is exactly the enforcement this layout is for.
 */

BP_SELECTED_PACKET(AddActorPacket, Core, "RFT", 13, 1, "13 fields; EntityMetadata, PropertySyncData, attributes and links in one packet")
BP_SELECTED_PACKET(AddPlayerPacket, Core, "RFT", 17, 1, "17 fields; embeds a whole UpdateAbilitiesPacket body plus EntityMetadata")
BP_SELECTED_PACKET(AddItemActorPacket, Common, "RFT", 7, 1, "7 fields; ItemStackWrapper plus EntityMetadata")
BP_SELECTED_PACKET(AddPaintingPacket, Rare, "F", 5, 1, "5 fields; unique/runtime id pair and a Vector3")
BP_SELECTED_PACKET(MoveActorDeltaPacket, Core, "RF", 8, 4, "8 fields, no create(); every field is written only under its own FLAG_HAS_* bit")
BP_SELECTED_PACKET(MobArmorEquipmentPacket, Common, "RFT", 6, 1, "6 fields; five item descriptors, order-sensitive")
BP_SELECTED_PACKET(MobEquipmentPacket, Core, "RFT", 5, 1, "5 fields; item descriptor plus three byte slots")
BP_SELECTED_PACKET(UpdateAttributesPacket, Core, "RFT", 3, 1, "3 fields; UpdateAttribute carries six floats and nested modifiers")
BP_SELECTED_PACKET(ActorEventPacket, Common, "RF", 4, 1, "4 fields; optional<Vector3> firePosition")
BP_SELECTED_PACKET(AnimatePacket, Core, "RF", 4, 1, "4 fields; optional<string> swingSource")
BP_SELECTED_PACKET(InteractPacket, Common, "RF", 3, 1, "3 fields, no create(); optional<Vector3> position")
BP_SELECTED_PACKET(ActorPickRequestPacket, Rare, "F", 3, 1, "3 fields; actorUniqueId is a LE long here, not a varint")
BP_SELECTED_PACKET(UpdatePlayerGameTypePacket, Common, "RF", 3, 1, "3 fields; mixed varint32/varint64 widths")
BP_SELECTED_PACKET(TakeItemActorPacket, Common, "RF", 2, 1, "2 fields; both u64, written in the opposite order to create()")
BP_SELECTED_PACKET(SpawnExperienceOrbPacket, Rare, "F", 2, 1, "2 fields; Vector3 plus signed varint")
BP_SELECTED_PACKET(EmoteListPacket, Common, "RT", 2, 1, "2 fields; vector<Uuid>")
BP_SELECTED_PACKET(RemoveActorPacket, Core, "R", 1, 4, "1 field; actorUniqueId varint64 zigzag")
BP_SELECTED_PACKET(SetActorLinkPacket, Rare, "T", 1, 1, "1 field; a bare EntityLink")
BP_SELECTED_PACKET(SetHealthPacket, Common, "R", 1, 1, "1 field; signed varint health")
BP_SELECTED_PACKET(SetPlayerGameTypePacket, Common, "R", 1, 1, "1 field; signed varint gamemode")
