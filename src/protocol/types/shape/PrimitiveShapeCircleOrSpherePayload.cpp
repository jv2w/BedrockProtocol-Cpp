/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeCircleOrSpherePayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeCircleOrSpherePayload.h"

#include "bedrock_protocol/encoding/Byte.h"

namespace bedrock_protocol::types::shape {

using encoding::Byte;

PrimitiveShapeCircleOrSpherePayload PrimitiveShapeCircleOrSpherePayload::read(encoding::ByteBufferReader &in)
{
    const auto segments = Byte::readUnsigned(in);
    return PrimitiveShapeCircleOrSpherePayload{segments};
}

void PrimitiveShapeCircleOrSpherePayload::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, segments);
}

}  // namespace bedrock_protocol::types::shape
