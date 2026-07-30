/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/NormalTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/NormalTransactionData.h"

#include <utility>

#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"

namespace bedrock_protocol::types::inventory {

const std::int32_t NormalTransactionData::ID = InventoryTransactionPacket::TYPE_NORMAL;

void NormalTransactionData::decodeData(encoding::ByteBufferReader & /*in*/)
{
}

void NormalTransactionData::encodeData(encoding::ByteBufferWriter & /*out*/) const
{
}

NormalTransactionData NormalTransactionData::create(std::vector<NetworkInventoryAction> actions)
{
    NormalTransactionData result;
    result.actions = std::move(actions);
    return result;
}

}  // namespace bedrock_protocol::types::inventory
