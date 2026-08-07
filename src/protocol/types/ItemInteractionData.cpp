/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ItemInteractionData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ItemInteractionData.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

ItemInteractionData ItemInteractionData::read(encoding::ByteBufferReader &in)
{
    const auto requestId = VarInt::readSignedInt(in);
    // reader.go:244-247 - the changed slot list carries its own presence bool; it is not keyed off
    // requestId the way the pre-2168 format was.
    auto requestChangedSlots = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &r) {
        std::vector<inventory::InventoryTransactionChangedSlotsHack> slots;
        const auto len = VarInt::readUnsignedInt(r);
        for (std::uint32_t i = 0; i < len; ++i) {
            slots.push_back(inventory::InventoryTransactionChangedSlotsHack::read(r));
        }
        return slots;
    });
    inventory::UseItemTransactionData transactionData;
    transactionData.decodeAuthInput(in);
    return ItemInteractionData(requestId, std::move(requestChangedSlots), std::move(transactionData));
}

void ItemInteractionData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, requestId);
    // writer.go:182-184 - presence bool, then the slice when it is there.
    serializer::CommonTypes::writeOptional(
        out, requestChangedSlots,
        [](encoding::ByteBufferWriter &w,
           const std::vector<inventory::InventoryTransactionChangedSlotsHack> &slots) {
            VarInt::writeUnsignedInt(w, static_cast<std::uint32_t>(slots.size()));
            for (const auto &changedSlot : slots) {
                changedSlot.write(w);
            }
        });
    transactionData.encodeAuthInput(out);
}

}  // namespace bedrock_protocol::types
