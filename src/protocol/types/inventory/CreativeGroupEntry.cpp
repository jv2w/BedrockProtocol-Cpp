/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/CreativeGroupEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/CreativeGroupEntry.h"

#include <utility>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::LE;
using serializer::CommonTypes;

CreativeGroupEntry CreativeGroupEntry::read(encoding::ByteBufferReader &in)
{
    const auto categoryId = LE::readSignedInt(in);
    auto categoryName = CommonTypes::getString(in);
    auto icon = CommonTypes::getItemStackWithoutStackId(in);
    return {categoryId, std::move(categoryName), std::move(icon)};
}

void CreativeGroupEntry::write(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedInt(out, categoryId);
    CommonTypes::putString(out, categoryName);
    CommonTypes::putItemStackWithoutStackId(out, icon);
}

}  // namespace bedrock_protocol::types::inventory
