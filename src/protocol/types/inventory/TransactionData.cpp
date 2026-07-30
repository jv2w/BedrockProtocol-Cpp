/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/TransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::inventory {

using encoding::VarInt;

void TransactionData::decodeTransaction(encoding::ByteBufferReader &in)
{
    const auto actionCount = VarInt::readUnsignedInt(in);
    actions.clear();
    for (std::uint32_t i = 0; i < actionCount; ++i) {
        actions.push_back(NetworkInventoryAction().readTransaction(in));
    }
    decodeData(in);
}

void TransactionData::decodeAuthInput(encoding::ByteBufferReader &in)
{
    const auto actionCount = VarInt::readUnsignedInt(in);
    actions.clear();
    for (std::uint32_t i = 0; i < actionCount; ++i) {
        actions.push_back(NetworkInventoryAction().readAuthInput(in));
    }
    decodeData(in);
}

void TransactionData::encodeTransaction(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actions.size()));
    for (const auto &action : actions) {
        action.writeTransaction(out);
    }
    encodeData(out);
}

void TransactionData::encodeAuthInput(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actions.size()));
    for (const auto &action : actions) {
        action.writeAuthInput(out);
    }
    encodeData(out);
}

}  // namespace bedrock_protocol::types::inventory
