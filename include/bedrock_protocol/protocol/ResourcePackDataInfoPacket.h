/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ResourcePackDataInfoPacket.php
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
#include "bedrock_protocol/protocol/types/resourcepacks/ResourcePackType.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ResourcePackDataInfoPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::RESOURCE_PACK_DATA_INFO_PACKET;


    std::string packId;
    std::uint32_t maxChunkSize = 0;
    std::uint32_t chunkCount = 0;
    std::uint64_t compressedPackSize = 0;
    std::string sha256;
    bool isPremium = false;
    std::uint8_t packType = types::resourcepacks::ResourcePackType::RESOURCES;  //TODO: check the values for this

    /**
     * @generate-create-func
     */
    static ResourcePackDataInfoPacket create(std::string packId, std::uint32_t maxChunkSize, std::uint32_t chunkCount, std::uint64_t compressedPackSize, std::string sha256, bool isPremium, std::uint8_t packType);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ResourcePackDataInfoPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
