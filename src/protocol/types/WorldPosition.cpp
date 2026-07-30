/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/WorldPosition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/WorldPosition.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

WorldPosition WorldPosition::read(encoding::ByteBufferReader &in)
{
    const auto position = CommonTypes::getVector3(in);
    const auto dimension = VarInt::readSignedInt(in);
    return WorldPosition(position, dimension);
}

void WorldPosition::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putVector3(out, position);
    VarInt::writeSignedInt(out, dimension);
}

}  // namespace bedrock_protocol::types
