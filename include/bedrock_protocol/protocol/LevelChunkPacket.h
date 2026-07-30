/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LevelChunkPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/ChunkPosition.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class LevelChunkPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::LEVEL_CHUNK_PACKET;


    types::ChunkPosition chunkPosition{0, 0};
    /** @phpstan-var DimensionIds::* */
    std::int32_t dimensionId = 0;
    std::int64_t subChunkCount = 0;
    bool clientSubChunkRequestsEnabled = false;
    /** @var int[]|null */
    std::optional<std::vector<std::uint64_t>> usedBlobHashes = std::nullopt;
    std::string extraPayload;

    /**
     * @generate-create-func
     */
    static LevelChunkPacket create(types::ChunkPosition chunkPosition, std::int32_t dimensionId, std::int64_t subChunkCount, bool clientSubChunkRequestsEnabled, std::optional<std::vector<std::uint64_t>> usedBlobHashes, std::string extraPayload);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "LevelChunkPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    /**
     * Client will request all subchunks as needed up to the top of the world
     */
    static constexpr std::uint32_t CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT = 0xFFFFFFFFU;
    /**
     * Client will request subchunks as needed up to the height written in the packet, and assume that anything above
     * that height is air (wtf mojang ...)
     */
    static constexpr std::uint32_t CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT = 0xFFFFFFFFU - 1;
    //this appears large enough for a world height of 1024 blocks - it may need to be increased in the future
    static constexpr std::uint32_t MAX_BLOB_HASHES = 64;
};

}  // namespace bedrock_protocol
