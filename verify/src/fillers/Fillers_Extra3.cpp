/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: session, HUD and polymorphic-payload packets. See Fillers_Core.cpp for the two conventions
 * that hold across every filler file (one create() call with every argument spelled out; values drawn
 * into named locals first, never inline).
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/ClientboundAttributeLayerSyncPacket.h"
#include "bedrock_protocol/protocol/ClientboundDataStorePacket.h"
#include "bedrock_protocol/protocol/PlayerLocationPacket.h"
#include "bedrock_protocol/protocol/PlayerUpdateEntityOverridesPacket.h"
#include "bedrock_protocol/protocol/PlayerVideoCapturePacket.h"
#include "bedrock_protocol/protocol/PrimitiveShapesPacket.h"
#include "bedrock_protocol/protocol/ServerStatsPacket.h"
#include "bedrock_protocol/protocol/ServerToClientHandshakePacket.h"
#include "bedrock_protocol/protocol/ServerboundDataDrivenScreenClosedPacket.h"
#include "bedrock_protocol/protocol/ServerboundLoadingScreenPacket.h"
#include "bedrock_protocol/protocol/ServerboundPackSettingChangePacket.h"
#include "bedrock_protocol/protocol/SetCommandsEnabledPacket.h"
#include "bedrock_protocol/protocol/SetDefaultGameTypePacket.h"
#include "bedrock_protocol/protocol/SetLastHurtByPacket.h"
#include "bedrock_protocol/protocol/SetLocalPlayerAsInitializedPacket.h"
#include "bedrock_protocol/protocol/SetSpawnPositionPacket.h"
#include "bedrock_protocol/protocol/ShowProfilePacket.h"
#include "bedrock_protocol/protocol/ShowStoreOfferPacket.h"
#include "bedrock_protocol/protocol/SimpleEventPacket.h"
#include "bedrock_protocol/protocol/SimulationTypePacket.h"
#include "bedrock_protocol/protocol/StopSoundPacket.h"
#include "bedrock_protocol/protocol/SubClientLoginPacket.h"
#include "bedrock_protocol/protocol/SyncActorPropertyPacket.h"
#include "bedrock_protocol/protocol/SyncWorldClocksPacket.h"
#include "bedrock_protocol/protocol/TickingAreasLoadStatusPacket.h"
#include "bedrock_protocol/protocol/UpdateClientInputLocksPacket.h"
#include "bedrock_protocol/protocol/types/shape/PacketShapeData.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {
namespace {

/**
 * One shape with every one of its nine optionals engaged.
 *
 * The shape type is pinned to LINE because PacketShapeData::read rejects any payload whose type id
 * does not match the declared shape type, and makePrimitiveShapePayload produces a line payload.
 */
types::shape::PacketShapeData makePacketShapeData(ValueWell &w)
{
    const auto networkId = w.u64();
    const auto type = w.some(ValueWell::pin(types::shape::PrimitiveShapeType::LINE));
    const auto location = w.some(w.vec3());
    const auto scale = w.some(w.f32());
    const auto rotation = w.some(w.vec3());
    const auto totalTimeLeft = w.some(w.f32());
    const auto maximumRenderDistance = w.some(w.f32());
    const auto color = w.some(makeColor(w));
    const auto dimensionId = w.some(w.i32());
    const auto attachedToEntityId = w.some(w.u64());
    auto payload = makePrimitiveShapePayload(w);

    return types::shape::PacketShapeData{networkId,     type,
                                         location,      scale,
                                         rotation,      totalTimeLeft,
                                         maximumRenderDistance, color,
                                         dimensionId,   attachedToEntityId,
                                         std::move(payload)};
}

}  // namespace

BP_FILLER(ClientboundAttributeLayerSyncPacket, 1)
{
    auto &w = ctx.well;
    auto payload = makeAttributeLayerSyncPayload(w);

    return std::make_unique<ClientboundAttributeLayerSyncPacket>(
        ClientboundAttributeLayerSyncPacket::create(std::move(payload)));
}

BP_FILLER(ClientboundDataStorePacket, 1)
{
    auto &w = ctx.well;
    std::vector<std::unique_ptr<types::ddui::DataStoreOperation>> values;
    values.push_back(makeDataStoreOperation(w));
    values.push_back(makeDataStoreOperation(w));

    return std::make_unique<ClientboundDataStorePacket>(
        ClientboundDataStorePacket::create(std::move(values)));
}

