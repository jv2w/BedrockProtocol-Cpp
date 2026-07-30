/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/CreativeItemEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/CreativeItemEntry.h"

#include <utility>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::VarInt;
using serializer::CommonTypes;

CreativeItemEntry CreativeItemEntry::read(encoding::ByteBufferReader &in)
{
    const auto entryId = CommonTypes::readCreativeItemNetId(in);
    auto item = CommonTypes::getItemStackWithoutStackId(in);
    const auto groupId = VarInt::readUnsignedInt(in);
    return {entryId, std::move(item), groupId};
}

void CreativeItemEntry::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeCreativeItemNetId(out, entryId);
    CommonTypes::putItemStackWithoutStackId(out, item);
    VarInt::writeUnsignedInt(out, groupId);
}

}  // namespace bedrock_protocol::types::inventory
