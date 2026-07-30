/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AnimatePacket.php
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

class AnimatePacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::ANIMATE_PACKET;

    static constexpr std::int32_t ACTION_SWING_ARM = 1;
    static constexpr std::int32_t ACTION_STOP_SLEEP = 3;
    static constexpr std::int32_t ACTION_CRITICAL_HIT = 4;
    static constexpr std::int32_t ACTION_MAGICAL_CRITICAL_HIT = 5;

    std::uint8_t action = 0;
    std::uint64_t actorRuntimeId = 0;
    float data = 0.0;
    std::optional<std::string> swingSource = std::nullopt;

    static AnimatePacket create(std::uint64_t actorRuntimeId, std::uint8_t action, float data, std::optional<std::string> swingSource);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AnimatePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
