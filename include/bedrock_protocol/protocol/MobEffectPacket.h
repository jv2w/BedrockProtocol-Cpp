/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MobEffectPacket.php
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

class MobEffectPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::MOB_EFFECT_PACKET;

    static constexpr std::int32_t EVENT_ADD = 1;
    static constexpr std::int32_t EVENT_MODIFY = 2;
    static constexpr std::int32_t EVENT_REMOVE = 3;

    std::uint64_t actorRuntimeId = 0;
    std::uint8_t eventId = 0;
    std::int32_t effectId = 0;
    std::int32_t amplifier = 0;
    bool particles = true;
    std::int32_t duration = 0;
    std::uint64_t tick = 0;
    bool ambient = true;

    /**
     * @generate-create-func
     */
    static MobEffectPacket create(std::uint64_t actorRuntimeId, std::uint8_t eventId, std::int32_t effectId, std::int32_t amplifier, bool particles, std::int32_t duration, std::uint64_t tick, bool ambient);

    static MobEffectPacket add(std::uint64_t actorRuntimeId, bool replace, std::int32_t effectId, std::int32_t amplifier, bool particles, std::int32_t duration, std::uint64_t tick, bool ambient);

    static MobEffectPacket remove(std::uint64_t actorRuntimeId, std::int32_t effectId, std::uint64_t tick);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "MobEffectPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
