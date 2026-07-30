/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MoveActorAbsolutePacket.php
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

class MoveActorAbsolutePacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MOVE_ACTOR_ABSOLUTE_PACKET;

    static constexpr std::int32_t FLAG_GROUND = 0x01;
    static constexpr std::int32_t FLAG_TELEPORT = 0x02;
    static constexpr std::int32_t FLAG_FORCE_MOVE_LOCAL_ENTITY = 0x04;
    static constexpr std::int32_t FLAG_FORCE_COMPLETION = 0x8;

    std::uint64_t actorRuntimeId = 0;
    math::Vector3 position;
    float pitch = 0.0F;
    float yaw = 0.0F;
    float headYaw = 0.0F;  //always zero for non-mobs
    std::uint8_t flags = 0;

    /**
     * @generate-create-func
     */
    static MoveActorAbsolutePacket create(std::uint64_t actorRuntimeId, math::Vector3 position, float pitch, float yaw, float headYaw, std::uint8_t flags);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MoveActorAbsolutePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
