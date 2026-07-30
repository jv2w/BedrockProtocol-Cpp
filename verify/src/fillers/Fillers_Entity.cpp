/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * Fillers: entity spawn, movement and equipment packets.
 *
 * See Fillers_Core.cpp for the two conventions every filler file holds to (one create() call with
 * every argument spelled out, and values drawn into named locals first).
 *
 * Two packets here have no generated create() and so assign their fields individually:
 *   - MoveActorDeltaPacket: its fields are conditional on `flags`.
 *   - InteractPacket: `position` is optional and action-dependent.
 */

#include "bedrock_protocol/verify/Filler.h"

#include <unordered_map>

#include "bedrock_protocol/protocol/ActorEventPacket.h"
#include "bedrock_protocol/protocol/ActorPickRequestPacket.h"
#include "bedrock_protocol/protocol/AddActorPacket.h"
#include "bedrock_protocol/protocol/AddItemActorPacket.h"
#include "bedrock_protocol/protocol/AddPaintingPacket.h"
#include "bedrock_protocol/protocol/AddPlayerPacket.h"
#include "bedrock_protocol/protocol/AnimatePacket.h"
#include "bedrock_protocol/protocol/EmoteListPacket.h"
#include "bedrock_protocol/protocol/InteractPacket.h"
#include "bedrock_protocol/protocol/MobArmorEquipmentPacket.h"
#include "bedrock_protocol/protocol/MobEquipmentPacket.h"
#include "bedrock_protocol/protocol/MoveActorDeltaPacket.h"
#include "bedrock_protocol/protocol/RemoveActorPacket.h"
#include "bedrock_protocol/protocol/SetActorLinkPacket.h"
#include "bedrock_protocol/protocol/SetHealthPacket.h"
#include "bedrock_protocol/protocol/SetPlayerGameTypePacket.h"
#include "bedrock_protocol/protocol/SpawnExperienceOrbPacket.h"
#include "bedrock_protocol/protocol/TakeItemActorPacket.h"
#include "bedrock_protocol/protocol/UpdateAbilitiesPacket.h"
#include "bedrock_protocol/protocol/UpdateAttributesPacket.h"
#include "bedrock_protocol/protocol/UpdatePlayerGameTypePacket.h"
#include "bedrock_protocol/protocol/types/AbilitiesData.h"
#include "bedrock_protocol/protocol/types/AbilitiesLayer.h"
#include "bedrock_protocol/protocol/types/entity/UpdateAttribute.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {
namespace {

/**
 * UpdateAttribute is a distinct type from Attribute (three extra default* floats) and only
 * UpdateAttributesPacket carries it, so the builder stays local rather than growing WellTypes.
 */
types::entity::UpdateAttribute makeUpdateAttribute(ValueWell &w)
{
    auto id = w.str("updateAttributeId");
    const auto min = w.f32();
    const auto max = w.f32();
    const auto current = w.f32();
    const auto defaultMin = w.f32();
    const auto defaultMax = w.f32();
    const auto defaultValue = w.f32();
    std::vector<types::entity::AttributeModifier> modifiers = {makeAttributeModifier(w), makeAttributeModifier(w)};

    return {std::move(id), min, max, current, defaultMin, defaultMax, defaultValue, std::move(modifiers)};
}

types::AbilitiesLayer makeAbilitiesLayer(ValueWell &w)
{
    const auto layerId = w.u16();
    // Every bool ability the decoder iterates, minus the two indices it skips because they are the
    // speed flags. Setting only a couple would leave most bits of setAbilities always zero, so a
    // shift-by-wrong-index would re-encode to the same word.
    std::unordered_map<std::int32_t, bool> boolAbilities;
    for (std::int32_t ability = 0; ability < types::AbilitiesLayer::NUMBER_OF_ABILITIES; ++ability) {
        if (ability == types::AbilitiesLayer::ABILITY_VERTICAL_FLY_SPEED) {
            continue;  // set implicitly by verticalFlySpeed below
        }
        if (ability == 13 || ability == 14) {
            continue;  // ABILITY_FLY_SPEED / ABILITY_WALK_SPEED: private, driven by the optionals
        }
        boolAbilities[ability] = w.flag();
    }
    // All three engaged: an absent speed forces its float to zero, and the decoder throws if a
    // non-zero speed arrives with its bit clear.
    const auto flySpeed = w.some(w.f32());
    const auto verticalFlySpeed = w.some(w.f32());
    const auto walkSpeed = w.some(w.f32());

    return {layerId, std::move(boolAbilities), flySpeed, verticalFlySpeed, walkSpeed};
}

types::AbilitiesData makeAbilitiesData(ValueWell &w)
{
    const auto commandPermission = w.u8();
    const auto playerPermission = w.u8();
    const auto targetActorUniqueId = w.i64();
    // The layer count goes out as one byte, so more than one layer is what proves the loop bound.
    std::vector<types::AbilitiesLayer> layers = {makeAbilitiesLayer(w), makeAbilitiesLayer(w)};

    return {commandPermission, playerPermission, targetActorUniqueId, std::move(layers)};
}

}  // namespace

