/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/TransactionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
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
    // reader.go:248-250 - the action slice is a DoubleOptionalFunc here, and its elements are the
    // ordinary InventoryAction (inventory.go:44-51), the same one the transaction path reads.
    if (Byte::readUnsigned(in) != 1) {
        throw PacketDecodeException("Inconsistent optional state for actions");
    }
    actions.clear();
    if (serializer::CommonTypes::getBool(in)) {
        const auto actionCount = VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < actionCount; ++i) {
            actions.push_back(NetworkInventoryAction().readTransaction(in));
        }
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
    // writer.go:184-186 - outer presence bool is always true, then the inner optional's own bool.
    // The action list is never absent on the way out, so both bools go out set.
    Byte::writeUnsigned(out, 1);
    serializer::CommonTypes::putBool(out, true);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actions.size()));
    for (const auto &action : actions) {
        action.writeTransaction(out);
    }
    encodeData(out);
}

}  // namespace bedrock_protocol::types::inventory
