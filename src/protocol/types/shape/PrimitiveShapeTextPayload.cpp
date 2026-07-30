/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeTextPayload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeTextPayload.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::shape {

using encoding::LE;
using serializer::CommonTypes;

PrimitiveShapeTextPayload PrimitiveShapeTextPayload::read(encoding::ByteBufferReader &in)
{
    auto text = CommonTypes::getString(in);
    const auto useRotation = CommonTypes::getBool(in);
    const auto backgroundColor = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return color::Color::fromARGB(LE::readUnsignedInt(in));
    });
    const auto depthTest = CommonTypes::getBool(in);
    const auto showBackface = CommonTypes::getBool(in);
    const auto showTextBackface = CommonTypes::getBool(in);

    return PrimitiveShapeTextPayload{std::move(text), useRotation,  backgroundColor,
                                     depthTest,       showBackface, showTextBackface};
}

void PrimitiveShapeTextPayload::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, text);
    CommonTypes::putBool(out, useRotation_);
    CommonTypes::writeOptional(out, backgroundColor, [](encoding::ByteBufferWriter &out, const color::Color &color) {
        LE::writeUnsignedInt(out, color.toARGB());
    });
    CommonTypes::putBool(out, depthTest);
    CommonTypes::putBool(out, showBackface);
    CommonTypes::putBool(out, showTextBackface);
}

}  // namespace bedrock_protocol::types::shape
