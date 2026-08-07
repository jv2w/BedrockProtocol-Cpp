/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerAuthInputPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerAuthInputPacket.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {

/**
 * The outer bool of a DoubleOptionalFunc is always written as true; the inner one carries the actual
 * presence (minecraft/protocol/io.go, function DoubleOptionalFunc).
 */
template <typename T, typename Writer>
void putDoubleOptional(encoding::ByteBufferWriter &out, const std::optional<T> &value, Writer writer)
{
    serializer::CommonTypes::putBool(out, true);
    serializer::CommonTypes::putBool(out, value.has_value());
    if (value.has_value()) {
        writer(out, *value);
    }
}

template <typename Reader>
auto getDoubleOptional(encoding::ByteBufferReader &in, Reader reader)
    -> std::optional<decltype(reader(in))>
{
    std::optional<decltype(reader(in))> result = std::nullopt;
    if (!serializer::CommonTypes::getBool(in)) {
        return result;
    }
    if (serializer::CommonTypes::getBool(in)) {
        result = reader(in);
    }
    return result;
}

}  // namespace

PlayerAuthInputPacket PlayerAuthInputPacket::internalCreate(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, types::PlayerAuthInputFlagList inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::int32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<types::PlayerBlockAction>> blockActions, std::optional<math::Vector2> vehicleRotation, std::optional<std::int64_t> clientPredictedVehicle, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove)
{
    PlayerAuthInputPacket result;
    result.position = std::move(position);
    result.pitch = pitch;
    result.yaw = yaw;
    result.headYaw = headYaw;
    result.moveVecX = moveVecX;
    result.moveVecZ = moveVecZ;
    result.inputFlags = std::move(inputFlags);
    result.inputMode = inputMode;
    result.playMode = playMode;
    result.interactionMode = interactionMode;
    result.interactRotation = std::move(interactRotation);
    result.tick = tick;
    result.delta = std::move(delta);
    result.itemInteractionData = std::move(itemInteractionData);
    result.itemStackRequest = std::move(itemStackRequest);
    result.blockActions = std::move(blockActions);
    result.vehicleRotation = std::move(vehicleRotation);
    result.clientPredictedVehicle = clientPredictedVehicle;
    result.analogMoveVecX = analogMoveVecX;
    result.analogMoveVecZ = analogMoveVecZ;
    result.cameraOrientation = std::move(cameraOrientation);
    result.rawMove = std::move(rawMove);
    return result;
}

PlayerAuthInputPacket PlayerAuthInputPacket::create(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, types::PlayerAuthInputFlagList inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::int32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<types::PlayerBlockAction>> blockActions, std::optional<math::Vector2> vehicleRotation, std::optional<std::int64_t> clientPredictedVehicle, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove)
{
    // An absent flag list is legal and has no size (input_flags.go:8-9); a present one must be sized
    // exactly, since decodePayload reads it back with NUMBER_OF_FLAGS and rejects any ID beyond it.
    if (inputFlags.isPresent() && inputFlags.getSize() != types::PlayerAuthInputFlags::NUMBER_OF_FLAGS) {
        throw std::invalid_argument("Input flags must hold " + std::to_string(types::PlayerAuthInputFlags::NUMBER_OF_FLAGS) + " flags");
    }

    return internalCreate(
        std::move(position),
        pitch,
        yaw,
        headYaw,
        moveVecX,
        moveVecZ,
        std::move(inputFlags),
        inputMode,
        playMode,
        interactionMode,
        std::move(interactRotation),
        tick,
        std::move(delta),
        std::move(itemInteractionData),
        std::move(itemStackRequest),
        std::move(blockActions),
        std::move(vehicleRotation),
        clientPredictedVehicle,
        analogMoveVecX,
        analogMoveVecZ,
        std::move(cameraOrientation),
        std::move(rawMove)
    );
}

