/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketHeightMapInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPacketHeightMapInfo.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/Byte.h"

namespace bedrock_protocol::types {

using encoding::Byte;

SubChunkPacketHeightMapInfo::SubChunkPacketHeightMapInfo(std::vector<std::int8_t> heights) : heights(std::move(heights))
{
    if (this->heights.size() != 256) {
        throw std::invalid_argument("Expected exactly 256 heightmap values");
    }
}

SubChunkPacketHeightMapInfo SubChunkPacketHeightMapInfo::read(encoding::ByteBufferReader &in)
{
    std::vector<std::int8_t> heights;
    for (int i = 0; i < 256; ++i) {
        heights.push_back(Byte::readSigned(in));
    }
    return SubChunkPacketHeightMapInfo(std::move(heights));
}

void SubChunkPacketHeightMapInfo::write(encoding::ByteBufferWriter &out) const
{
    for (int i = 0; i < 256; ++i) {
        Byte::writeSigned(out, heights[static_cast<std::size_t>(i)]);
    }
}

SubChunkPacketHeightMapInfo SubChunkPacketHeightMapInfo::allTooLow()
{
    return SubChunkPacketHeightMapInfo(std::vector<std::int8_t>(256, -1));
}

SubChunkPacketHeightMapInfo SubChunkPacketHeightMapInfo::allTooHigh()
{
    return SubChunkPacketHeightMapInfo(std::vector<std::int8_t>(256, 16));
}

bool SubChunkPacketHeightMapInfo::isAllTooLow() const
{
    for (const auto &height : heights) {
        if (height >= 0) {
            return false;
        }
    }
    return true;
}

bool SubChunkPacketHeightMapInfo::isAllTooHigh() const
{
    for (const auto &height : heights) {
        if (height <= 15) {
            return false;
        }
    }
    return true;
}

}  // namespace bedrock_protocol::types