BP_FILLER(AddActorPacket, 13)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto actorRuntimeId = w.u64();
    auto type = w.str("type");
    const auto position = w.vec3();
    const auto motion = w.some(w.vec3());
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    const auto headYaw = w.f32();
    const auto bodyYaw = w.f32();
    // encodePayload writes only id/min/current/max per attribute, and decodePayload rebuilds the
    // Attribute with defaultValue = current and no modifiers. That is lossy by value but exact by
    // byte, which is the only thing the suite gates on - and it matches the PHP original.
    std::vector<types::entity::Attribute> attributes = {makeAttribute(w), makeAttribute(w)};
    auto metadata = makeEntityMetadata(w);
    auto syncedProperties = makePropertySyncData(w);
    std::vector<types::entity::EntityLink> links = {makeEntityLink(w), makeEntityLink(w), makeEntityLink(w)};

    return std::make_unique<AddActorPacket>(AddActorPacket::create(
        actorUniqueId, actorRuntimeId, std::move(type), position, motion, pitch, yaw, headYaw, bodyYaw,
        std::move(attributes), std::move(metadata), std::move(syncedProperties), std::move(links)));
}

BP_FILLER(AddPlayerPacket, 17)
{
    auto &w = ctx.well;
    const auto playerUuid = w.uuid();
    auto username = w.str("username");
    const auto actorRuntimeId = w.u64();
    auto platformChatId = w.str("platformChatId");
    const auto position = w.vec3();
    const auto motion = w.some(w.vec3());
    const auto pitch = w.f32();
    const auto yaw = w.f32();
    const auto headYaw = w.f32();
    auto item = makeItemStackWrapper(w);
    const auto gameMode = w.i32();
    auto metadata = makeEntityMetadata(w);
    auto syncedProperties = makePropertySyncData(w);
    // The whole UpdateAbilitiesPacket body is inlined here by encodeBody/decodeBody, so its data
    // must be engaged - encodePayload dereferences the optional unconditionally.
    auto abilitiesPacket = UpdateAbilitiesPacket::create(makeAbilitiesData(w));
    std::vector<types::entity::EntityLink> links = {makeEntityLink(w), makeEntityLink(w), makeEntityLink(w)};
    auto deviceId = w.str("deviceId");
    const auto buildPlatform = w.i32();

    return std::make_unique<AddPlayerPacket>(AddPlayerPacket::create(
        playerUuid, std::move(username), actorRuntimeId, std::move(platformChatId), position, motion, pitch, yaw,
        headYaw, std::move(item), gameMode, std::move(metadata), std::move(syncedProperties),
        std::move(abilitiesPacket), std::move(links), std::move(deviceId), buildPlatform));
}

BP_FILLER(AddItemActorPacket, 7)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto actorRuntimeId = w.u64();
    auto item = makeItemStackWrapper(w);
    const auto position = w.vec3();
    const auto motion = w.some(w.vec3());
    auto metadata = makeEntityMetadata(w);
    const auto isFromFishing = w.flag();

    return std::make_unique<AddItemActorPacket>(AddItemActorPacket::create(
        actorUniqueId, actorRuntimeId, std::move(item), position, motion, std::move(metadata), isFromFishing));
}

BP_FILLER(AddPaintingPacket, 5)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto actorRuntimeId = w.u64();
    const auto position = w.vec3();
    const auto direction = w.i32();
    auto title = w.str("title");

    return std::make_unique<AddPaintingPacket>(
        AddPaintingPacket::create(actorUniqueId, actorRuntimeId, position, direction, std::move(title)));
}

BP_FILLER_NOCREATE(MoveActorDeltaPacket, 8)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<MoveActorDeltaPacket>();
    packet->actorRuntimeId = w.u64();
    // Every FLAG_HAS_* bit set: each coordinate and rotation is written only when its own bit is on,
    // so any other value would leave up to six of the eight fields entirely untested. The three
    // non-positional bits are included too, since flags itself is a plain u16 on the wire.
    packet->flags = ValueWell::pin<std::uint16_t>(
        MoveActorDeltaPacket::FLAG_HAS_X | MoveActorDeltaPacket::FLAG_HAS_Y | MoveActorDeltaPacket::FLAG_HAS_Z |
        MoveActorDeltaPacket::FLAG_HAS_PITCH | MoveActorDeltaPacket::FLAG_HAS_YAW |
        MoveActorDeltaPacket::FLAG_HAS_HEAD_YAW | MoveActorDeltaPacket::FLAG_GROUND |
        MoveActorDeltaPacket::FLAG_TELEPORT | MoveActorDeltaPacket::FLAG_FORCE_MOVE_LOCAL_ENTITY);
    packet->xPos = w.f32();
    packet->yPos = w.f32();
    packet->zPos = w.f32();
    packet->pitch = w.f32();
    packet->yaw = w.f32();
    packet->headYaw = w.f32();

    return packet;
}

