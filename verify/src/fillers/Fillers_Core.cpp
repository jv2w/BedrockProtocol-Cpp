/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: core gameplay packets.
 *
 * Two conventions hold across every filler file, and both exist for a reason.
 *
 * 1. One create() call, every argument spelled out. That is what makes field coverage a compile-time
 *    guarantee rather than a claim: a missing argument is a wrong-arity error and a field added
 *    upstream breaks the build. BP_FILLER static_asserts the declared count against create()'s arity.
 *
 * 2. Values are drawn into named locals first, never inline inside the create() call. C++ leaves the
 *    evaluation order of function arguments unspecified, so drawing inline would hand different
 *    values to different fields depending on the compiler, and the reported hex for a failure would
 *    not match between the offline MSVC build and the in-game clang-cl one.
 *
 * ctx.well.pin(X) marks a value that must be LEGAL rather than unique - a discriminator the decoder
 * branches on. Every use is accompanied by the branch it selects.
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/AnimateEntityPacket.h"
#include "bedrock_protocol/protocol/ChangeDimensionPacket.h"
#include "bedrock_protocol/protocol/ChangeMobPropertyPacket.h"
#include "bedrock_protocol/protocol/ClientMovementPredictionSyncPacket.h"
#include "bedrock_protocol/protocol/CorrectPlayerMovePredictionPacket.h"
#include "bedrock_protocol/protocol/DisconnectPacket.h"
#include "bedrock_protocol/protocol/EmotePacket.h"
#include "bedrock_protocol/protocol/LevelSoundEventPacket.h"
#include "bedrock_protocol/protocol/LoginPacket.h"
#include "bedrock_protocol/protocol/MobEffectPacket.h"
#include "bedrock_protocol/protocol/MotionPredictionHintsPacket.h"
#include "bedrock_protocol/protocol/MoveActorAbsolutePacket.h"
#include "bedrock_protocol/protocol/MovePlayerPacket.h"
#include "bedrock_protocol/protocol/MovementEffectPacket.h"
#include "bedrock_protocol/protocol/NetworkSettingsPacket.h"
#include "bedrock_protocol/protocol/NpcDialoguePacket.h"
#include "bedrock_protocol/protocol/PlayStatusPacket.h"
#include "bedrock_protocol/protocol/PlaySoundPacket.h"
#include "bedrock_protocol/protocol/PlayerAuthInputPacket.h"
#include "bedrock_protocol/protocol/RespawnPacket.h"
#include "bedrock_protocol/protocol/ServerPlayerPostMovePositionPacket.h"
#include "bedrock_protocol/protocol/SetActorDataPacket.h"
#include "bedrock_protocol/protocol/SetActorMotionPacket.h"
#include "bedrock_protocol/protocol/SetTitlePacket.h"
#include "bedrock_protocol/protocol/StartGamePacket.h"
#include "bedrock_protocol/protocol/TransferPacket.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

BP_FILLER(MovePlayerPacket, 10)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto position = w.vec3();
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    const auto headYaw = w.f32();
    const auto mode = ValueWell::pin(MovePlayerPacket::MODE_TELEPORT);
    const auto onGround = w.flag();
    const auto ridingActorRuntimeId = w.u64();
    // Engaged: the presence bool is always on the wire now, and only the engaged branch writes the
    // two LE32s behind it.
    const auto teleportCause = w.i32();
    const auto teleportItem = w.i32();
    auto teleportData = w.some(types::TeleportData(teleportCause, teleportItem));
    const auto tick = w.u64();

    return std::make_unique<MovePlayerPacket>(MovePlayerPacket::create(
        actorRuntimeId, position, pitch, yaw, headYaw, mode, onGround, ridingActorRuntimeId,
        std::move(teleportData), tick));
}

BP_FILLER(LevelSoundEventPacket, 8)
{
    auto &w = ctx.well;
    const auto sound = w.str("sound");
    const auto position = w.vec3();
    const auto extraData = w.i32();
    const auto entityType = w.str("entityType");
    const auto isBabyMob = w.flag();
    const auto disableRelativeVolume = w.flag();
    const auto actorUniqueId = w.i64();
    // Engaged on purpose: an empty optional would skip the value branch of writeOptional entirely.
    const auto firePosition = w.some(w.vec3());

    return std::make_unique<LevelSoundEventPacket>(LevelSoundEventPacket::create(
        sound, position, extraData, entityType, isBabyMob, disableRelativeVolume, actorUniqueId, firePosition));
}

