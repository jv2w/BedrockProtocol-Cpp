/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/MismatchTransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/MismatchTransactionData.h"

#include <string>

#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::inventory {

const std::int32_t MismatchTransactionData::ID = InventoryTransactionPacket::TYPE_MISMATCH;

void MismatchTransactionData::decodeData(encoding::ByteBufferReader & /*in*/)
{
    if (actions.size() > 0) {
        throw PacketDecodeException("Mismatch transaction type should not have any actions associated with it, but got " +
                                    std::to_string(actions.size()));
    }
}

void MismatchTransactionData::encodeData(encoding::ByteBufferWriter & /*out*/) const
{
}

MismatchTransactionData MismatchTransactionData::create()
{
    return {};  //no arguments
}

}  // namespace bedrock_protocol::types::inventory
