/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeLinePayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeLinePayload.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using serializer::CommonTypes;

PrimitiveShapeLinePayload PrimitiveShapeLinePayload::read(encoding::ByteBufferReader &in)
{
    const auto lineEndLocation = CommonTypes::getVector3(in);
    return PrimitiveShapeLinePayload{lineEndLocation};
}

void PrimitiveShapeLinePayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putVector3(out, lineEndLocation);
}

}  // namespace bedrock_protocol::types::shape
