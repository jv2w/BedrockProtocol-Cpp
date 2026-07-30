/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPositionOffset.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPositionOffset.h"

#include <limits>
#include <stdexcept>
#include <string>

#include "bedrock_protocol/encoding/Byte.h"

namespace bedrock_protocol::types {

using encoding::Byte;

namespace {
constexpr std::int32_t INT8_MIN_VALUE = std::numeric_limits<std::int8_t>::min();
constexpr std::int32_t INT8_MAX_VALUE = std::numeric_limits<std::int8_t>::max();
}  // namespace

SubChunkPositionOffset::SubChunkPositionOffset(std::int32_t xOffset, std::int32_t yOffset, std::int32_t zOffset)
{
    clampOffset(xOffset);
    clampOffset(yOffset);
    clampOffset(zOffset);

    this->xOffset = static_cast<std::int8_t>(xOffset);
    this->yOffset = static_cast<std::int8_t>(yOffset);
    this->zOffset = static_cast<std::int8_t>(zOffset);
}

void SubChunkPositionOffset::clampOffset(std::int32_t v)
{
    if (v < INT8_MIN_VALUE || v > INT8_MAX_VALUE) {
        throw std::invalid_argument("Offsets must be within the range of a byte (" + std::to_string(INT8_MIN_VALUE) + " ... " + std::to_string(INT8_MAX_VALUE) + ")");
    }
}

SubChunkPositionOffset SubChunkPositionOffset::read(encoding::ByteBufferReader &in)
{
    const auto xOffset = Byte::readSigned(in);
    const auto yOffset = Byte::readSigned(in);
    const auto zOffset = Byte::readSigned(in);

    return SubChunkPositionOffset(xOffset, yOffset, zOffset);
}

void SubChunkPositionOffset::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeSigned(out, xOffset);
    Byte::writeSigned(out, yOffset);
    Byte::writeSigned(out, zOffset);
}

}  // namespace bedrock_protocol::types
