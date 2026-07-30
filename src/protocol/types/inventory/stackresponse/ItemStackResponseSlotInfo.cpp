/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponseSlotInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory::stackresponse {

using encoding::Byte;
using encoding::VarInt;
using serializer::CommonTypes;

ItemStackResponseSlotInfo ItemStackResponseSlotInfo::read(encoding::ByteBufferReader &in)
{
    const auto slot = Byte::readUnsigned(in);
    const auto hotbarSlot = Byte::readUnsigned(in);
    const auto count = Byte::readUnsigned(in);
    const auto itemStackId = CommonTypes::readServerItemStackId(in);
    auto customName = CommonTypes::getString(in);
    auto filteredCustomName = CommonTypes::getString(in);
    const auto durabilityCorrection = VarInt::readSignedInt(in);
    return ItemStackResponseSlotInfo(slot, hotbarSlot, count, itemStackId, std::move(customName),
                                     std::move(filteredCustomName), durabilityCorrection);
}

void ItemStackResponseSlotInfo::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, slot);
    Byte::writeUnsigned(out, hotbarSlot);
    Byte::writeUnsigned(out, count);
    CommonTypes::writeServerItemStackId(out, itemStackId);
    CommonTypes::putString(out, customName);
    CommonTypes::putString(out, filteredCustomName);
    VarInt::writeSignedInt(out, durabilityCorrection);
}

}  // namespace bedrock_protocol::types::inventory::stackresponse
