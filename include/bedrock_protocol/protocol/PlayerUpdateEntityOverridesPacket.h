/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayerUpdateEntityOverridesPacket.php
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
#include "bedrock_protocol/protocol/types/OverrideUpdateType.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class PlayerUpdateEntityOverridesPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PLAYER_UPDATE_ENTITY_OVERRIDES_PACKET;


    std::uint64_t actorRuntimeId = 0;
    std::uint32_t propertyIndex = 0;
    types::OverrideUpdateType updateType = types::OverrideUpdateType::CLEAR_OVERRIDES;
    std::optional<std::int32_t> intOverrideValue;
    std::optional<float> floatOverrideValue;

    /**
     * @generate-create-func
     */
    static PlayerUpdateEntityOverridesPacket create(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, types::OverrideUpdateType updateType, std::optional<std::int32_t> intOverrideValue, std::optional<float> floatOverrideValue);

    static PlayerUpdateEntityOverridesPacket createIntOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, std::int32_t value);

    static PlayerUpdateEntityOverridesPacket createFloatOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, float value);

    static PlayerUpdateEntityOverridesPacket createClearOverrides(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex);

    static PlayerUpdateEntityOverridesPacket createRemoveOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PlayerUpdateEntityOverridesPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
