/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryCommon.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPacketEntry.h"

#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

SubChunkPacketEntry SubChunkPacketEntry::read(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/sub_chunk.go:43-56.
    auto offset = SubChunkPositionOffset::read(in);

    const auto requestResult = Byte::readUnsigned(in);

    auto terrainData = CommonTypes::getBool(in) ? std::optional(CommonTypes::getString(in)) : std::nullopt;

    const auto heightMapType = Byte::readUnsigned(in);
    auto heightMap = CommonTypes::getBool(in) ? std::optional(SubChunkPacketHeightMapInfo::read(in)) : std::nullopt;

    const auto renderHeightMapType = Byte::readUnsigned(in);
    auto renderHeightMap = CommonTypes::getBool(in) ? std::optional(SubChunkPacketHeightMapInfo::read(in)) : std::nullopt;

    auto usedBlobHash = CommonTypes::getBool(in) ? std::optional(LE::readUnsignedLong(in)) : std::nullopt;

    return SubChunkPacketEntry(
        offset,
        requestResult,
        std::move(terrainData),
        heightMapType,
        std::move(heightMap),
        renderHeightMapType,
        std::move(renderHeightMap),
        usedBlobHash
    );
}

void SubChunkPacketEntry::write(encoding::ByteBufferWriter &out) const
{
    offset.write(out);

    Byte::writeUnsigned(out, requestResult);

    CommonTypes::putBool(out, terrainData.has_value());
    if (terrainData.has_value()) {
        CommonTypes::putString(out, *terrainData);
    }

    Byte::writeUnsigned(out, heightMapType);
    CommonTypes::putBool(out, heightMap.has_value());
    if (heightMap.has_value()) {
        heightMap->write(out);
    }

    Byte::writeUnsigned(out, renderHeightMapType);
    CommonTypes::putBool(out, renderHeightMap.has_value());
    if (renderHeightMap.has_value()) {
        renderHeightMap->write(out);
    }

    CommonTypes::putBool(out, usedBlobHash.has_value());
    if (usedBlobHash.has_value()) {
        LE::writeUnsignedLong(out, *usedBlobHash);
    }
}

}  // namespace bedrock_protocol::types
