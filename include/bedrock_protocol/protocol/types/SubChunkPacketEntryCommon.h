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

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/SubChunkPacketHeightMapInfo.h"
#include "bedrock_protocol/protocol/types/SubChunkPositionOffset.h"

namespace bedrock_protocol::types {

class SubChunkPacketEntryCommon final {
public:
    SubChunkPacketEntryCommon(SubChunkPositionOffset offset, std::uint8_t requestResult, std::string terrainData, std::optional<SubChunkPacketHeightMapInfo> heightMap, std::optional<SubChunkPacketHeightMapInfo> renderHeightMap)
        : offset(offset), requestResult(requestResult), terrainData(std::move(terrainData)), heightMap(std::move(heightMap)), renderHeightMap(std::move(renderHeightMap))
    {
    }

    [[nodiscard]] const SubChunkPositionOffset &getOffset() const { return offset; }

    [[nodiscard]] std::uint8_t getRequestResult() const { return requestResult; }

    [[nodiscard]] const std::string &getTerrainData() const { return terrainData; }

    [[nodiscard]] const std::optional<SubChunkPacketHeightMapInfo> &getHeightMap() const { return heightMap; }

    [[nodiscard]] const std::optional<SubChunkPacketHeightMapInfo> &getRenderHeightMap() const { return renderHeightMap; }

    static SubChunkPacketEntryCommon read(encoding::ByteBufferReader &in, bool cacheEnabled);

    void write(encoding::ByteBufferWriter &out, bool cacheEnabled) const;

private:
    SubChunkPositionOffset offset;
    std::uint8_t requestResult;
    std::string terrainData;
    std::optional<SubChunkPacketHeightMapInfo> heightMap;
    std::optional<SubChunkPacketHeightMapInfo> renderHeightMap;
};

}  // namespace bedrock_protocol::types
