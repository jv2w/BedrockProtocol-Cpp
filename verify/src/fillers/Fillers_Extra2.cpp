/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * Fillers: small transport, telemetry and education packets. See Fillers_Core.cpp for the two
 * conventions that hold across every filler file (one create() call with every argument spelled out;
 * values drawn into named locals first, never inline).
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/ClientboundControlSchemeSetPacket.h"
#include "bedrock_protocol/protocol/ClientboundDataDrivenUICloseScreenPacket.h"
#include "bedrock_protocol/protocol/ClientboundDataDrivenUIShowScreenPacket.h"
#include "bedrock_protocol/protocol/ClientboundTextureShiftPacket.h"
#include "bedrock_protocol/protocol/ContainerRegistryCleanupPacket.h"
#include "bedrock_protocol/protocol/EditorNetworkPacket.h"
#include "bedrock_protocol/protocol/EduUriResourcePacket.h"
#include "bedrock_protocol/protocol/JigsawStructureDataPacket.h"
#include "bedrock_protocol/protocol/MapCreateLockedCopyPacket.h"
#include "bedrock_protocol/protocol/MultiplayerSettingsPacket.h"
#include "bedrock_protocol/protocol/NetworkStackLatencyPacket.h"
#include "bedrock_protocol/protocol/PacketViolationWarningPacket.h"
#include "bedrock_protocol/protocol/PartyChangedPacket.h"
#include "bedrock_protocol/protocol/PartyDestinationCookieResponsePacket.h"
#include "bedrock_protocol/protocol/PhotoTransferPacket.h"
#include "bedrock_protocol/protocol/PlayerActionPacket.h"
#include "bedrock_protocol/protocol/PlayerStartItemCooldownPacket.h"
#include "bedrock_protocol/protocol/PositionTrackingDBClientRequestPacket.h"
#include "bedrock_protocol/protocol/PositionTrackingDBServerBroadcastPacket.h"
#include "bedrock_protocol/protocol/PurchaseReceiptPacket.h"
#include "bedrock_protocol/protocol/RequestNetworkSettingsPacket.h"
#include "bedrock_protocol/protocol/RequestPermissionsPacket.h"
#include "bedrock_protocol/protocol/ScriptMessagePacket.h"
#include "bedrock_protocol/protocol/SendPartyDestinationCookiePacket.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

BP_FILLER(ClientboundControlSchemeSetPacket, 1)
{
    auto &w = ctx.well;
    // Pinned: decodePayload runs the byte through ControlSchemeFromPacket, which throws on anything
    // outside 0..4. PLAYER_RELATIVE_STRAFE is the highest legal value, so it is also non-default.
    const auto scheme = ValueWell::pin(types::ControlScheme::PLAYER_RELATIVE_STRAFE);
    (void)w;

    return std::make_unique<ClientboundControlSchemeSetPacket>(
        ClientboundControlSchemeSetPacket::create(scheme));
}

BP_FILLER(ClientboundDataDrivenUICloseScreenPacket, 1)
{
    auto &w = ctx.well;
    const auto formId = w.some(w.u32());

    return std::make_unique<ClientboundDataDrivenUICloseScreenPacket>(
        ClientboundDataDrivenUICloseScreenPacket::create(formId));
}

BP_FILLER(ClientboundDataDrivenUIShowScreenPacket, 3)
{
    auto &w = ctx.well;
    const auto screenId = w.str("screenId");
    const auto formId = w.u32();
    const auto dataInstanceId = w.some(w.u32());

    return std::make_unique<ClientboundDataDrivenUIShowScreenPacket>(
        ClientboundDataDrivenUIShowScreenPacket::create(screenId, formId, dataInstanceId));
}

BP_FILLER(ClientboundTextureShiftPacket, 8)
{
    auto &w = ctx.well;
    const auto actionId = w.u8();
    const auto collectionName = w.str("collectionName");
    const auto fromStep = w.str("fromStep");
    const auto toStep = w.str("toStep");
    const std::vector<std::string> allSteps{w.str("step0"), w.str("step1"), w.str("step2")};
    const auto currentLengthTicks = w.u64();
    const auto totalLengthTicks = w.u64();
    const auto enabled = w.flag();

    return std::make_unique<ClientboundTextureShiftPacket>(
        ClientboundTextureShiftPacket::create(actionId, collectionName, fromStep, toStep, allSteps,
                                              currentLengthTicks, totalLengthTicks, enabled));
}

BP_FILLER(ContainerRegistryCleanupPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::inventory::FullContainerName> removedContainers;
    removedContainers.push_back(makeFullContainerName(w));
    removedContainers.push_back(makeFullContainerName(w));

    return std::make_unique<ContainerRegistryCleanupPacket>(
        ContainerRegistryCleanupPacket::create(std::move(removedContainers)));
}

BP_FILLER(EditorNetworkPacket, 2)
{
    auto &w = ctx.well;
    const auto isRouteToManager = w.flag();
    auto payload = makeCacheableNbt(w);

    return std::make_unique<EditorNetworkPacket>(
        EditorNetworkPacket::create(isRouteToManager, std::move(payload)));
}

BP_FILLER(EduUriResourcePacket, 1)
{
    auto &w = ctx.well;
    auto resource = makeEducationUriResource(w);

    return std::make_unique<EduUriResourcePacket>(EduUriResourcePacket::create(std::move(resource)));
}

BP_FILLER(JigsawStructureDataPacket, 1)
{
    auto &w = ctx.well;
    auto nbt = makeCacheableNbt(w);

    return std::make_unique<JigsawStructureDataPacket>(
        JigsawStructureDataPacket::create(std::move(nbt)));
}

