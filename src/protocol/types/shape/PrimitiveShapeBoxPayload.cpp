/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeBoxPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeBoxPayload.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using serializer::CommonTypes;

PrimitiveShapeBoxPayload PrimitiveShapeBoxPayload::read(encoding::ByteBufferReader &in)
{
    const auto boxBound = CommonTypes::getVector3(in);
    return PrimitiveShapeBoxPayload{boxBound};
}

void PrimitiveShapeBoxPayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putVector3(out, boxBound);
}

}  // namespace bedrock_protocol::types::shape