BP_FILLER(SetTitlePacket, 8)
{
    auto &w = ctx.well;
    const auto type = w.i32();
    const auto text = w.str("text");
    const auto fadeInTime = w.i32();
    const auto stayTime = w.i32();
    const auto fadeOutTime = w.i32();
    const auto xuid = w.str("xuid");
    const auto platformOnlineId = w.str("platformOnlineId");
    const auto filteredTitleText = w.str("filteredTitleText");

    return std::make_unique<SetTitlePacket>(SetTitlePacket::create(type, text, fadeInTime, stayTime, fadeOutTime,
                                                                  xuid, platformOnlineId, filteredTitleText));
}

BP_FILLER(MobEffectPacket, 8)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto eventId = w.u8();
    const auto effectId = w.i32();
    const auto amplifier = w.i32();
    const auto particles = w.flag();
    const auto duration = w.i32();
    const auto tick = w.u64();
    const auto ambient = w.flag();

    return std::make_unique<MobEffectPacket>(MobEffectPacket::create(actorRuntimeId, eventId, effectId, amplifier,
                                                                     particles, duration, tick, ambient));
}

BP_FILLER(MoveActorAbsolutePacket, 6)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto position = w.vec3();
    // pitch/yaw/headYaw go through putRotationByte, which quantises a float to one byte. The suite
    // gates on BYTE equality, not value equality, so the quantisation is fine - what it does test is
    // that the quantisation is stable, i.e. that re-encoding a decoded rotation lands on the same
    // byte rather than drifting one step every time the bridge rewrites a packet.
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    const auto headYaw = w.f32();
    const auto flags = w.u8();

    return std::make_unique<MoveActorAbsolutePacket>(
        MoveActorAbsolutePacket::create(actorRuntimeId, position, pitch, yaw, headYaw, flags));
}

BP_FILLER(AnimateEntityPacket, 7)
{
    auto &w = ctx.well;
    const auto animation = w.str("animation");
    const auto nextState = w.str("nextState");
    const auto stopExpression = w.str("stopExpression");
    const auto stopExpressionVersion = w.i32();
    const auto controller = w.str("controller");
    const auto blendOutTime = w.f32();
    // Three entries, not one: a length prefix that is written but ignored, or an off-by-one in the
    // read loop, only shows up once the collection has more than one element.
    const std::vector<std::uint64_t> actorRuntimeIds = {w.u64(), w.u64(), w.u64()};

    return std::make_unique<AnimateEntityPacket>(AnimateEntityPacket::create(
        animation, nextState, stopExpression, stopExpressionVersion, controller, blendOutTime, actorRuntimeIds));
}

BP_FILLER(PlaySoundPacket, 8)
{
    auto &w = ctx.well;
    const auto soundName = w.str("soundName");
    // x/y/z are encoded as a BlockPosition of (coordinate * 8), so they are lossy by design - the
    // same is true in PHP. Byte equality still holds because the decoded value re-quantises to the
    // identical integer, which is exactly the property the bridge depends on.
    const auto x = w.f32();
    const auto y = w.f32();
    const auto z = w.f32();
    const auto volume = w.f32();
    const auto pitch = w.f32();
    const auto loopCount = w.i32();
    const auto serverSoundHandle = w.some(w.u64());

    return std::make_unique<PlaySoundPacket>(
        PlaySoundPacket::create(soundName, x, y, z, volume, pitch, loopCount, serverSoundHandle));
}

BP_FILLER(EmotePacket, 6)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto emoteId = w.str("emoteId");
    const auto emoteLengthTicks = w.u32();
    const auto xboxUserId = w.str("xboxUserId");
    const auto platformChatId = w.str("platformChatId");
    const auto flags = w.u8();

    return std::make_unique<EmotePacket>(
        EmotePacket::create(actorRuntimeId, emoteId, emoteLengthTicks, xboxUserId, platformChatId, flags));
}

BP_FILLER(NpcDialoguePacket, 6)
{
    auto &w = ctx.well;
    const auto npcActorUniqueId = w.i64();
    const auto actionType = w.i32();
    const auto dialogue = w.str("dialogue");
    const auto sceneName = w.str("sceneName");
    const auto npcName = w.str("npcName");
    const auto actionJson = w.str("actionJson");

    return std::make_unique<NpcDialoguePacket>(
        NpcDialoguePacket::create(npcActorUniqueId, actionType, dialogue, sceneName, npcName, actionJson));
}

