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
    std::uint32_t subChunkCount = 0;
    /** Maximum number of sub-chunks a client in request mode will ask for; -1 means no limit. */
    std::optional<std::int32_t> subChunkLimit = std::nullopt;
    bool cacheEnabled = false;
    /** @var int[] */
    std::vector<std::uint64_t> usedBlobHashes;
    std::string extraPayload;

    /**
     * @generate-create-func
     */
    static LevelChunkPacket create(types::ChunkPosition chunkPosition, std::int32_t dimensionId, std::uint32_t subChunkCount, std::optional<std::int32_t> subChunkLimit, bool cacheEnabled, std::vector<std::uint64_t> usedBlobHashes, std::string extraPayload);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "LevelChunkPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    //gophertunnel v1.58.0 minecraft/protocol/packet/level_chunk.go:47-49 rejects anything above this.
    static constexpr std::uint32_t MAX_SUB_CHUNK_COUNT = 64;
};

}  // namespace bedrock_protocol
