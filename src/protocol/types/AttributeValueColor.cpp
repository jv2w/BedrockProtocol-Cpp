/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeValueColor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeValueColor.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/AttributeValueColorArray.h"
#include "bedrock_protocol/protocol/types/AttributeValueColorString.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

AttributeValueColor AttributeValueColor::read(encoding::ByteBufferReader &in)
{
    std::unique_ptr<AttributeValueColorValue> value;
    switch (static_cast<std::int32_t>(VarInt::readUnsignedInt(in))) {
    case AttributeValueColorArray::ID:
        value = std::make_unique<AttributeValueColorArray>(AttributeValueColorArray::read(in));
        break;
    case AttributeValueColorString::ID:
        value = std::make_unique<AttributeValueColorString>(AttributeValueColorString::read(in));
        break;
    default:
        throw PacketDecodeException("Unknown AttributeValueColor type");
    }
    auto operation = CommonTypes::getString(in);

    return AttributeValueColor(std::move(value), std::move(operation));
}

void AttributeValueColor::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(value->getTypeId()));
    value->write(out);
    CommonTypes::putString(out, operation);
}

}  // namespace bedrock_protocol::types
