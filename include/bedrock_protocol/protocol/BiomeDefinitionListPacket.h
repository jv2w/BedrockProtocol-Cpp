/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BiomeDefinitionListPacket.php
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
#include "bedrock_protocol/protocol/types/biome/BiomeDefinitionData.h"
#include "bedrock_protocol/protocol/types/biome/BiomeDefinitionEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class BiomeDefinitionListPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::BIOME_DEFINITION_LIST_PACKET;


    /**
     */
    std::vector<types::biome::BiomeDefinitionData> definitionData;
    /**
     */
    std::vector<std::string> strings;

    /**
     * @generate-create-func
     */
    static BiomeDefinitionListPacket create(std::vector<types::biome::BiomeDefinitionData> definitionData, std::vector<std::string> strings);

    /**
     * Builds the packet from high-level entries, interning every biome name and tag into the shared
     * string table the wire format uses.
     */
    static BiomeDefinitionListPacket fromDefinitions(const std::vector<types::biome::BiomeDefinitionEntry> &definitions);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "BiomeDefinitionListPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
