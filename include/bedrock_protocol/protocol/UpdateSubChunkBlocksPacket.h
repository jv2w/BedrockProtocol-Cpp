/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateSubChunkBlocksPacket.php
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
#include "bedrock_protocol/protocol/types/UpdateSubChunkBlocksPacketEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class UpdateSubChunkBlocksPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UPDATE_SUB_CHUNK_BLOCKS_PACKET;


    types::BlockPosition baseBlockPosition;
    /** @var UpdateSubChunkBlocksPacketEntry[] */
    std::vector<types::UpdateSubChunkBlocksPacketEntry> layer0Updates;
    /** @var UpdateSubChunkBlocksPacketEntry[] */
    std::vector<types::UpdateSubChunkBlocksPacketEntry> layer1Updates;

    /**
     * @generate-create-func
     */
    static UpdateSubChunkBlocksPacket create(types::BlockPosition baseBlockPosition, std::vector<types::UpdateSubChunkBlocksPacketEntry> layer0Updates, std::vector<types::UpdateSubChunkBlocksPacketEntry> layer1Updates);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UpdateSubChunkBlocksPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