BP_FILLER(ChangeMobPropertyPacket, 6)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto propertyName = w.str("propertyName");
    const auto boolValue = w.flag();
    const auto stringValue = w.str("stringValue");
    const auto intValue = w.i32();
    const auto floatValue = w.f32();

    return std::make_unique<ChangeMobPropertyPacket>(ChangeMobPropertyPacket::create(
        actorUniqueId, propertyName, boolValue, stringValue, intValue, floatValue));
}

BP_FILLER(SetActorDataPacket, 4)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    auto metadata = makeEntityMetadata(w);
    auto syncedProperties = makePropertySyncData(w);
    const auto tick = w.u64();

    return std::make_unique<SetActorDataPacket>(
        SetActorDataPacket::create(actorRuntimeId, std::move(metadata), std::move(syncedProperties), tick));
}

// ---- builders local to this file -------------------------------------------------------------
// These belong in WellTypes once more than one filler file needs them; today only StartGamePacket
// and PlayerAuthInputPacket do, and WellTypes is owned by another batch.
namespace {

/**
 * A NetworkInventoryAction as the auth-input path writes it.
 *
 * Since 2168 the auth-input actions are the ordinary InventoryAction (inventory.go:44-51), so
 * sourceType is no longer a discriminator over the framing - it is a plain varuint32 and windowId
 * and sourceFlags are both always present as DoubleOptionals.
 */
types::inventory::NetworkInventoryAction makeAuthInputInventoryAction(ValueWell &w)
{
    types::inventory::NetworkInventoryAction action;
    action.sourceType = w.u32();
    // windowId goes out as a signed byte, so anything wider would not survive its own encoding.
    action.windowId = w.some(static_cast<std::int32_t>(w.i8()));
    action.sourceFlags = w.some(w.u32());
    action.inventorySlot = w.u32();
    action.oldItem = makeItemStackWrapper(w);
    action.newItem = makeItemStackWrapper(w);

    return action;
}

types::ItemInteractionData makeItemInteractionData(ValueWell &w)
{
    const auto requestId = w.i32();

    const auto slotsContainerA = w.u8();
    const std::vector<std::uint8_t> slotIndexesA = {w.u8(), w.u8(), w.u8()};
    const auto slotsContainerB = w.u8();
    const std::vector<std::uint8_t> slotIndexesB = {w.u8(), w.u8()};
    std::vector<types::inventory::InventoryTransactionChangedSlotsHack> changedSlots;
    changedSlots.emplace_back(slotsContainerA, slotIndexesA);
    changedSlots.emplace_back(slotsContainerB, slotIndexesB);

    std::vector<types::inventory::NetworkInventoryAction> actions;
    actions.push_back(makeAuthInputInventoryAction(w));
    actions.push_back(makeAuthInputInventoryAction(w));

    const auto actionType = w.i32();
    // TriggerTypeFromPacket and PredictedResultFromPacket both reject unknown values, so these two are
    // pinned to legal members rather than drawn.
    const auto triggerType = ValueWell::pin(types::inventory::TriggerType::PLAYER_INPUT);
    const auto blockPosition = makeBlockPosition(w);
    const auto face = w.u8();
    const auto hotbarSlot = w.i32();
    auto itemInHand = makeItemStackWrapper(w);
    const auto playerPosition = w.vec3();
    const auto clickPosition = w.vec3();
    const auto blockRuntimeId = w.u32();
    const auto clientInteractPrediction = ValueWell::pin(types::inventory::PredictedResult::SUCCESS);
    const auto clientCooldownState = w.u8();

    auto transactionData = types::inventory::UseItemTransactionData::create(
        std::move(actions), actionType, triggerType, blockPosition, face, hotbarSlot, std::move(itemInHand),
        playerPosition, clickPosition, blockRuntimeId, clientInteractPrediction, clientCooldownState);

    return types::ItemInteractionData(requestId, w.some(std::move(changedSlots)), std::move(transactionData));
}

types::ServerJoinInformation makeServerJoinInformation(ValueWell &w)
{
    const auto experienceId = w.uuid();
    auto experienceName = w.str("experienceName");
    const auto experienceWorldId = w.uuid();
    auto experienceWorldName = w.str("experienceWorldName");
    auto creatorId = w.str("creatorId");
    const auto targetId = w.uuid();
    auto gatheringScenarioId = w.str("gatheringScenarioId");
    auto gatheringServerId = w.str("gatheringServerId");
    auto gatheringJoinInfo = w.some(types::GatheringJoinInfo(
        experienceId, std::move(experienceName), experienceWorldId, std::move(experienceWorldName),
        std::move(creatorId), targetId, std::move(gatheringScenarioId), std::move(gatheringServerId)));

    auto storeId = w.str("storeEntryId");
    auto storeName = w.str("storeEntryName");
    auto storeEntryPointInfo = w.some(types::StoreEntryPointInfo(std::move(storeId), std::move(storeName)));

    auto richPresenceId = w.some(w.str("richPresenceId"));
    auto presenceInfo = w.some(types::PresenceInfo(std::move(richPresenceId)));

    return {std::move(gatheringJoinInfo), std::move(storeEntryPointInfo), std::move(presenceInfo)};
}

types::ServerTelemetryData makeServerTelemetryData(ValueWell &w)
{
    auto serverId = w.str("telemetryServerId");
    auto scenarioId = w.str("telemetryScenarioId");
    auto worldId = w.str("telemetryWorldId");
    auto ownerId = w.str("telemetryOwnerId");

    return {std::move(serverId), std::move(scenarioId), std::move(worldId), std::move(ownerId)};
}

}  // namespace

