/*
 * Selection fragment: Ui packets.
 * Owned by verify/src/fillers/Fillers_Ui.cpp. See PacketSelection.inc for the column meanings.
 *
 * Every line added here needs a matching fill<Type> in that file, or the build fails at link time -
 * which is exactly the enforcement this layout is for.
 */

BP_SELECTED_PACKET(CameraInstructionPacket, Rare, "FT", 9, 1, "9 fields, all optional camera instructions; every one engaged")
BP_SELECTED_PACKET(TextPacket, Core, "RF", 8, 4, "8 fields; TYPE_TRANSLATION branch - sourceName not on the wire under it")
BP_SELECTED_PACKET(BossEventPacket, Common, "RF", 8, 1, "8 fields, all written unconditionally in this revision")
BP_SELECTED_PACKET(CameraAimAssistPacket, Rare, "FT", 6, 1, "6 fields; two validated enums plus a Vector2")
BP_SELECTED_PACKET(SetDisplayObjectivePacket, Common, "RF", 5, 1, "5 fields; four strings plus a zigzag sortOrder")
BP_SELECTED_PACKET(NpcRequestPacket, Rare, "F", 5, 1, "5 fields; actorRuntimeId plus two byte discriminators")
BP_SELECTED_PACKET(CameraShakePacket, Common, "RF", 4, 1, "4 fields; two floats and two byte enums")
BP_SELECTED_PACKET(ModalFormResponsePacket, Common, "RF", 3, 1, "3 fields; two optionals, both engaged")
BP_SELECTED_PACKET(SetScorePacket, Common, "RFT", 1, 1, "1 field; one entry per variant, each writing the variant ID, its name and the entry ID, then the members that variant carries")
BP_SELECTED_PACKET(SetScoreboardIdentityPacket, Common, "RT", 2, 1, "2 fields; every entry writes a presence byte before its actorUniqueId, whatever the action type")
BP_SELECTED_PACKET(SetHudPacket, Rare, "T", 2, 1, "2 fields; vector<HudElement> plus a validated visibility enum")
BP_SELECTED_PACKET(ModalFormRequestPacket, Common, "R", 2, 1, "2 fields; u32 formId and a JSON string")
BP_SELECTED_PACKET(ServerSettingsResponsePacket, Rare, "R", 2, 1, "2 fields; u32 formId and a JSON string")
BP_SELECTED_PACKET(ToastRequestPacket, Rare, "R", 2, 1, "2 fields; two strings")
BP_SELECTED_PACKET(ShowCreditsPacket, Rare, "R", 2, 1, "2 fields; actorRuntimeId plus zigzag status")
BP_SELECTED_PACKET(OpenSignPacket, Common, "R", 2, 1, "2 fields; BlockPosition plus a bool")
BP_SELECTED_PACKET(RemoveObjectivePacket, Common, "R", 1, 1, "1 field; a single string")
BP_SELECTED_PACKET(PlayerFogPacket, Rare, "R", 1, 1, "1 field; vector<string> fogLayers")
BP_SELECTED_PACKET(OnScreenTextureAnimationPacket, Rare, "R", 1, 1, "1 field; a single LE u32")
BP_SELECTED_PACKET(CameraPresetsPacket, Rare, "FT", 1, 1, "1 field; vector<CameraPreset>, 22 optionals each, all engaged")
