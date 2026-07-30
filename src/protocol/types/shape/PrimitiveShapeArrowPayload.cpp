/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeArrowPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeArrowPayload.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

PrimitiveShapeArrowPayload PrimitiveShapeArrowPayload::read(encoding::ByteBufferReader &in)
{
    auto lineEndLocation = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CommonTypes::getVector3(in);
    });
    auto arrowHeadLength = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    auto arrowHeadRadius = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    auto segments = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return Byte::readUnsigned(in);
    });

    return PrimitiveShapeArrowPayload{
        lineEndLocation,
        arrowHeadLength,
        arrowHeadRadius,
        segments,
    };
}

void PrimitiveShapeArrowPayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::writeOptional(out, lineEndLocation, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, arrowHeadLength, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    CommonTypes::writeOptional(out, arrowHeadRadius, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    CommonTypes::writeOptional(out, segments, [](encoding::ByteBufferWriter &out, const std::uint8_t v) {
        Byte::writeUnsigned(out, v);
    });
}

}  // namespace bedrock_protocol::types::shape
