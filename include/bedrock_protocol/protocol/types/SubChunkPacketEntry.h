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

/**
 * A single sub-chunk entry.
 *
 * gophertunnel v1.58.0 minecraft/protocol/sub_chunk.go:23-56 has ONE entry shape: the cached and
 * uncached forms were merged, and every variable part now sits behind its own presence byte instead
 * of being implied by the result code or the height map type.
 */
class SubChunkPacketEntry final {
public:
    SubChunkPacketEntry(SubChunkPositionOffset offset, std::uint8_t requestResult, std::optional<std::string> terrainData, std::uint8_t heightMapType, std::optional<SubChunkPacketHeightMapInfo> heightMap, std::uint8_t renderHeightMapType, std::optional<SubChunkPacketHeightMapInfo> renderHeightMap, std::optional<std::uint64_t> usedBlobHash)
        : offset(offset), requestResult(requestResult), terrainData(std::move(terrainData)), heightMapType(heightMapType), heightMap(std::move(heightMap)), renderHeightMapType(renderHeightMapType), renderHeightMap(std::move(renderHeightMap)), usedBlobHash(usedBlobHash)
    {
    }

    [[nodiscard]] const SubChunkPositionOffset &getOffset() const { return offset; }

    [[nodiscard]] std::uint8_t getRequestResult() const { return requestResult; }

    [[nodiscard]] const std::optional<std::string> &getTerrainData() const { return terrainData; }

    [[nodiscard]] std::uint8_t getHeightMapType() const { return heightMapType; }

    [[nodiscard]] const std::optional<SubChunkPacketHeightMapInfo> &getHeightMap() const { return heightMap; }

    [[nodiscard]] std::uint8_t getRenderHeightMapType() const { return renderHeightMapType; }

    [[nodiscard]] const std::optional<SubChunkPacketHeightMapInfo> &getRenderHeightMap() const { return renderHeightMap; }

    [[nodiscard]] const std::optional<std::uint64_t> &getUsedBlobHash() const { return usedBlobHash; }

    static SubChunkPacketEntry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    SubChunkPositionOffset offset;
    std::uint8_t requestResult;
    std::optional<std::string> terrainData;
    std::uint8_t heightMapType;
    std::optional<SubChunkPacketHeightMapInfo> heightMap;
    std::uint8_t renderHeightMapType;
    std::optional<SubChunkPacketHeightMapInfo> renderHeightMap;
    std::optional<std::uint64_t> usedBlobHash;
};

}  // namespace bedrock_protocol::types
