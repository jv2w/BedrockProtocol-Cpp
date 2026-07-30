/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeValue.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/types/AttributeValueBool.h"
#include "bedrock_protocol/protocol/types/AttributeValueColor.h"
#include "bedrock_protocol/protocol/types/AttributeValueFloat.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

std::unique_ptr<AttributeValue> AttributeValue::read(encoding::ByteBufferReader &in)
{
    switch (static_cast<std::int32_t>(VarInt::readUnsignedInt(in))) {
    case AttributeValueBool::ID:
        return std::make_unique<AttributeValueBool>(AttributeValueBool::read(in));
    case AttributeValueFloat::ID:
        return std::make_unique<AttributeValueFloat>(AttributeValueFloat::read(in));
    case AttributeValueColor::ID:
        return std::make_unique<AttributeValueColor>(AttributeValueColor::read(in));
    default:
        throw PacketDecodeException("Unknown AttributeValue type");
    }
}

}  // namespace bedrock_protocol::types