void PlayerAuthInputPacket::decodePayload(encoding::ByteBufferReader &in)
{
    pitch = encoding::LE::readFloat(in);
    yaw = encoding::LE::readFloat(in);
    position = serializer::CommonTypes::getVector3(in);
    moveVecX = encoding::LE::readFloat(in);
    moveVecZ = encoding::LE::readFloat(in);
    headYaw = encoding::LE::readFloat(in);
    inputFlags = types::PlayerAuthInputFlagList::read(in, types::PlayerAuthInputFlags::NUMBER_OF_FLAGS);
    inputMode = encoding::VarInt::readUnsignedInt(in);
    playMode = encoding::VarInt::readUnsignedInt(in);
    interactionMode = encoding::VarInt::readSignedInt(in);
    interactRotation = serializer::CommonTypes::getVector2(in);
    tick = encoding::VarInt::readUnsignedLong(in);
    delta = serializer::CommonTypes::getVector3(in);
    // player_auth_input.go:186-194 - five DoubleOptionalFunc payloads, each with its own framing and
    // no dependence on the input flags.
    itemInteractionData = getDoubleOptional(in, [](encoding::ByteBufferReader &r) {
        return types::ItemInteractionData::read(r);
    });
    itemStackRequest = getDoubleOptional(in, [](encoding::ByteBufferReader &r) {
        return types::inventory::stackrequest::ItemStackRequest::read(r);
    });
    blockActions = getDoubleOptional(in, [](encoding::ByteBufferReader &r) {
        std::vector<types::PlayerBlockAction> actions;
        // Not reserved up front: the count is attacker-controlled, and a truncated payload must fail
        // on the reader rather than on a multi-gigabyte allocation.
        const auto count = encoding::VarInt::readUnsignedInt(r);
        for (std::uint32_t i = 0; i < count; ++i) {
            actions.push_back(types::PlayerBlockAction::read(r));
        }
        return actions;
    });
    vehicleRotation = getDoubleOptional(in, [](encoding::ByteBufferReader &r) {
        return serializer::CommonTypes::getVector2(r);
    });
    clientPredictedVehicle = getDoubleOptional(in, [](encoding::ByteBufferReader &r) {
        return serializer::CommonTypes::getActorUniqueId(r);
    });
    analogMoveVecX = encoding::LE::readFloat(in);
    analogMoveVecZ = encoding::LE::readFloat(in);
    cameraOrientation = serializer::CommonTypes::getVector3(in);
    rawMove = serializer::CommonTypes::getVector2(in);

}

void PlayerAuthInputPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, pitch);
    encoding::LE::writeFloat(out, yaw);
    serializer::CommonTypes::putVector3(out, position);
    encoding::LE::writeFloat(out, moveVecX);
    encoding::LE::writeFloat(out, moveVecZ);
    encoding::LE::writeFloat(out, headYaw);
    inputFlags.write(out);
    encoding::VarInt::writeUnsignedInt(out, inputMode);
    encoding::VarInt::writeUnsignedInt(out, playMode);
    encoding::VarInt::writeSignedInt(out, interactionMode);
    serializer::CommonTypes::putVector2(out, interactRotation);
    encoding::VarInt::writeUnsignedLong(out, tick);
    serializer::CommonTypes::putVector3(out, delta);
    putDoubleOptional(out, itemInteractionData,
                      [](encoding::ByteBufferWriter &w, const types::ItemInteractionData &v) { v.write(w); });
    putDoubleOptional(out, itemStackRequest,
                      [](encoding::ByteBufferWriter &w,
                         const types::inventory::stackrequest::ItemStackRequest &v) { v.write(w); });
    putDoubleOptional(out, blockActions,
                      [](encoding::ByteBufferWriter &w, const std::vector<types::PlayerBlockAction> &v) {
                          encoding::VarInt::writeUnsignedInt(w, static_cast<std::uint32_t>(v.size()));
                          for (const auto &blockAction : v) {
                              blockAction.write(w);
                          }
                      });
    putDoubleOptional(out, vehicleRotation, [](encoding::ByteBufferWriter &w, const math::Vector2 &v) {
        serializer::CommonTypes::putVector2(w, v);
    });
    putDoubleOptional(out, clientPredictedVehicle, [](encoding::ByteBufferWriter &w, const std::int64_t v) {
        serializer::CommonTypes::putActorUniqueId(w, v);
    });
    encoding::LE::writeFloat(out, analogMoveVecX);
    encoding::LE::writeFloat(out, analogMoveVecZ);
    serializer::CommonTypes::putVector3(out, cameraOrientation);
    serializer::CommonTypes::putVector2(out, rawMove);

}

bool PlayerAuthInputPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerAuthInput(*this);
}

}  // namespace bedrock_protocol