BP_FILLER(StartGamePacket, 26)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto actorRuntimeId = w.u64();
    const auto playerGamemode = w.i32();
    const auto playerPosition = w.vec3();
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    auto playerActorProperties = makeCacheableNbt(w);
    auto levelSettings = makeLevelSettings(w);
    const auto levelId = w.str("levelId");
    const auto worldName = w.str("worldName");
    const auto premiumWorldTemplateId = w.str("premiumWorldTemplateId");
    const auto isTrial = w.flag();
    auto playerMovementSettings = makePlayerMovementSettings(w);
    const auto currentTick = w.u64();
    const auto enchantmentSeed = w.i32();
    const auto multiplayerCorrelationId = w.str("multiplayerCorrelationId");
    const auto enableNewInventorySystem = w.flag();
    const auto serverSoftwareVersion = w.str("serverSoftwareVersion");
    const auto worldTemplateId = w.uuid();
    const auto enableClientSideChunkGeneration = w.flag();
    const auto blockNetworkIdsAreHashes = w.flag();
    auto networkPermissions = makeNetworkPermissions(w);
    // Engaged, and with all three of its own sub-optionals engaged: an empty ServerJoinInformation
    // would collapse four nested writeOptional value branches into their absent form.
    auto serverJoinInformation = w.some(makeServerJoinInformation(w));
    auto serverTelemetryData = makeServerTelemetryData(w);
    std::vector<types::BlockPaletteEntry> blockPalette;
    blockPalette.push_back(makeBlockPaletteEntry(w));
    blockPalette.push_back(makeBlockPaletteEntry(w));
    blockPalette.push_back(makeBlockPaletteEntry(w));
    const auto blockPaletteChecksum = w.u64();

    return std::make_unique<StartGamePacket>(StartGamePacket::create(
        actorUniqueId, actorRuntimeId, playerGamemode, playerPosition, pitch, yaw, std::move(playerActorProperties),
        std::move(levelSettings), levelId, worldName, premiumWorldTemplateId, isTrial, std::move(playerMovementSettings),
        currentTick, enchantmentSeed, multiplayerCorrelationId, enableNewInventorySystem, serverSoftwareVersion,
        worldTemplateId, enableClientSideChunkGeneration, blockNetworkIdsAreHashes, std::move(networkPermissions),
        std::move(serverJoinInformation), std::move(serverTelemetryData), std::move(blockPalette),
        blockPaletteChecksum));
}

