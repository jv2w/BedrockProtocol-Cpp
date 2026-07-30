/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RequestAbilityPacket.php
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
#include <variant>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

/**
 * Sent by the client to request server enabling/disabling/changing certain abilities, such as flying, noclip, etc.
 * As of 1.19.0, the vanilla server only handles this for flying/noclip, despite there being a large range of additional
 * abilities which could be requested, and the packet supporting the use of float values.
 */
class RequestAbilityPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::REQUEST_ABILITY_PACKET;

    static constexpr std::int32_t ABILITY_FLYING = 9;
    static constexpr std::int32_t ABILITY_NOCLIP = 17;

    std::int32_t abilityId = 0;
    std::variant<bool, float> abilityValue;

    /**
     * @generate-create-func
     */
    static RequestAbilityPacket create(std::int32_t abilityId, std::variant<bool, float> abilityValue);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "RequestAbilityPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;

private:
    static constexpr std::int32_t VALUE_TYPE_BOOL = 1;
    static constexpr std::int32_t VALUE_TYPE_FLOAT = 2;
};

}  // namespace bedrock_protocol
