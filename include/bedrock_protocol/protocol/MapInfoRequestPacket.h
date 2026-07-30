/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MapInfoRequestPacket.php
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
#include "bedrock_protocol/protocol/types/MapImage.h"
#include "bedrock_protocol/protocol/types/MapInfoRequestPacketClientPixel.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class MapInfoRequestPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MAP_INFO_REQUEST_PACKET;


    std::int64_t mapId = 0;
    /** @var MapInfoRequestPacketClientPixel[] */
    std::vector<types::MapInfoRequestPacketClientPixel> clientPixels;

    /**
     * @generate-create-func
     */
    static MapInfoRequestPacket create(std::int64_t mapId, std::vector<types::MapInfoRequestPacketClientPixel> clientPixels);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MapInfoRequestPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
