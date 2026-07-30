/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/ReleaseItemTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/ReleaseItemTransactionData.h"

#include <utility>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::VarInt;
using serializer::CommonTypes;

const std::int32_t ReleaseItemTransactionData::ID = InventoryTransactionPacket::TYPE_RELEASE_ITEM;

void ReleaseItemTransactionData::decodeData(encoding::ByteBufferReader &in)
{
    actionType = VarInt::readSignedInt(in);
    hotbarSlot = VarInt::readSignedInt(in);
    itemInHand = CommonTypes::getNetworkItemStackDescriptor(in);
    headPosition = CommonTypes::getVector3(in);
}

void ReleaseItemTransactionData::encodeData(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, actionType);
    VarInt::writeSignedInt(out, hotbarSlot);
    CommonTypes::putNetworkItemStackDescriptor(out, itemInHand);
    CommonTypes::putVector3(out, headPosition);
}

ReleaseItemTransactionData ReleaseItemTransactionData::initSelf(const std::int32_t actionType,
                                                                const std::int32_t hotbarSlot,
                                                                ItemStackWrapper itemInHand, math::Vector3 headPosition)
{
    ReleaseItemTransactionData result;
    result.actionType = actionType;
    result.hotbarSlot = hotbarSlot;
    result.itemInHand = std::move(itemInHand);
    result.headPosition = headPosition;
    return result;
}

ReleaseItemTransactionData ReleaseItemTransactionData::create(std::vector<NetworkInventoryAction> actions,
                                                              const std::int32_t actionType,
                                                              const std::int32_t hotbarSlot,
                                                              ItemStackWrapper itemInHand, math::Vector3 headPosition)
{
    ReleaseItemTransactionData result = initSelf(actionType, hotbarSlot, std::move(itemInHand), headPosition);
    result.actions = std::move(actions);
    return result;
}

}  // namespace bedrock_protocol::types::inventory
