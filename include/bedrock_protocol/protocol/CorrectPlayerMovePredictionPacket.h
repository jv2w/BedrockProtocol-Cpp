/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CorrectPlayerMovePredictionPacket.php
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
#include "bedrock_protocol/math/Vector2.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CorrectPlayerMovePredictionPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CORRECT_PLAYER_MOVE_PREDICTION_PACKET;

    static constexpr std::int32_t PREDICTION_TYPE_PLAYER = 0;
    static constexpr std::int32_t PREDICTION_TYPE_VEHICLE = 1;

    math::Vector3 position;
    math::Vector3 delta;
    bool onGround = false;
    std::uint64_t tick = 0;
    std::uint8_t predictionType = 0;
    math::Vector2 vehicleRotation;
    std::optional<float> vehicleAngularVelocity;

    /**
     * @generate-create-func
     */
    static CorrectPlayerMovePredictionPacket create(math::Vector3 position, math::Vector3 delta, bool onGround, std::uint64_t tick, std::uint8_t predictionType, math::Vector2 vehicleRotation, std::optional<float> vehicleAngularVelocity);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CorrectPlayerMovePredictionPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
