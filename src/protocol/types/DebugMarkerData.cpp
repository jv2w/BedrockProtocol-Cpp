/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/DebugMarkerData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/DebugMarkerData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using color::Color;
using encoding::LE;
using serializer::CommonTypes;

DebugMarkerData DebugMarkerData::read(encoding::ByteBufferReader &in)
{
    auto text = CommonTypes::getString(in);
    const auto position = CommonTypes::getVector3(in);
    const auto color = Color::fromARGB(LE::readUnsignedInt(in));
    const auto durationMillis = LE::readUnsignedLong(in);

    return DebugMarkerData(std::move(text), position, color, durationMillis);
}

void DebugMarkerData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, text);
    CommonTypes::putVector3(out, position);
    LE::writeUnsignedInt(out, color.toARGB());
    LE::writeUnsignedLong(out, durationMillis);
}

}  // namespace bedrock_protocol::types
