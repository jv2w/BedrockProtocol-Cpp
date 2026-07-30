/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MoveActorDeltaPacket.php
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

class MoveActorDeltaPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MOVE_ACTOR_DELTA_PACKET;

    static constexpr std::int32_t FLAG_HAS_X = 0x01;
    static constexpr std::int32_t FLAG_HAS_Y = 0x02;
    static constexpr std::int32_t FLAG_HAS_Z = 0x04;
    static constexpr std::int32_t FLAG_HAS_PITCH = 0x08;
    static constexpr std::int32_t FLAG_HAS_YAW = 0x10;
    static constexpr std::int32_t FLAG_HAS_HEAD_YAW = 0x20;
    static constexpr std::int32_t FLAG_GROUND = 0x40;
    static constexpr std::int32_t FLAG_TELEPORT = 0x80;
    static constexpr std::int32_t FLAG_FORCE_MOVE_LOCAL_ENTITY = 0x100;

    std::uint64_t actorRuntimeId = 0;
    std::uint16_t flags = 0;
    float xPos = 0;
    float yPos = 0;
    float zPos = 0;
    float pitch = 0.0;
    float yaw = 0.0;
    float headYaw = 0.0;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MoveActorDeltaPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    /** @throws encoding::DataDecodeException */
    [[nodiscard]] float maybeReadCoord(std::int32_t flag, encoding::ByteBufferReader &in) const;

    /** @throws encoding::DataDecodeException */
    [[nodiscard]] float maybeReadRotation(std::int32_t flag, encoding::ByteBufferReader &in) const;

    void maybeWriteCoord(std::int32_t flag, float val, encoding::ByteBufferWriter &out) const;

    void maybeWriteRotation(std::int32_t flag, float val, encoding::ByteBufferWriter &out) const;
};

}  // namespace bedrock_protocol
