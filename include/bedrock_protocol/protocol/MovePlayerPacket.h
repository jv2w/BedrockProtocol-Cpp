/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MovePlayerPacket.php
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

class MovePlayerPacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MOVE_PLAYER_PACKET;

    static constexpr std::int32_t MODE_NORMAL = 0;
    static constexpr std::int32_t MODE_RESET = 1;
    static constexpr std::uint8_t MODE_TELEPORT = 2;
    static constexpr std::int32_t MODE_PITCH = 3;  //facepalm Mojang

    std::uint64_t actorRuntimeId = 0;
    math::Vector3 position;
    float pitch = 0.0F;
    float yaw = 0.0F;
    float headYaw = 0.0F;
    std::uint8_t mode = MODE_NORMAL;
    bool onGround = false;  //TODO
    std::uint64_t ridingActorRuntimeId = 0;
    std::int32_t teleportCause = 0;
    std::int32_t teleportItem = 0;
    std::uint64_t tick = 0;

    /**
     * @generate-create-func
     */
    static MovePlayerPacket create(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t mode, bool onGround, std::uint64_t ridingActorRuntimeId, std::int32_t teleportCause, std::int32_t teleportItem, std::uint64_t tick);

    static MovePlayerPacket simple(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t mode, bool onGround, std::uint64_t ridingActorRuntimeId, std::uint64_t tick);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MovePlayerPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
