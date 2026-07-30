/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * Fillers: UI, scoreboard and camera packets.
 *
 * The two conventions from Fillers_Core.cpp hold here too: one create() call with every argument
 * spelled out, and every value drawn into a named local first.
 *
 * Two packets in this batch have no generated create() and use BP_FILLER_NOCREATE:
 *   - TextPacket:      its factories are per-message-type helpers (raw/translation/popup/...), none
 *                      of which takes the full field set.
 *   - BossEventPacket: same shape - show/hide/registerPlayer/... each fill a subset.
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/BossEventPacket.h"
#include "bedrock_protocol/protocol/CameraAimAssistPacket.h"
#include "bedrock_protocol/protocol/CameraInstructionPacket.h"
#include "bedrock_protocol/protocol/CameraPresetsPacket.h"
#include "bedrock_protocol/protocol/CameraShakePacket.h"
#include "bedrock_protocol/protocol/ModalFormRequestPacket.h"
#include "bedrock_protocol/protocol/ModalFormResponsePacket.h"
#include "bedrock_protocol/protocol/NpcRequestPacket.h"
#include "bedrock_protocol/protocol/OnScreenTextureAnimationPacket.h"
#include "bedrock_protocol/protocol/OpenSignPacket.h"
#include "bedrock_protocol/protocol/PlayerFogPacket.h"
#include "bedrock_protocol/protocol/RemoveObjectivePacket.h"
#include "bedrock_protocol/protocol/ServerSettingsResponsePacket.h"
#include "bedrock_protocol/protocol/SetDisplayObjectivePacket.h"
#include "bedrock_protocol/protocol/SetHudPacket.h"
#include "bedrock_protocol/protocol/SetScorePacket.h"
#include "bedrock_protocol/protocol/SetScoreboardIdentityPacket.h"
#include "bedrock_protocol/protocol/ShowCreditsPacket.h"
#include "bedrock_protocol/protocol/TextPacket.h"
#include "bedrock_protocol/protocol/ToastRequestPacket.h"
#include "bedrock_protocol/protocol/types/ControlScheme.h"
#include "bedrock_protocol/protocol/types/camera/CameraPreset.h"
#include "bedrock_protocol/protocol/types/camera/CameraPresetAimAssist.h"
#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionEaseType.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

