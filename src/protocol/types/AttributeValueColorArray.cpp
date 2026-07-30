/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeValueColorArray.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeValueColorArray.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::LE;

AttributeValueColorArray AttributeValueColorArray::read(encoding::ByteBufferReader &in)
{
    const auto r = LE::readUnsignedInt(in);
    const auto g = LE::readUnsignedInt(in);
    const auto b = LE::readUnsignedInt(in);
    const auto a = LE::readUnsignedInt(in);

    return AttributeValueColorArray(color::Color(static_cast<std::uint8_t>(r), static_cast<std::uint8_t>(g),
                                                 static_cast<std::uint8_t>(b), static_cast<std::uint8_t>(a)));
}

void AttributeValueColorArray::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, value.getR());
    LE::writeUnsignedInt(out, value.getG());
    LE::writeUnsignedInt(out, value.getB());
    LE::writeUnsignedInt(out, value.getA());
}

}  // namespace bedrock_protocol::types
