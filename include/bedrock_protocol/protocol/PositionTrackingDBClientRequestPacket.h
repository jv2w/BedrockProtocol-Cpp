/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PositionTrackingDBClientRequestPacket.php
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

namespace bedrock_protocol {

class PacketHandlerInterface;

class PositionTrackingDBClientRequestPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::POSITION_TRACKING_DB_CLIENT_REQUEST_PACKET;

    static constexpr std::int32_t ACTION_QUERY = 0;

    std::uint8_t action = 0;
    std::int32_t trackingId = 0;

    /**
     * @generate-create-func
     */
    static PositionTrackingDBClientRequestPacket create(std::uint8_t action, std::int32_t trackingId);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PositionTrackingDBClientRequestPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