BP_FILLER(PlayerAuthInputPacket, 22)
{
    auto &w = ctx.well;
    const auto position = w.vec3();
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    const auto headYaw = w.f32();
    const auto moveVecX = w.f32();
    const auto moveVecZ = w.f32();
    // Exact size is mandatory: create() rejects any other, and decodePayload reads back with
    // NUMBER_OF_FLAGS and rejects every ID beyond it.
    auto inputFlags = makePlayerAuthInputFlagList(w, types::PlayerAuthInputFlags::NUMBER_OF_FLAGS);
    const auto inputMode = w.u32();
    const auto playMode = w.u32();
    const auto interactionMode = w.i32();
    const auto interactRotation = w.vec2();
    const auto tick = w.u64();
    const auto delta = w.vec3();
    // The five optionals below no longer depend on the input flags: each carries its own
    // DoubleOptional framing. All five are engaged so that no inner branch is left unwritten.
    auto itemInteractionData = w.some(makeItemInteractionData(w));
    auto itemStackRequest = w.some(makeItemStackRequest(w));
    std::vector<types::PlayerBlockAction> blockActionList;
    blockActionList.push_back(makePlayerBlockAction(w));
    blockActionList.push_back(makePlayerBlockAction(w));
    auto blockActions = w.some(std::move(blockActionList));
    auto vehicleRotation = w.some(w.vec2());
    auto clientPredictedVehicle = w.some(w.i64());
    const auto analogMoveVecX = w.f32();
    const auto analogMoveVecZ = w.f32();
    const auto cameraOrientation = w.vec3();
    const auto rawMove = w.vec2();

    return std::make_unique<PlayerAuthInputPacket>(PlayerAuthInputPacket::create(
        position, pitch, yaw, headYaw, moveVecX, moveVecZ, std::move(inputFlags), inputMode, playMode, interactionMode,
        interactRotation, tick, delta, std::move(itemInteractionData), std::move(itemStackRequest),
        std::move(blockActions), std::move(vehicleRotation), clientPredictedVehicle, analogMoveVecX, analogMoveVecZ,
        cameraOrientation, rawMove));
}

BP_FILLER(ClientMovementPredictionSyncPacket, 15)
{
    auto &w = ctx.well;
    // create() rejects any other length outright, and decodePayload reads back with FLAG_LENGTH.
    auto flags = makeBitSet(w, ClientMovementPredictionSyncPacket::FLAG_LENGTH);
    const auto scale = w.f32();
    const auto width = w.f32();
    const auto height = w.f32();
    const auto movementSpeed = w.f32();
    const auto underwaterMovementSpeed = w.f32();
    const auto lavaMovementSpeed = w.f32();
    const auto jumpStrength = w.f32();
    const auto health = w.f32();
    const auto hunger = w.f32();
    const auto frictionModifier = w.f32();
    const auto bounciness = w.f32();
    const auto airDragModifier = w.f32();
    const auto actorUniqueId = w.i64();
    const auto actorFlyingState = w.flag();

    return std::make_unique<ClientMovementPredictionSyncPacket>(ClientMovementPredictionSyncPacket::create(
        std::move(flags), scale, width, height, movementSpeed, underwaterMovementSpeed, lavaMovementSpeed,
        jumpStrength, health, hunger, frictionModifier, bounciness, airDragModifier, actorUniqueId, actorFlyingState));
}

BP_FILLER(CorrectPlayerMovePredictionPacket, 7)
{
    auto &w = ctx.well;
    const auto position = w.vec3();
    const auto delta = w.vec3();
    const auto onGround = w.flag();
    const auto tick = w.u64();
    // Every field is written unconditionally regardless of predictionType, so this only needs to be a
    // legal value rather than the branch that writes the most.
    const auto predictionType = ValueWell::pin<std::uint8_t>(CorrectPlayerMovePredictionPacket::PREDICTION_TYPE_VEHICLE);
    const auto vehicleRotation = w.vec2();
    const auto vehicleAngularVelocity = w.some(w.f32());

    return std::make_unique<CorrectPlayerMovePredictionPacket>(CorrectPlayerMovePredictionPacket::create(
        position, delta, onGround, tick, predictionType, vehicleRotation, vehicleAngularVelocity));
}

BP_FILLER(NetworkSettingsPacket, 5)
{
    auto &w = ctx.well;
    const auto compressionThreshold = w.u16();
    const auto compressionAlgorithm = w.u16();
    const auto enableClientThrottling = w.flag();
    const auto clientThrottleThreshold = w.u8();
    const auto clientThrottleScalar = w.f32();

    return std::make_unique<NetworkSettingsPacket>(NetworkSettingsPacket::create(
        compressionThreshold, compressionAlgorithm, enableClientThrottling, clientThrottleThreshold,
        clientThrottleScalar));
}

