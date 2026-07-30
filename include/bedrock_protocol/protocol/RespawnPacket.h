/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RespawnPacket.php
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
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class RespawnPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::RESPAWN_PACKET;

    static constexpr std::int32_t SEARCHING_FOR_SPAWN = 0;
    static constexpr std::int32_t READY_TO_SPAWN = 1;
    static constexpr std::int32_t CLIENT_READY_TO_SPAWN = 2;

    math::Vector3 position;
    std::uint8_t respawnState = SEARCHING_FOR_SPAWN;
    std::uint64_t actorRuntimeId = 0;

    /**
     * @generate-create-func
     */
    static RespawnPacket create(math::Vector3 position, std::uint8_t respawnState, std::uint64_t actorRuntimeId);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "RespawnPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
