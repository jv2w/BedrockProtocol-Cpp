/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NetworkChunkPublisherUpdatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/ChunkPosition.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class NetworkChunkPublisherUpdatePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::NETWORK_CHUNK_PUBLISHER_UPDATE_PACKET;

    static constexpr std::uint32_t MAX_SAVED_CHUNKS = 9216;

    types::BlockPosition blockPosition;
    std::uint32_t radius = 0;
    /** @var ChunkPosition[] */
    std::vector<types::ChunkPosition> savedChunks;

    /**
     * @generate-create-func
     */
    static NetworkChunkPublisherUpdatePacket create(types::BlockPosition blockPosition, std::uint32_t radius, std::vector<types::ChunkPosition> savedChunks);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "NetworkChunkPublisherUpdatePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