BP_FILLER(MovementEffectPacket, 4)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    // MovementEffectTypeFromPacket rejects anything but the two enumerated members, and INVALID would
    // encode as a five-byte varint of -1; GLIDE_BOOST is the ordinary case.
    const auto effectType = ValueWell::pin(types::MovementEffectType::GLIDE_BOOST);
    const auto duration = w.u32();
    const auto tick = w.u64();

    return std::make_unique<MovementEffectPacket>(
        MovementEffectPacket::create(actorRuntimeId, effectType, duration, tick));
}

BP_FILLER(ChangeDimensionPacket, 4)
{
    auto &w = ctx.well;
    const auto dimension = w.i32();
    const auto position = w.vec3();
    const auto respawn = w.flag();
    const auto loadingScreenId = w.some(w.u32());

    return std::make_unique<ChangeDimensionPacket>(
        ChangeDimensionPacket::create(dimension, position, respawn, loadingScreenId));
}

BP_FILLER(LoginPacket, 3)
{
    auto &w = ctx.well;
    const auto protocol = w.u32();
    // Both strings are length-prefixed inside a nested connection-request buffer that is itself
    // length-prefixed, so a wrong inner width shows up as a shifted outer payload rather than a short read.
    const auto authInfoJson = w.str("authInfoJson");
    const auto clientDataJwt = w.str("clientDataJwt");

    return std::make_unique<LoginPacket>(LoginPacket::create(protocol, authInfoJson, clientDataJwt));
}

BP_FILLER(MotionPredictionHintsPacket, 3)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto motion = w.vec3();
    const auto onGround = w.flag();

    return std::make_unique<MotionPredictionHintsPacket>(
        MotionPredictionHintsPacket::create(actorRuntimeId, motion, onGround));
}

BP_FILLER(SetActorMotionPacket, 3)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto motion = w.vec3();
    const auto tick = w.u64();

    return std::make_unique<SetActorMotionPacket>(SetActorMotionPacket::create(actorRuntimeId, motion, tick));
}

BP_FILLER(RespawnPacket, 3)
{
    auto &w = ctx.well;
    const auto position = w.vec3();
    const auto respawnState = ValueWell::pin<std::uint8_t>(RespawnPacket::CLIENT_READY_TO_SPAWN);
    const auto actorRuntimeId = w.u64();

    return std::make_unique<RespawnPacket>(RespawnPacket::create(position, respawnState, actorRuntimeId));
}

BP_FILLER(DisconnectPacket, 3)
{
    auto &w = ctx.well;
    const auto reason = w.i32();
    // Both engaged: encodePayload writes a "skip message" discriminator when BOTH are absent, and in
    // that branch neither string reaches the wire at all.
    const auto message = w.some(w.str("message"));
    const auto filteredMessage = w.some(w.str("filteredMessage"));

    return std::make_unique<DisconnectPacket>(DisconnectPacket::create(reason, message, filteredMessage));
}

BP_FILLER(TransferPacket, 4)
{
    auto &w = ctx.well;
    const auto address = w.str("address");
    const auto port = w.u16();
    const auto reloadWorld = w.flag();
    const auto experienceId = w.uuid();
    auto experienceName = w.str("experienceName");
    const auto experienceWorldId = w.uuid();
    auto experienceWorldName = w.str("experienceWorldName");
    auto creatorId = w.str("creatorId");
    const auto targetId = w.uuid();
    auto gatheringScenarioId = w.str("gatheringScenarioId");
    auto gatheringServerId = w.str("gatheringServerId");
    auto gatheringJoinInfo = w.some(types::GatheringJoinInfo(
        experienceId, std::move(experienceName), experienceWorldId, std::move(experienceWorldName),
        std::move(creatorId), targetId, std::move(gatheringScenarioId), std::move(gatheringServerId)));

    return std::make_unique<TransferPacket>(TransferPacket::create(address, port, reloadWorld, std::move(gatheringJoinInfo)));
}

BP_FILLER(PlayStatusPacket, 1)
{
    auto &w = ctx.well;
    const auto status = w.u32();

    return std::make_unique<PlayStatusPacket>(PlayStatusPacket::create(status));
}

BP_FILLER(ServerPlayerPostMovePositionPacket, 1)
{
    auto &w = ctx.well;
    const auto position = w.vec3();

    return std::make_unique<ServerPlayerPostMovePositionPacket>(
        ServerPlayerPostMovePositionPacket::create(position));
}

}  // namespace bedrock_protocol::verify