namespace {

// Camera nested types are used by exactly two packets in this file, so they live here rather than in
// WellTypes.h. If a later batch needs them from another filler file they should move.

types::camera::CameraSetInstruction makeCameraSetInstruction(ValueWell &w)
{
    const auto preset = w.u32();
    // IN_OUT_CUBIC is only required to be a legal ease id; the byte is written verbatim either way.
    const auto ease = w.some(types::camera::CameraSetInstructionEase(
        ValueWell::pin(types::camera::CameraSetInstructionEaseType::IN_OUT_CUBIC), w.f32()));
    const auto cameraPosition = w.some(w.vec3());
    const auto rotationPitch = w.f32();
    const auto rotationYaw = w.f32();
    const auto rotation = w.some(types::camera::CameraSetInstructionRotation(rotationPitch, rotationYaw));
    const auto facingPosition = w.some(w.vec3());
    const auto viewOffset = w.some(w.vec2());
    const auto entityOffset = w.some(w.vec3());
    const auto isDefault = w.some(w.flag());
    const auto ignoreStartingValuesComponent = w.flag();

    return {preset,      ease,          cameraPosition, rotation, facingPosition, viewOffset, entityOffset,
            isDefault,   ignoreStartingValuesComponent};
}

types::camera::CameraFadeInstruction makeCameraFadeInstruction(ValueWell &w)
{
    const auto fadeInTime = w.f32();
    const auto stayTime = w.f32();
    const auto fadeOutTime = w.f32();
    const auto red = w.f32();
    const auto green = w.f32();
    const auto blue = w.f32();

    return {w.some(types::camera::CameraFadeInstructionTime(fadeInTime, stayTime, fadeOutTime)),
            w.some(types::camera::CameraFadeInstructionColor(red, green, blue))};
}

types::camera::CameraSplineInstruction makeCameraSplineInstruction(ValueWell &w)
{
    const auto totalTime = w.f32();
    const auto easeType = ValueWell::pin(types::camera::CameraSetInstructionEaseType::OUT_BOUNCE);
    const std::vector<math::Vector3> curve = {w.vec3(), w.vec3(), w.vec3()};

    // The two option vectors carry their ease as a NAME on the wire, so a pinned id is enough.
    const auto progressValueA = w.f32();
    const auto progressTimeA = w.f32();
    const auto progressValueB = w.f32();
    const auto progressTimeB = w.f32();
    const std::vector<types::camera::CameraProgressOption> progressKeyFrames = {
        {progressValueA, progressTimeA, ValueWell::pin(types::camera::CameraSetInstructionEaseType::IN_SINE)},
        {progressValueB, progressTimeB, ValueWell::pin(types::camera::CameraSetInstructionEaseType::OUT_SINE)}};

    const auto rotationValueA = w.vec3();
    const auto rotationTimeA = w.f32();
    const auto rotationValueB = w.vec3();
    const auto rotationTimeB = w.f32();
    const std::vector<types::camera::CameraRotationOption> rotationOptions = {
        {rotationValueA, rotationTimeA, ValueWell::pin(types::camera::CameraSetInstructionEaseType::IN_BACK)},
        {rotationValueB, rotationTimeB, ValueWell::pin(types::camera::CameraSetInstructionEaseType::OUT_BACK)}};

    const auto splineIdentifier = w.str("splineIdentifier");
    const auto loadFromJson = w.flag();

    return {totalTime, easeType, curve, progressKeyFrames, rotationOptions, splineIdentifier, loadFromJson};
}

types::camera::CameraPreset makeCameraPreset(ValueWell &w)
{
    const auto name = w.str("presetName");
    const auto parent = w.str("presetParent");
    const auto xPosition = w.some(w.f32());
    const auto yPosition = w.some(w.f32());
    const auto zPosition = w.some(w.f32());
    const auto pitch = w.some(w.f32());
    const auto yaw = w.some(w.f32());
    const auto rotationSpeed = w.some(w.f32());
    const auto snapToTarget = w.some(w.flag());
    const auto horizontalRotationLimit = w.some(w.vec2());
    const auto verticalRotationLimit = w.some(w.vec2());
    const auto continueTargeting = w.some(w.flag());
    const auto blockListeningRadius = w.some(w.f32());
    const auto viewOffset = w.some(w.vec2());
    const auto entityOffset = w.some(w.vec3());
    const auto radius = w.some(w.f32());
    const auto yawLimitMin = w.some(w.f32());
    const auto yawLimitMax = w.some(w.f32());
    const auto audioListenerType = w.some(ValueWell::pin(types::camera::CameraPreset::AUDIO_LISTENER_TYPE_PLAYER));
    const auto playerEffects = w.some(w.flag());

    const auto aimAssistPresetId = w.some(w.str("aimAssistPresetId"));
    const auto aimAssistTargetMode = w.some(ValueWell::pin(types::camera::CameraAimAssistTargetMode::DISTANCE));
    const auto aimAssistViewAngle = w.some(w.vec2());
    const auto aimAssistDistance = w.some(w.f32());
    const auto aimAssist = w.some(types::camera::CameraPresetAimAssist(aimAssistPresetId, aimAssistTargetMode,
                                                                      aimAssistViewAngle, aimAssistDistance));
    const auto controlScheme = w.some(ValueWell::pin(types::ControlScheme::CAMERA_RELATIVE_STRAFE));

    return {name,
            parent,
            xPosition,
            yPosition,
            zPosition,
            pitch,
            yaw,
            rotationSpeed,
            snapToTarget,
            horizontalRotationLimit,
            verticalRotationLimit,
            continueTargeting,
            blockListeningRadius,
            viewOffset,
            entityOffset,
            radius,
            yawLimitMin,
            yawLimitMax,
            audioListenerType,
            playerEffects,
            aimAssist,
            controlScheme};
}

}  // namespace

BP_FILLER(CameraInstructionPacket, 9)
{
    auto &w = ctx.well;
    // Every one of the nine fields is an optional written through writeOptional. A disengaged one
    // costs a whole instruction type: the packet would still round-trip while testing one bool.
    const auto set = w.some(makeCameraSetInstruction(w));
    const auto clear = w.some(w.flag());
    const auto fade = w.some(makeCameraFadeInstruction(w));
    const auto targetCenterOffset = w.some(w.vec3());
    const auto targetActorUniqueId = w.i64();
    const auto target = w.some(types::camera::CameraTargetInstruction(targetCenterOffset, targetActorUniqueId));
    const auto removeTarget = w.some(w.flag());
    const auto fovValue = w.f32();
    const auto fovEaseTime = w.f32();
    const auto fovClear = w.flag();
    const auto fieldOfView = w.some(types::camera::CameraFovInstruction(
        fovValue, fovEaseTime, ValueWell::pin(types::camera::CameraSetInstructionEaseType::IN_OUT_ELASTIC), fovClear));
    const auto spline = w.some(makeCameraSplineInstruction(w));
    const auto attachToEntity = w.some(w.i64());
    const auto detachFromEntity = w.some(w.flag());

    return std::make_unique<CameraInstructionPacket>(CameraInstructionPacket::create(
        set, clear, fade, target, removeTarget, fieldOfView, spline, attachToEntity, detachFromEntity));
}