BP_FILLER(MobArmorEquipmentPacket, 6)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    auto head = makeItemStackWrapper(w);
    auto chest = makeItemStackWrapper(w);
    auto legs = makeItemStackWrapper(w);
    auto feet = makeItemStackWrapper(w);
    auto body = makeItemStackWrapper(w);

    return std::make_unique<MobArmorEquipmentPacket>(MobArmorEquipmentPacket::create(
        actorRuntimeId, std::move(head), std::move(chest), std::move(legs), std::move(feet), std::move(body)));
}

BP_FILLER(MobEquipmentPacket, 5)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    auto item = makeItemStackWrapper(w);
    const auto inventorySlot = w.u8();
    const auto hotbarSlot = w.u8();
    const auto windowId = w.u8();

    return std::make_unique<MobEquipmentPacket>(
        MobEquipmentPacket::create(actorRuntimeId, std::move(item), inventorySlot, hotbarSlot, windowId));
}

BP_FILLER(UpdateAttributesPacket, 3)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    std::vector<types::entity::UpdateAttribute> entries = {makeUpdateAttribute(w), makeUpdateAttribute(w)};
    const auto tick = w.u64();

    return std::make_unique<UpdateAttributesPacket>(
        UpdateAttributesPacket::create(actorRuntimeId, std::move(entries), tick));
}

BP_FILLER(ActorEventPacket, 4)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto eventId = w.u8();
    const auto eventData = w.i32();
    const auto firePosition = w.some(w.vec3());

    return std::make_unique<ActorEventPacket>(
        ActorEventPacket::create(actorRuntimeId, eventId, eventData, firePosition));
}

BP_FILLER(AnimatePacket, 4)
{
    auto &w = ctx.well;
    const auto actorRuntimeId = w.u64();
    const auto action = w.u8();
    const auto data = w.f32();
    const auto swingSource = w.some(w.str("swingSource"));

    return std::make_unique<AnimatePacket>(AnimatePacket::create(actorRuntimeId, action, data, swingSource));
}

BP_FILLER_NOCREATE(InteractPacket, 3)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<InteractPacket>();
    packet->action = w.u8();
    packet->targetActorRuntimeId = w.u64();
    packet->position = w.some(w.vec3());

    return packet;
}

BP_FILLER(ActorPickRequestPacket, 3)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto hotbarSlot = w.u8();
    const auto addUserData = w.flag();

    return std::make_unique<ActorPickRequestPacket>(
        ActorPickRequestPacket::create(actorUniqueId, hotbarSlot, addUserData));
}

BP_FILLER(UpdatePlayerGameTypePacket, 3)
{
    auto &w = ctx.well;
    const auto gameMode = w.i32();
    const auto playerActorUniqueId = w.i64();
    const auto tick = w.u64();

    return std::make_unique<UpdatePlayerGameTypePacket>(
        UpdatePlayerGameTypePacket::create(gameMode, playerActorUniqueId, tick));
}

BP_FILLER(TakeItemActorPacket, 2)
{
    auto &w = ctx.well;
    const auto takerActorRuntimeId = w.u64();
    const auto itemActorRuntimeId = w.u64();

    return std::make_unique<TakeItemActorPacket>(
        TakeItemActorPacket::create(takerActorRuntimeId, itemActorRuntimeId));
}

BP_FILLER(SpawnExperienceOrbPacket, 2)
{
    auto &w = ctx.well;
    const auto position = w.vec3();
    const auto amount = w.i32();

    return std::make_unique<SpawnExperienceOrbPacket>(SpawnExperienceOrbPacket::create(position, amount));
}

BP_FILLER(EmoteListPacket, 2)
{
    auto &w = ctx.well;
    const auto playerActorRuntimeId = w.u64();
    std::vector<uuid::Uuid> emoteIds = {w.uuid(), w.uuid(), w.uuid()};

    return std::make_unique<EmoteListPacket>(EmoteListPacket::create(playerActorRuntimeId, std::move(emoteIds)));
}

BP_FILLER(RemoveActorPacket, 1)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();

    return std::make_unique<RemoveActorPacket>(RemoveActorPacket::create(actorUniqueId));
}

BP_FILLER(SetActorLinkPacket, 1)
{
    auto &w = ctx.well;
    auto link = makeEntityLink(w);

    return std::make_unique<SetActorLinkPacket>(SetActorLinkPacket::create(std::move(link)));
}

BP_FILLER(SetHealthPacket, 1)
{
    auto &w = ctx.well;
    const auto health = w.i32();

    return std::make_unique<SetHealthPacket>(SetHealthPacket::create(health));
}

BP_FILLER(SetPlayerGameTypePacket, 1)
{
    auto &w = ctx.well;
    const auto gamemode = w.i32();

    return std::make_unique<SetPlayerGameTypePacket>(SetPlayerGameTypePacket::create(gamemode));
}

}  // namespace bedrock_protocol::verify
