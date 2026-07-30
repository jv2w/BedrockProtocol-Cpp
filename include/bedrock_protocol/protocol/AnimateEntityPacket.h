/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AnimateEntityPacket.php
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

class AnimateEntityPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::ANIMATE_ENTITY_PACKET;


    std::string animation;
    std::string nextState;
    std::string stopExpression;
    std::int32_t stopExpressionVersion = 0;
    std::string controller;
    float blendOutTime = 0.0F;
    /**
     */
    std::vector<std::uint64_t> actorRuntimeIds;

    /**
     * @generate-create-func
     */
    static AnimateEntityPacket create(std::string animation, std::string nextState, std::string stopExpression, std::int32_t stopExpressionVersion, std::string controller, float blendOutTime, std::vector<std::uint64_t> actorRuntimeIds);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AnimateEntityPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