BP_FILLER_NOCREATE(TextPacket, 8)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<TextPacket>();

    // TYPE_TRANSLATION selects the CATEGORY_MESSAGE_WITH_PARAMETERS branch, the only one that writes
    // the `parameters` vector - and a length-prefixed collection is the more valuable thing to test
    // than the single extra string the authored-message branch would give. sourceName is therefore
    // NOT on the wire here; it is only written under TYPE_CHAT / TYPE_WHISPER / TYPE_ANNOUNCEMENT.
    packet->type = ValueWell::pin(static_cast<std::uint8_t>(TextPacket::TYPE_TRANSLATION));
    packet->needsTranslation = w.flag();
    packet->sourceName = w.str("sourceName");
    packet->message = w.str("message");
    packet->parameters = {w.str("parameterA"), w.str("parameterB"), w.str("parameterC")};
    packet->xboxUserId = w.str("xboxUserId");
    packet->platformChatId = w.str("platformChatId");
    packet->filteredMessage = w.some(w.str("filteredMessage"));

    return packet;
}

BP_FILLER_NOCREATE(BossEventPacket, 8)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<BossEventPacket>();

    // Unlike the PHP versions that branched per event type, both encodePayload and decodePayload of
    // this protocol revision write and read all eight fields unconditionally, so eventType needs no
    // pin beyond staying inside the declared range - TYPE_SHOW is the natural representative.
    packet->bossActorUniqueId = w.i64();
    packet->eventType = ValueWell::pin(static_cast<std::uint8_t>(BossEventPacket::TYPE_SHOW));
    packet->playerActorUniqueId = w.i64();
    packet->healthPercent = w.f32();
    packet->title = w.str("title");
    packet->filteredTitle = w.str("filteredTitle");
    packet->color = w.u8();
    packet->overlay = w.u8();

    return packet;
}

BP_FILLER(CameraAimAssistPacket, 6)
{
    auto &w = ctx.well;
    const auto presetId = w.str("presetId");
    const auto viewAngle = w.vec2();
    const auto distance = w.f32();
    // Both enums are validated on decode, so they must be legal members rather than unique draws.
    const auto targetMode = ValueWell::pin(types::camera::CameraAimAssistTargetMode::DISTANCE);
    const auto actionType = ValueWell::pin(types::camera::CameraAimAssistActionType::CLEAR);
    const auto showDebugRender = w.flag();

    return std::make_unique<CameraAimAssistPacket>(
        CameraAimAssistPacket::create(presetId, viewAngle, distance, targetMode, actionType, showDebugRender));
}

BP_FILLER(SetDisplayObjectivePacket, 5)
{
    auto &w = ctx.well;
    const auto displaySlot = w.str("displaySlot");
    const auto objectiveName = w.str("objectiveName");
    const auto displayName = w.str("displayName");
    const auto criteriaName = w.str("criteriaName");
    const auto sortOrder = w.i32();

    return std::make_unique<SetDisplayObjectivePacket>(
        SetDisplayObjectivePacket::create(displaySlot, objectiveName, displayName, criteriaName, sortOrder));
}

BP_FILLER(NpcRequestPacket, 5)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto requestType = w.u8();
    const auto commandString = w.str("commandString");
    const auto actionIndex = w.u8();
    const auto sceneName = w.str("sceneName");

    return std::make_unique<NpcRequestPacket>(
        NpcRequestPacket::create(actorRuntimeId, requestType, commandString, actionIndex, sceneName));
}

BP_FILLER(CameraShakePacket, 4)
{
    auto &w = ctx.well;
    const auto intensity = w.f32();
    const auto duration = w.f32();
    const auto shakeType = w.u8();
    const auto shakeAction = w.u8();

    return std::make_unique<CameraShakePacket>(
        CameraShakePacket::create(intensity, duration, shakeType, shakeAction));
}

BP_FILLER(ModalFormResponsePacket, 3)
{
    auto &w = ctx.well;
    const auto formId = w.u32();
    // Engaged on purpose: the packet is three fields, two of them optionals, so a disengaged one
    // would leave the filler testing a single varint.
    const auto formData = w.some(w.str("formData"));
    const auto cancelReason = w.some(w.u8());

    return std::make_unique<ModalFormResponsePacket>(
        ModalFormResponsePacket::create(formId, formData, cancelReason));
}

