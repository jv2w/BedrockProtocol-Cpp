/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponseContainerInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::inventory::stackresponse {

using encoding::VarInt;

ItemStackResponseContainerInfo ItemStackResponseContainerInfo::read(encoding::ByteBufferReader &in)
{
    auto containerName = FullContainerName::read(in);
    std::vector<ItemStackResponseSlotInfo> slots;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        slots.push_back(ItemStackResponseSlotInfo::read(in));
    }
    return ItemStackResponseContainerInfo(std::move(containerName), std::move(slots));
}

void ItemStackResponseContainerInfo::write(encoding::ByteBufferWriter &out) const
{
    containerName.write(out);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(slots.size()));
    for (const auto &slot : slots) {
        slot.write(out);
    }
}

}  // namespace bedrock_protocol::types::inventory::stackresponse
