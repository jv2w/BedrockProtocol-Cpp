/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeEllipsoidPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeEllipsoidPayload.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using encoding::Byte;
using serializer::CommonTypes;

PrimitiveShapeEllipsoidPayload PrimitiveShapeEllipsoidPayload::read(encoding::ByteBufferReader &in)
{
    const auto radii = CommonTypes::getVector3(in);
    const auto segmentsPerAxis = Byte::readUnsigned(in);

    return PrimitiveShapeEllipsoidPayload{radii, segmentsPerAxis};
}

void PrimitiveShapeEllipsoidPayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putVector3(out, radii);
    Byte::writeUnsigned(out, segmentsPerAxis);
}

}  // namespace bedrock_protocol::types::shape
