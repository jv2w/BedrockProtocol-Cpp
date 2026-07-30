/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/InventoryTransactionChangedSlotsHack.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/InventoryTransactionChangedSlotsHack.h"

#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
using encoding::VarInt;

InventoryTransactionChangedSlotsHack InventoryTransactionChangedSlotsHack::read(encoding::ByteBufferReader &in)
{
    const auto containerId = Byte::readUnsigned(in);
    std::vector<std::uint8_t> changedSlots;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        changedSlots.push_back(Byte::readUnsigned(in));
    }
    return {containerId, std::move(changedSlots)};
}

void InventoryTransactionChangedSlotsHack::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, containerId);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(changedSlotIndexes.size()));
    for (const auto &index : changedSlotIndexes) {
        Byte::writeUnsigned(out, index);
    }
}

}  // namespace bedrock_protocol::types::inventory
