/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PacketViolationWarningPacket.php
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

class PacketViolationWarningPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PACKET_VIOLATION_WARNING_PACKET;

    static constexpr std::int32_t TYPE_MALFORMED = 0;
    static constexpr std::int32_t SEVERITY_WARNING = 0;
    static constexpr std::int32_t SEVERITY_FINAL_WARNING = 1;
    static constexpr std::int32_t SEVERITY_TERMINATING_CONNECTION = 2;

    std::int32_t type = 0;
    std::int32_t severity = 0;
    std::int32_t packetId = 0;
    std::string message;

    /**
     * @generate-create-func
     */
    static PacketViolationWarningPacket create(std::int32_t type, std::int32_t severity, std::int32_t packetId, std::string message);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PacketViolationWarningPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