BP_FILLER(SetScorePacket, 2)
{
    auto &w = ctx.well;
    // TYPE_REMOVE suppresses the per-entry type byte and its payload, dropping three of the six
    // ScorePacketEntry members off the wire. TYPE_CHANGE writes all of them.
    const auto type = ValueWell::pin(static_cast<std::uint8_t>(SetScorePacket::TYPE_CHANGE));
    const std::vector<types::ScorePacketEntry> entries = {makeScorePacketEntry(w), makeScorePacketEntry(w),
                                                          makeScorePacketEntry(w)};

    return std::make_unique<SetScorePacket>(SetScorePacket::create(type, entries));
}

BP_FILLER(SetScoreboardIdentityPacket, 2)
{
    auto &w = ctx.well;
    // Only TYPE_REGISTER_IDENTITY writes the entries' actorUniqueId; the clear form is a bare id list.
    const auto type = ValueWell::pin(SetScoreboardIdentityPacket::TYPE_REGISTER_IDENTITY);
    const std::vector<types::ScoreboardIdentityPacketEntry> entries = {makeScoreboardIdentityPacketEntry(w),
                                                                        makeScoreboardIdentityPacketEntry(w),
                                                                        makeScoreboardIdentityPacketEntry(w)};

    return std::make_unique<SetScoreboardIdentityPacket>(SetScoreboardIdentityPacket::create(type, entries));
}

BP_FILLER(SetHudPacket, 2)
{
    // Both members are validated enums, so all values here are pinned; three distinct elements still
    // prove the loop bounds and the order of the list.
    const std::vector<types::hud::HudElement> hudElements = {
        ValueWell::pin(types::hud::HudElement::PAPER_DOLL), ValueWell::pin(types::hud::HudElement::HORSE_HEALTH),
        ValueWell::pin(types::hud::HudElement::ITEM_TEXT)};
    const auto visibility = ValueWell::pin(types::hud::HudVisibility::RESET);

    return std::make_unique<SetHudPacket>(SetHudPacket::create(hudElements, visibility));
}

BP_FILLER(ModalFormRequestPacket, 2)
{
    auto &w = ctx.well;
    const auto formId = w.u32();
    const auto formData = w.str("formData");

    return std::make_unique<ModalFormRequestPacket>(ModalFormRequestPacket::create(formId, formData));
}

BP_FILLER(ServerSettingsResponsePacket, 2)
{
    auto &w = ctx.well;
    const auto formId = w.u32();
    const auto formData = w.str("formData");

    return std::make_unique<ServerSettingsResponsePacket>(
        ServerSettingsResponsePacket::create(formId, formData));
}

BP_FILLER(ToastRequestPacket, 2)
{
    auto &w = ctx.well;
    const auto title = w.str("title");
    const auto body = w.str("body");

    return std::make_unique<ToastRequestPacket>(ToastRequestPacket::create(title, body));
}

BP_FILLER(ShowCreditsPacket, 2)
{
    auto &w = ctx.well;
    const auto playerActorRuntimeId = w.u64();
    const auto status = w.i32();

    return std::make_unique<ShowCreditsPacket>(ShowCreditsPacket::create(playerActorRuntimeId, status));
}

BP_FILLER(OpenSignPacket, 2)
{
    auto &w = ctx.well;
    const auto blockPosition = makeBlockPosition(w);
    const auto front = w.flag();

    return std::make_unique<OpenSignPacket>(OpenSignPacket::create(blockPosition, front));
}

BP_FILLER(RemoveObjectivePacket, 1)
{
    auto &w = ctx.well;
    const auto objectiveName = w.str("objectiveName");

    return std::make_unique<RemoveObjectivePacket>(RemoveObjectivePacket::create(objectiveName));
}

BP_FILLER(PlayerFogPacket, 1)
{
    auto &w = ctx.well;
    const std::vector<std::string> fogLayers = {w.str("fogLayerA"), w.str("fogLayerB"), w.str("fogLayerC")};

    return std::make_unique<PlayerFogPacket>(PlayerFogPacket::create(fogLayers));
}

BP_FILLER(OnScreenTextureAnimationPacket, 1)
{
    auto &w = ctx.well;
    const auto effectId = w.u32();

    return std::make_unique<OnScreenTextureAnimationPacket>(
        OnScreenTextureAnimationPacket::create(effectId));
}

BP_FILLER(CameraPresetsPacket, 1)
{
    auto &w = ctx.well;
    // Two presets, both fully engaged: CameraPreset is 22 optionals and is by some margin the widest
    // nested type in this batch, so a single disengaged member hides a whole field.
    const std::vector<types::camera::CameraPreset> presets = {makeCameraPreset(w), makeCameraPreset(w)};

    return std::make_unique<CameraPresetsPacket>(CameraPresetsPacket::create(presets));
}

}  // namespace bedrock_protocol::verify
