/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayerAuthInputPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerAuthInputPacket.h"

#include <memory>
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

PlayerAuthInputPacket PlayerAuthInputPacket::internalCreate(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, serializer::BitSet inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::uint32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<std::unique_ptr<types::PlayerBlockAction>>> blockActions, std::optional<types::PlayerAuthInputVehicleInfo> vehicleInfo, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove)
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
    result.vehicleInfo = std::move(vehicleInfo);
    result.analogMoveVecX = analogMoveVecX;
    result.analogMoveVecZ = analogMoveVecZ;
    result.cameraOrientation = std::move(cameraOrientation);
    result.rawMove = std::move(rawMove);
    return result;
}

PlayerAuthInputPacket PlayerAuthInputPacket::create(math::Vector3 position, float pitch, float yaw, float headYaw, float moveVecX, float moveVecZ, serializer::BitSet inputFlags, std::uint32_t inputMode, std::uint32_t playMode, std::uint32_t interactionMode, math::Vector2 interactRotation, std::uint64_t tick, math::Vector3 delta, std::optional<types::ItemInteractionData> itemInteractionData, std::optional<types::inventory::stackrequest::ItemStackRequest> itemStackRequest, std::optional<std::vector<std::unique_ptr<types::PlayerBlockAction>>> blockActions, std::optional<types::PlayerAuthInputVehicleInfo> vehicleInfo, float analogMoveVecX, float analogMoveVecZ, math::Vector3 cameraOrientation, math::Vector2 rawMove)
{
    if (inputFlags.getLength() != types::PlayerAuthInputFlags::NUMBER_OF_FLAGS) {
        throw std::invalid_argument("Input flags must be " + std::to_string(types::PlayerAuthInputFlags::NUMBER_OF_FLAGS) + " bits long");
    }

    inputFlags.set(types::PlayerAuthInputFlags::PERFORM_ITEM_STACK_REQUEST, itemStackRequest.has_value());
    inputFlags.set(types::PlayerAuthInputFlags::PERFORM_ITEM_INTERACTION, itemInteractionData.has_value());
    inputFlags.set(types::PlayerAuthInputFlags::PERFORM_BLOCK_ACTIONS, blockActions.has_value());
    inputFlags.set(types::PlayerAuthInputFlags::IN_CLIENT_PREDICTED_VEHICLE, vehicleInfo.has_value());

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
        std::move(vehicleInfo),
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
    inputFlags = serializer::BitSet::read(in, types::PlayerAuthInputFlags::NUMBER_OF_FLAGS);
    inputMode = encoding::VarInt::readUnsignedInt(in);
    playMode = encoding::VarInt::readUnsignedInt(in);
    interactionMode = encoding::VarInt::readUnsignedInt(in);
    interactRotation = serializer::CommonTypes::getVector2(in);
    tick = encoding::VarInt::readUnsignedLong(in);
    delta = serializer::CommonTypes::getVector3(in);
    if (inputFlags.get(types::PlayerAuthInputFlags::PERFORM_ITEM_INTERACTION)) {
        itemInteractionData = types::ItemInteractionData::read(in);
    }
    if (inputFlags.get(types::PlayerAuthInputFlags::PERFORM_ITEM_STACK_REQUEST)) {
        itemStackRequest = types::inventory::stackrequest::ItemStackRequest::read(in);
    }
    if (inputFlags.get(types::PlayerAuthInputFlags::PERFORM_BLOCK_ACTIONS)) {
        blockActions.emplace();
        const auto max = encoding::VarInt::readSignedInt(in);
        for (std::int32_t i = 0; i < max; ++i) {
            const auto actionType = encoding::VarInt::readSignedInt(in);
            if (types::PlayerBlockActionWithBlockInfo::isValidActionType(actionType)) {
                blockActions->push_back(std::make_unique<types::PlayerBlockActionWithBlockInfo>(
                    types::PlayerBlockActionWithBlockInfo::read(in, actionType)));
            } else if (actionType == types::PlayerAction::STOP_BREAK) {
                blockActions->push_back(std::make_unique<types::PlayerBlockActionStopBreak>());
            } else {
                throw PacketDecodeException("Unexpected block action type " + std::to_string(actionType));
            }
        }
    }
    if (inputFlags.get(types::PlayerAuthInputFlags::IN_CLIENT_PREDICTED_VEHICLE)) {
        vehicleInfo = types::PlayerAuthInputVehicleInfo::read(in);
    }
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
    encoding::VarInt::writeUnsignedInt(out, interactionMode);
    serializer::CommonTypes::putVector2(out, interactRotation);
    encoding::VarInt::writeUnsignedLong(out, tick);
    serializer::CommonTypes::putVector3(out, delta);
    if (itemInteractionData.has_value()) {
        itemInteractionData->write(out);
    }
    if (itemStackRequest.has_value()) {
        itemStackRequest->write(out);
    }
    if (blockActions.has_value()) {
        encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(blockActions->size()));
        for (const auto &blockAction : *blockActions) {
            encoding::VarInt::writeSignedInt(out, blockAction->getActionType());
            blockAction->write(out);
        }
    }
    if (vehicleInfo.has_value()) {
        vehicleInfo->write(out);
    }
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
