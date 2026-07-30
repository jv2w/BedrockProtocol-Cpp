/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueString.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/cereal/DynamicValueString.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::cereal {

using serializer::CommonTypes;

DynamicValueString DynamicValueString::readValue(encoding::ByteBufferReader &in)
{
    return DynamicValueString(std::string(CommonTypes::getString(in)));
}

void DynamicValueString::writeValue(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, value);
}

}  // namespace bedrock_protocol::types::cereal