BP_FILLER(PlayerLocationPacket, 3)
{
    auto &w = ctx.well;
    // PLAYER_LOCATION_COORDINATES is the only type under which encodePayload writes `position`; under
    // PLAYER_LOCATION_HIDE one of the three fields would never reach the wire.
    const auto type = ValueWell::pin(types::PlayerLocationType::PLAYER_LOCATION_COORDINATES);
    const auto actorUniqueId = w.i64();
    const auto position = w.some(w.vec3());

    return std::make_unique<PlayerLocationPacket>(
        PlayerLocationPacket::create(type, actorUniqueId, position));
}

BP_FILLER(PlayerUpdateEntityOverridesPacket, 5)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto propertyIndex = w.u32();
    // The two override values are mutually exclusive on the wire: SET_INT_OVERRIDE writes only
    // intOverrideValue, SET_FLOAT_OVERRIDE only floatOverrideValue, and the other two types write
    // neither. SET_INT_OVERRIDE is the maximum any single instance can exercise; floatOverrideValue
    // is therefore populated but not reachable by this filler.
    const auto updateType = ValueWell::pin(types::OverrideUpdateType::SET_INT_OVERRIDE);
    const auto intOverrideValue = w.some(w.i32());
    const auto floatOverrideValue = w.some(w.f32());

    return std::make_unique<PlayerUpdateEntityOverridesPacket>(PlayerUpdateEntityOverridesPacket::create(
        actorRuntimeId, propertyIndex, updateType, intOverrideValue, floatOverrideValue));
}

/*
 * NOCREATE: create() is private on this packet (it is on the PHP original too); the public surface is
 * createStartRecording / createStopRecording. createStartRecording assigns all three fields -
 * recording=true plus both optionals - which is also the only state under which encodePayload writes
 * anything beyond the leading bool, so this is full field coverage despite the missing static_assert.
 */
BP_FILLER_NOCREATE(PlayerVideoCapturePacket, 3)
{
    auto &w = ctx.well;
    const auto frameRate = w.u32();
    const auto filePrefix = w.str("filePrefix");

    return std::make_unique<PlayerVideoCapturePacket>(
        PlayerVideoCapturePacket::createStartRecording(frameRate, filePrefix));
}

BP_FILLER(PrimitiveShapesPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::shape::PacketShapeData> shapes;
    shapes.push_back(makePacketShapeData(w));
    shapes.push_back(makePacketShapeData(w));

    return std::make_unique<PrimitiveShapesPacket>(PrimitiveShapesPacket::create(std::move(shapes)));
}

BP_FILLER(ServerStatsPacket, 2)
{
    auto &w = ctx.well;
    const auto serverTime = w.f32();
    const auto networkTime = w.f32();

    return std::make_unique<ServerStatsPacket>(ServerStatsPacket::create(serverTime, networkTime));
}

BP_FILLER(ServerToClientHandshakePacket, 1)
{
    auto &w = ctx.well;
    const auto jwt = w.str("jwt");

    return std::make_unique<ServerToClientHandshakePacket>(
        ServerToClientHandshakePacket::create(jwt));
}

BP_FILLER(ServerboundDataDrivenScreenClosedPacket, 2)
{
    auto &w = ctx.well;
    const auto formId = w.u32();
    const auto closeReason = w.str("closeReason");

    return std::make_unique<ServerboundDataDrivenScreenClosedPacket>(
        ServerboundDataDrivenScreenClosedPacket::create(formId, closeReason));
}

BP_FILLER(ServerboundLoadingScreenPacket, 2)
{
    auto &w = ctx.well;
    // Pinned: decode runs the varint through LoadingScreenTypeFromPacket, which throws outside 0..2.
    const auto loadingScreenType = ValueWell::pin(types::hud::LoadingScreenType::STOP_LOADING_SCREEN);
    const auto loadingScreenId = w.some(w.u32());

    return std::make_unique<ServerboundLoadingScreenPacket>(
        ServerboundLoadingScreenPacket::create(loadingScreenType, loadingScreenId));
}

BP_FILLER(ServerboundPackSettingChangePacket, 2)
{
    auto &w = ctx.well;
    const auto packId = w.uuid();
    auto packSetting = makePackSetting(w);

    return std::make_unique<ServerboundPackSettingChangePacket>(
        ServerboundPackSettingChangePacket::create(packId, std::move(packSetting)));
}

BP_FILLER(SetCommandsEnabledPacket, 1)
{
    auto &w = ctx.well;
    // Pinned: the packet's only field is a bool, so a drawn `false` would encode identically to the
    // default-constructed packet and the anti-tautology stage would fail.
    const auto enabled = ValueWell::pin(true);
    (void)w;

    return std::make_unique<SetCommandsEnabledPacket>(SetCommandsEnabledPacket::create(enabled));
}

