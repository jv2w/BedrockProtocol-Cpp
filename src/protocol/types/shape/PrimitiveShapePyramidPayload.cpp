/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapePyramidPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePyramidPayload.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using encoding::LE;
using serializer::CommonTypes;

PrimitiveShapePyramidPayload PrimitiveShapePyramidPayload::read(encoding::ByteBufferReader &in)
{
    const auto width = LE::readFloat(in);
    const auto depth = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    const auto height = LE::readFloat(in);

    return PrimitiveShapePyramidPayload{width, depth, height};
}

void PrimitiveShapePyramidPayload::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, width);
    CommonTypes::writeOptional(out, depth, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    LE::writeFloat(out, height);
}

}  // namespace bedrock_protocol::types::shape
