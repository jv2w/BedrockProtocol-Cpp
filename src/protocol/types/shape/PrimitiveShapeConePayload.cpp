/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeConePayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeConePayload.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

PrimitiveShapeConePayload PrimitiveShapeConePayload::read(encoding::ByteBufferReader &in)
{
    const auto radii = CommonTypes::getVector2(in);
    const auto height = LE::readFloat(in);
    const auto segments = Byte::readUnsigned(in);

    return PrimitiveShapeConePayload{radii, height, segments};
}

void PrimitiveShapeConePayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putVector2(out, radii);
    LE::writeFloat(out, height);
    Byte::writeUnsigned(out, segments);
}

}  // namespace bedrock_protocol::types::shape
