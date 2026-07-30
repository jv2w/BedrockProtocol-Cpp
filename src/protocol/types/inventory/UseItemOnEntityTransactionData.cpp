/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/UseItemOnEntityTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/UseItemOnEntityTransactionData.h"

#include <utility>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::VarInt;
using serializer::CommonTypes;

const std::int32_t UseItemOnEntityTransactionData::ID = InventoryTransactionPacket::TYPE_USE_ITEM_ON_ENTITY;

void UseItemOnEntityTransactionData::decodeData(encoding::ByteBufferReader &in)
{
    actorRuntimeId = CommonTypes::getActorRuntimeId(in);
    actionType = VarInt::readSignedInt(in);
    hotbarSlot = VarInt::readSignedInt(in);
    itemInHand = CommonTypes::getNetworkItemStackDescriptor(in);
    playerPosition = CommonTypes::getVector3(in);
    clickPosition = CommonTypes::getVector3(in);
}

void UseItemOnEntityTransactionData::encodeData(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    VarInt::writeSignedInt(out, actionType);
    VarInt::writeSignedInt(out, hotbarSlot);
    CommonTypes::putNetworkItemStackDescriptor(out, itemInHand);
    CommonTypes::putVector3(out, playerPosition);
    CommonTypes::putVector3(out, clickPosition);
}

UseItemOnEntityTransactionData UseItemOnEntityTransactionData::initSelf(
    const std::uint64_t actorRuntimeId, const std::int32_t actionType, const std::int32_t hotbarSlot,
    ItemStackWrapper itemInHand, math::Vector3 playerPosition, math::Vector3 clickPosition)
{
    UseItemOnEntityTransactionData result;
    result.actorRuntimeId = actorRuntimeId;
    result.actionType = actionType;
    result.hotbarSlot = hotbarSlot;
    result.itemInHand = std::move(itemInHand);
    result.playerPosition = playerPosition;
    result.clickPosition = clickPosition;
    return result;
}

UseItemOnEntityTransactionData UseItemOnEntityTransactionData::create(
    std::vector<NetworkInventoryAction> actions, const std::uint64_t actorRuntimeId, const std::int32_t actionType,
    const std::int32_t hotbarSlot, ItemStackWrapper itemInHand, math::Vector3 playerPosition,
    math::Vector3 clickPosition)
{
    UseItemOnEntityTransactionData result =
        initSelf(actorRuntimeId, actionType, hotbarSlot, std::move(itemInHand), playerPosition, clickPosition);
    result.actions = std::move(actions);
    return result;
}

}  // namespace bedrock_protocol::types::inventory
