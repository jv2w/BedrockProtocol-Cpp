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

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/DimensionIds.h"
#include "bedrock_protocol/protocol/types/MapDecoration.h"
#include "bedrock_protocol/protocol/types/MapTrackedObject.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ClientboundMapItemDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CLIENTBOUND_MAP_ITEM_DATA_PACKET;

    std::int64_t mapId = 0;
    std::uint8_t dimensionId = types::DimensionIds::OVERWORLD;
    bool isLocked = false;
    types::BlockPosition origin;
    /**
     * Every field from here down is an independent optional with its own presence byte. The
     * UpdateFlags bitmask that used to gate them is gone.
     * gophertunnel v1.58.0 minecraft/protocol/packet/client_bound_map_item_data.go:61-83.
     */
    std::optional<std::vector<std::int64_t>> parentMapIds = std::nullopt;
    std::optional<std::uint8_t> scale = std::nullopt;
    std::optional<std::vector<types::MapTrackedObject>> trackedEntities = std::nullopt;
    std::optional<std::vector<types::MapDecoration>> decorations = std::nullopt;
    std::optional<std::int32_t> width = std::nullopt;
    std::optional<std::int32_t> height = std::nullopt;
    std::optional<std::int32_t> xOffset = std::nullopt;
    std::optional<std::int32_t> yOffset = std::nullopt;
    /** A flat, self-counting pixel list - its length is no longer tied to width * height. */
    std::optional<std::vector<color::Color>> colors = std::nullopt;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ClientboundMapItemDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
