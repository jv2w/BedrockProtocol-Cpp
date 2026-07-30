/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundMapItemDataPacket.php
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
#include "bedrock_protocol/protocol/types/DimensionIds.h"
#include "bedrock_protocol/protocol/types/MapDecoration.h"
#include "bedrock_protocol/protocol/types/MapImage.h"
#include "bedrock_protocol/protocol/types/MapTrackedObject.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientboundMapItemDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENTBOUND_MAP_ITEM_DATA_PACKET;

    static constexpr std::uint32_t BITFLAG_TEXTURE_UPDATE = 0x02;
    static constexpr std::uint32_t BITFLAG_DECORATION_UPDATE = 0x04;
    static constexpr std::uint32_t BITFLAG_MAP_CREATION = 0x08;

    std::int64_t mapId = 0;
    std::uint32_t type = 0;
    std::uint8_t dimensionId = types::DimensionIds::OVERWORLD;
    bool isLocked = false;
    types::BlockPosition origin;
    /** @var int[] */
    std::vector<std::int64_t> parentMapIds;
    std::uint8_t scale = 0;
    /** @var MapTrackedObject[] */
    std::vector<types::MapTrackedObject> trackedEntities;
    /** @var MapDecoration[] */
    std::vector<types::MapDecoration> decorations;
    std::int32_t xOffset = 0;
    std::int32_t yOffset = 0;
    std::optional<types::MapImage> colors = std::nullopt;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientboundMapItemDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
