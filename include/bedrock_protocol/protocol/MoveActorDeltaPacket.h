/*
 * This file is part of BedrockProtocol-Cpp.
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

    // The packed flags word is gone as of 1.26.40: every coordinate and rotation carries its own
    // presence bool, and the former FLAG_GROUND/FLAG_TELEPORT/FLAG_FORCE_MOVE_LOCAL_ENTITY bits are
    // trailing bools (move_actor_delta.go:40-52).
    std::uint64_t actorRuntimeId = 0;
    std::optional<float> xPos = std::nullopt;
    std::optional<float> yPos = std::nullopt;
    std::optional<float> zPos = std::nullopt;
    std::optional<float> pitch = std::nullopt;
    std::optional<float> yaw = std::nullopt;
    std::optional<float> headYaw = std::nullopt;
    bool onGround = false;
    bool forceMove = false;
    bool forceMoveLocalEntity = false;
    bool forceCompletion = false;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MoveActorDeltaPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
