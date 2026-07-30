/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/FullContainerName.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/FullContainerName.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

FullContainerName FullContainerName::read(encoding::ByteBufferReader &in)
{
    const auto containerId = Byte::readUnsigned(in);
    auto dynamicId = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return LE::readUnsignedInt(in); });
    return FullContainerName(containerId, dynamicId);
}

void FullContainerName::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, containerId);
    CommonTypes::writeOptional(out, dynamicId,
                               [](encoding::ByteBufferWriter &out, std::uint32_t value) { LE::writeUnsignedInt(out, value); });
}

}  // namespace bedrock_protocol::types::inventory
