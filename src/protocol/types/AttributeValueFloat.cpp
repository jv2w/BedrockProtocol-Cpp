/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeValueFloat.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeValueFloat.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

AttributeValueFloat AttributeValueFloat::read(encoding::ByteBufferReader &in)
{
    const auto value = LE::readFloat(in);
    auto operation = CommonTypes::getString(in);

    return AttributeValueFloat(value, std::move(operation));
}

void AttributeValueFloat::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, value);
    CommonTypes::putString(out, operation);
}

}  // namespace bedrock_protocol::types
