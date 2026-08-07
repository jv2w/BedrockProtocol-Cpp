/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SystemCategory.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SystemCategory.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

SystemCategory SystemCategory::read(encoding::ByteBufferReader &in)
{
    auto categoryName = CommonTypes::getString(in);
    const auto systemIndex = LE::readUnsignedLong(in);

    return SystemCategory(std::move(categoryName), systemIndex);
}

void SystemCategory::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, categoryName);
    LE::writeUnsignedLong(out, systemIndex);
}

}  // namespace bedrock_protocol::types
