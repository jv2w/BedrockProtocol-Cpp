/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SpawnParticleEffectPacket.php
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
#include "bedrock_protocol/protocol/types/DimensionIds.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class SpawnParticleEffectPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SPAWN_PARTICLE_EFFECT_PACKET;


    std::uint8_t dimensionId = types::DimensionIds::OVERWORLD;  //wtf mojang
    std::int64_t actorUniqueId = -1;  //default none
    math::Vector3 position;
    std::string particleName;
    std::optional<std::string> molangVariablesJson = std::nullopt;

    /**
     * @generate-create-func
     */
    static SpawnParticleEffectPacket create(std::uint8_t dimensionId, std::int64_t actorUniqueId, math::Vector3 position, std::string particleName, std::optional<std::string> molangVariablesJson);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SpawnParticleEffectPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
