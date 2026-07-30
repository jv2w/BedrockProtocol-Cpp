/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CreativeContentPacket.php
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
#include "bedrock_protocol/protocol/types/inventory/CreativeGroupEntry.h"
#include "bedrock_protocol/protocol/types/inventory/CreativeItemEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CreativeContentPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CREATIVE_CONTENT_PACKET;

    static constexpr std::int32_t CATEGORY_CONSTRUCTION = 1;
    static constexpr std::int32_t CATEGORY_NATURE = 2;
    static constexpr std::int32_t CATEGORY_EQUIPMENT = 3;
    static constexpr std::int32_t CATEGORY_ITEMS = 4;

    /** @var CreativeGroupEntry[] */
    std::vector<types::inventory::CreativeGroupEntry> groups;
    /** @var CreativeItemEntry[] */
    std::vector<types::inventory::CreativeItemEntry> items;

    /**
     * @generate-create-func
     */
    static CreativeContentPacket create(std::vector<types::inventory::CreativeGroupEntry> groups, std::vector<types::inventory::CreativeItemEntry> items);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CreativeContentPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