BP_FILLER(SetDefaultGameTypePacket, 1)
{
    auto &w = ctx.well;
    const auto gamemode = w.i32();

    return std::make_unique<SetDefaultGameTypePacket>(SetDefaultGameTypePacket::create(gamemode));
}

BP_FILLER(SetLastHurtByPacket, 1)
{
    auto &w = ctx.well;
    const auto entityTypeId = w.i32();

    return std::make_unique<SetLastHurtByPacket>(SetLastHurtByPacket::create(entityTypeId));
}

BP_FILLER(SetLocalPlayerAsInitializedPacket, 1)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();

    return std::make_unique<SetLocalPlayerAsInitializedPacket>(
        SetLocalPlayerAsInitializedPacket::create(actorRuntimeId));
}

BP_FILLER(SetSpawnPositionPacket, 4)
{
    auto &w = ctx.well;
    const auto spawnType = w.i32();
    const auto spawnPosition = makeBlockPosition(w);
    const auto dimension = w.i32();
    const auto causingBlockPosition = makeBlockPosition(w);

    return std::make_unique<SetSpawnPositionPacket>(
        SetSpawnPositionPacket::create(spawnType, spawnPosition, dimension, causingBlockPosition));
}

BP_FILLER(ShowProfilePacket, 1)
{
    auto &w = ctx.well;
    const auto xuid = w.str("xuid");

    return std::make_unique<ShowProfilePacket>(ShowProfilePacket::create(xuid));
}

BP_FILLER(ShowStoreOfferPacket, 2)
{
    auto &w = ctx.well;
    const auto offerId = w.uuid();
    // Pinned: decode validates the byte through ShowStoreOfferRedirectTypeFromPacket (0..2).
    const auto redirectType = ValueWell::pin(types::ShowStoreOfferRedirectType::THIRD_PARTY_SERVER_PAGE);

    return std::make_unique<ShowStoreOfferPacket>(ShowStoreOfferPacket::create(offerId, redirectType));
}

BP_FILLER(SimpleEventPacket, 1)
{
    auto &w = ctx.well;
    const auto eventType = w.u16();

    return std::make_unique<SimpleEventPacket>(SimpleEventPacket::create(eventType));
}

BP_FILLER(SimulationTypePacket, 1)
{
    auto &w = ctx.well;
    const auto type = w.u8();

    return std::make_unique<SimulationTypePacket>(SimulationTypePacket::create(type));
}

BP_FILLER(StopSoundPacket, 3)
{
    auto &w = ctx.well;
    const auto soundName = w.str("soundName");
    const auto stopAll = w.flag();
    const auto stopLegacyMusic = w.flag();

    return std::make_unique<StopSoundPacket>(
        StopSoundPacket::create(soundName, stopAll, stopLegacyMusic));
}

BP_FILLER(SubClientLoginPacket, 1)
{
    auto &w = ctx.well;
    const auto connectionRequestData = w.str("connectionRequestData");

    return std::make_unique<SubClientLoginPacket>(
        SubClientLoginPacket::create(connectionRequestData));
}

BP_FILLER(SyncActorPropertyPacket, 1)
{
    auto &w = ctx.well;
    auto nbt = makeCacheableNbt(w);

    return std::make_unique<SyncActorPropertyPacket>(
        SyncActorPropertyPacket::create(std::move(nbt)));
}

BP_FILLER(SyncWorldClocksPacket, 1)
{
    auto &w = ctx.well;
    auto payload = makeSyncWorldClocksPayload(w);

    return std::make_unique<SyncWorldClocksPacket>(
        SyncWorldClocksPacket::create(std::move(payload)));
}

BP_FILLER(TickingAreasLoadStatusPacket, 1)
{
    auto &w = ctx.well;
    // Pinned: single-bool packet, see SetCommandsEnabledPacket.
    const auto waitingForPreload = ValueWell::pin(true);
    (void)w;

    return std::make_unique<TickingAreasLoadStatusPacket>(
        TickingAreasLoadStatusPacket::create(waitingForPreload));
}

BP_FILLER(UpdateClientInputLocksPacket, 1)
{
    auto &w = ctx.well;
    const auto flags = w.u32();

    return std::make_unique<UpdateClientInputLocksPacket>(
        UpdateClientInputLocksPacket::create(flags));
}

}  // namespace bedrock_protocol::verify
