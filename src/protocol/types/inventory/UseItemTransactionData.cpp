/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/UseItemTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/UseItemTransactionData.h"

#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
using encoding::VarInt;
using serializer::CommonTypes;

const std::int32_t UseItemTransactionData::ID = InventoryTransactionPacket::TYPE_USE_ITEM;

UseItemTransactionData UseItemTransactionData::initSelf(
    std::int32_t actionType, TriggerType triggerType, types::BlockPosition blockPosition, std::uint8_t face,
    std::int32_t hotbarSlot, ItemStackWrapper itemInHand, math::Vector3 playerPosition, math::Vector3 clickPosition,
    std::uint32_t blockRuntimeId, PredictedResult clientInteractPrediction, std::uint8_t clientCooldownState)
{
    UseItemTransactionData result;
    result.actionType = actionType;
    result.triggerType = triggerType;
    result.blockPosition = blockPosition;
    result.face = face;
    result.hotbarSlot = hotbarSlot;
    result.itemInHand = std::move(itemInHand);
    result.playerPosition = playerPosition;
    result.clickPosition = clickPosition;
    result.blockRuntimeId = blockRuntimeId;
    result.clientInteractPrediction = clientInteractPrediction;
    result.clientCooldownState = clientCooldownState;
    return result;
}

UseItemTransactionData UseItemTransactionData::create(
    std::vector<NetworkInventoryAction> actions, std::int32_t actionType, TriggerType triggerType,
    types::BlockPosition blockPosition, std::uint8_t face, std::int32_t hotbarSlot, ItemStackWrapper itemInHand,
    math::Vector3 playerPosition, math::Vector3 clickPosition, std::uint32_t blockRuntimeId,
    PredictedResult clientInteractPrediction, std::uint8_t clientCooldownState)
{
    auto result = initSelf(actionType, triggerType, blockPosition, face, hotbarSlot, std::move(itemInHand),
                           playerPosition, clickPosition, blockRuntimeId, clientInteractPrediction,
                           clientCooldownState);
    result.actions = std::move(actions);
    return result;
}

void UseItemTransactionData::decodeData(encoding::ByteBufferReader &in)
{
    actionType = VarInt::readSignedInt(in);
    triggerType = TriggerTypeFromPacket(Byte::readUnsigned(in));
    blockPosition = CommonTypes::getBlockPosition(in);
    face = Byte::readUnsigned(in);
    hotbarSlot = VarInt::readSignedInt(in);
    itemInHand = CommonTypes::getNetworkItemStackDescriptor(in);
    playerPosition = CommonTypes::getVector3(in);
    clickPosition = CommonTypes::getVector3(in);
    blockRuntimeId = VarInt::readUnsignedInt(in);
    clientInteractPrediction = PredictedResultFromPacket(Byte::readUnsigned(in));
    clientCooldownState = Byte::readUnsigned(in);
}

void UseItemTransactionData::encodeData(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, actionType);
    Byte::writeUnsigned(out, static_cast<std::uint8_t>(triggerType));
    CommonTypes::putBlockPosition(out, blockPosition);
    Byte::writeUnsigned(out, face);
    VarInt::writeSignedInt(out, hotbarSlot);
    CommonTypes::putNetworkItemStackDescriptor(out, itemInHand);
    CommonTypes::putVector3(out, playerPosition);
    CommonTypes::putVector3(out, clickPosition);
    VarInt::writeUnsignedInt(out, blockRuntimeId);
    Byte::writeUnsigned(out, static_cast<std::uint8_t>(clientInteractPrediction));
    Byte::writeUnsigned(out, clientCooldownState);
}

}  // namespace bedrock_protocol::types::inventory
