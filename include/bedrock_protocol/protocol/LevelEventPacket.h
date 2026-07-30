/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LevelEventPacket.php
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
#include "bedrock_protocol/protocol/types/LevelEvent.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class LevelEventPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::LEVEL_EVENT_PACKET;


    /** @see LevelEvent */
    std::int32_t eventId = 0;
    std::int32_t eventData = 0;
    std::optional<math::Vector3> position = std::nullopt;

    /**
     * @generate-create-func
     */
    static LevelEventPacket create(std::int32_t eventId, std::int32_t eventData, std::optional<math::Vector3> position);

    static LevelEventPacket standardParticle(std::int32_t particleId, std::int32_t data, std::optional<math::Vector3> position);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "LevelEventPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
