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

namespace bedrock_protocol::types {

using encoding::VarInt;

ItemInteractionData ItemInteractionData::read(encoding::ByteBufferReader &in)
{
    const auto requestId = VarInt::readSignedInt(in);
    std::vector<inventory::InventoryTransactionChangedSlotsHack> requestChangedSlots;
    if (requestId != 0) {
        const auto len = VarInt::readUnsignedInt(in);
        for (std::uint32_t i = 0; i < len; ++i) {
            requestChangedSlots.push_back(inventory::InventoryTransactionChangedSlotsHack::read(in));
        }
    }
    inventory::UseItemTransactionData transactionData;
    transactionData.decodeAuthInput(in);
    return ItemInteractionData(requestId, std::move(requestChangedSlots), std::move(transactionData));
}

void ItemInteractionData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, requestId);
    if (requestId != 0) {
        VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(requestChangedSlots.size()));
        for (const auto &changedSlot : requestChangedSlots) {
            changedSlot.write(out);
        }
    }
    transactionData.encodeAuthInput(out);
}

}  // namespace bedrock_protocol::types