BP_FILLER(MapCreateLockedCopyPacket, 2)
{
    auto &w = ctx.well;
    const auto originalMapId = w.i64();
    const auto newMapId = w.i64();

    return std::make_unique<MapCreateLockedCopyPacket>(
        MapCreateLockedCopyPacket::create(originalMapId, newMapId));
}

BP_FILLER(MultiplayerSettingsPacket, 1)
{
    auto &w = ctx.well;
    const auto action = w.i32();

    return std::make_unique<MultiplayerSettingsPacket>(MultiplayerSettingsPacket::create(action));
}

BP_FILLER(NetworkStackLatencyPacket, 2)
{
    auto &w = ctx.well;
    const auto timestamp = w.u64();
    const auto needResponse = w.flag();

    return std::make_unique<NetworkStackLatencyPacket>(
        NetworkStackLatencyPacket::create(timestamp, needResponse));
}

BP_FILLER(PacketViolationWarningPacket, 4)
{
    auto &w = ctx.well;
    const auto type = w.i32();
    const auto severity = w.i32();
    const auto packetId = w.i32();
    const auto message = w.str("message");

    return std::make_unique<PacketViolationWarningPacket>(
        PacketViolationWarningPacket::create(type, severity, packetId, message));
}

BP_FILLER(PartyChangedPacket, 2)
{
    auto &w = ctx.well;
    const auto partyId = w.str("partyId");
    const auto partyLeader = w.flag();

    return std::make_unique<PartyChangedPacket>(PartyChangedPacket::create(partyId, partyLeader));
}

BP_FILLER(PartyDestinationCookieResponsePacket, 2)
{
    auto &w = ctx.well;
    const auto cookie = w.str("cookie");
    const auto accepted = w.flag();

    return std::make_unique<PartyDestinationCookieResponsePacket>(
        PartyDestinationCookieResponsePacket::create(cookie, accepted));
}

BP_FILLER(PhotoTransferPacket, 7)
{
    auto &w = ctx.well;
    const auto photoName = w.str("photoName");
    const auto photoData = w.str("photoData");
    const auto bookId = w.str("bookId");
    const auto type = w.u8();
    const auto sourceType = w.u8();
    const auto ownerActorUniqueId = w.i64();
    const auto newPhotoName = w.str("newPhotoName");

    return std::make_unique<PhotoTransferPacket>(PhotoTransferPacket::create(
        photoName, photoData, bookId, type, sourceType, ownerActorUniqueId, newPhotoName));
}

BP_FILLER(PlayerActionPacket, 5)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto action = w.i32();
    const auto blockPosition = makeBlockPosition(w);
    const auto resultPosition = makeBlockPosition(w);
    const auto face = w.i32();

    return std::make_unique<PlayerActionPacket>(
        PlayerActionPacket::create(actorRuntimeId, action, blockPosition, resultPosition, face));
}

BP_FILLER(PlayerStartItemCooldownPacket, 2)
{
    auto &w = ctx.well;
    const auto itemCategory = w.str("itemCategory");
    const auto cooldownTicks = w.i32();

    return std::make_unique<PlayerStartItemCooldownPacket>(
        PlayerStartItemCooldownPacket::create(itemCategory, cooldownTicks));
}

BP_FILLER(PositionTrackingDBClientRequestPacket, 2)
{
    auto &w = ctx.well;
    const auto action = w.u8();
    const auto trackingId = w.i32();

    return std::make_unique<PositionTrackingDBClientRequestPacket>(
        PositionTrackingDBClientRequestPacket::create(action, trackingId));
}

BP_FILLER(PositionTrackingDBServerBroadcastPacket, 3)
{
    auto &w = ctx.well;
    const auto action = w.u8();
    const auto trackingId = w.i32();
    auto nbt = makeCacheableNbt(w);

    return std::make_unique<PositionTrackingDBServerBroadcastPacket>(
        PositionTrackingDBServerBroadcastPacket::create(action, trackingId, std::move(nbt)));
}

BP_FILLER(PurchaseReceiptPacket, 1)
{
    auto &w = ctx.well;
    const std::vector<std::string> entries{w.str("receipt0"), w.str("receipt1"), w.str("receipt2")};

    return std::make_unique<PurchaseReceiptPacket>(PurchaseReceiptPacket::create(entries));
}

BP_FILLER(RequestNetworkSettingsPacket, 1)
{
    auto &w = ctx.well;
    const auto protocolVersion = w.u32();

    return std::make_unique<RequestNetworkSettingsPacket>(
        RequestNetworkSettingsPacket::create(protocolVersion));
}

BP_FILLER(RequestPermissionsPacket, 3)
{
    auto &w = ctx.well;
    const auto targetActorUniqueId = w.i64();
    const auto playerPermission = w.i32();
    const auto customFlags = w.u16();

    return std::make_unique<RequestPermissionsPacket>(
        RequestPermissionsPacket::create(targetActorUniqueId, playerPermission, customFlags));
}

BP_FILLER(ScriptMessagePacket, 2)
{
    auto &w = ctx.well;
    const auto messageId = w.str("messageId");
    const auto value = w.str("value");

    return std::make_unique<ScriptMessagePacket>(ScriptMessagePacket::create(messageId, value));
}

BP_FILLER(SendPartyDestinationCookiePacket, 3)
{
    auto &w = ctx.well;
    const auto cookie = w.str("cookie");
    const auto intent = w.str("intent");
    const auto destinationName = w.str("destinationName");

    return std::make_unique<SendPartyDestinationCookiePacket>(
        SendPartyDestinationCookiePacket::create(cookie, intent, destinationName));
}

}  // namespace bedrock_protocol::verify
