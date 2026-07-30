/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryCommon.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SubChunkPacketEntryCommon.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/SubChunkPacketHeightMapType.h"
#include "bedrock_protocol/protocol/types/SubChunkRequestResult.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using serializer::CommonTypes;

SubChunkPacketEntryCommon SubChunkPacketEntryCommon::read(encoding::ByteBufferReader &in, bool cacheEnabled)
{
    auto offset = SubChunkPositionOffset::read(in);

    const auto requestResult = Byte::readUnsigned(in);

    auto data = !cacheEnabled || requestResult != SubChunkRequestResult::SUCCESS_ALL_AIR ? CommonTypes::getString(in) : std::string();

    const auto heightMapDataType = Byte::readUnsigned(in);
    std::optional<SubChunkPacketHeightMapInfo> heightMapData;
    switch (heightMapDataType) {
    case SubChunkPacketHeightMapType::NO_DATA:
        heightMapData = std::nullopt;
        break;
    case SubChunkPacketHeightMapType::DATA:
        heightMapData = SubChunkPacketHeightMapInfo::read(in);
        break;
    case SubChunkPacketHeightMapType::ALL_TOO_HIGH:
        heightMapData = SubChunkPacketHeightMapInfo::allTooHigh();
        break;
    case SubChunkPacketHeightMapType::ALL_TOO_LOW:
        heightMapData = SubChunkPacketHeightMapInfo::allTooLow();
        break;
    default:
        throw PacketDecodeException("Unknown heightmap data type " + std::to_string(heightMapDataType));
    }

    const auto renderHeightMapDataType = Byte::readUnsigned(in);
    std::optional<SubChunkPacketHeightMapInfo> renderHeightMapData;
    switch (renderHeightMapDataType) {
    case SubChunkPacketHeightMapType::NO_DATA:
        renderHeightMapData = std::nullopt;
        break;
    case SubChunkPacketHeightMapType::DATA:
        renderHeightMapData = SubChunkPacketHeightMapInfo::read(in);
        break;
    case SubChunkPacketHeightMapType::ALL_TOO_HIGH:
        renderHeightMapData = SubChunkPacketHeightMapInfo::allTooHigh();
        break;
    case SubChunkPacketHeightMapType::ALL_TOO_LOW:
        renderHeightMapData = SubChunkPacketHeightMapInfo::allTooLow();
        break;
    case SubChunkPacketHeightMapType::ALL_COPIED:
        renderHeightMapData = heightMapData;
        break;
    default:
        throw PacketDecodeException("Unknown render heightmap data type " + std::to_string(renderHeightMapDataType));
    }

    return SubChunkPacketEntryCommon(
        offset,
        requestResult,
        std::move(data),
        std::move(heightMapData),
        std::move(renderHeightMapData)
    );
}

void SubChunkPacketEntryCommon::write(encoding::ByteBufferWriter &out, bool cacheEnabled) const
{
    offset.write(out);

    Byte::writeUnsigned(out, requestResult);

    if (!cacheEnabled || requestResult != SubChunkRequestResult::SUCCESS_ALL_AIR) {
        CommonTypes::putString(out, terrainData);
    }

    if (!heightMap.has_value()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::NO_DATA);
    } else if (heightMap->isAllTooLow()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::ALL_TOO_LOW);
    } else if (heightMap->isAllTooHigh()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::ALL_TOO_HIGH);
    } else {
        const auto &heightMapData = *heightMap; //avoid PHPStan purity issue
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::DATA);
        heightMapData.write(out);
    }

    if (!renderHeightMap.has_value()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::ALL_COPIED);
    } else if (renderHeightMap->isAllTooLow()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::ALL_TOO_LOW);
    } else if (renderHeightMap->isAllTooHigh()) {
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::ALL_TOO_HIGH);
    } else {
        const auto &renderHeightMapData = *renderHeightMap; //avoid PHPStan purity issue
        Byte::writeUnsigned(out, SubChunkPacketHeightMapType::DATA);
        renderHeightMapData.write(out);
    }
}

}  // namespace bedrock_protocol::types
