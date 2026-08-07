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
    actions.clear();
    // io.go DoubleOptionalFunc - an outer bool of 0 means the whole value is absent and nothing at all
    // follows it, so it must not be treated as a malformed packet.
    actionsPresent = false;
    if (serializer::CommonTypes::getBool(in)) {
        actionsPresent = serializer::CommonTypes::getBool(in);
        if (actionsPresent) {
            const auto actionCount = VarInt::readUnsignedInt(in);
            for (std::uint32_t i = 0; i < actionCount; ++i) {
                actions.push_back(NetworkInventoryAction().readTransaction(in));
            }
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
    // writer.go:184-186 - the outer presence bool is always written set; the inner one mirrors whether
    // the list was actually there, so that re-encoding a decoded packet reproduces its shape. An absent
    // list that has since been given actions is written as present, or they would be dropped.
    Byte::writeUnsigned(out, 1);
    const auto present = actionsPresent || !actions.empty();
    serializer::CommonTypes::putBool(out, present);
    if (present) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actions.size()));
        for (const auto &action : actions) {
            action.writeTransaction(out);
        }
    }
    encodeData(out);
}

}  // namespace bedrock_protocol::types::inventory
