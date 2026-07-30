/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CompletedUsingItemPacket.php
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

class CompletedUsingItemPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::COMPLETED_USING_ITEM_PACKET;

    static constexpr std::int32_t ACTION_UNKNOWN = -1;
    static constexpr std::int32_t ACTION_EQUIP_ARMOR = 0;
    static constexpr std::int32_t ACTION_EAT = 1;
    static constexpr std::int32_t ACTION_ATTACK = 2;
    static constexpr std::int32_t ACTION_CONSUME = 3;
    static constexpr std::int32_t ACTION_THROW = 4;
    static constexpr std::int32_t ACTION_SHOOT = 5;
    static constexpr std::int32_t ACTION_PLACE = 6;
    static constexpr std::int32_t ACTION_FILL_BOTTLE = 7;
    static constexpr std::int32_t ACTION_FILL_BUCKET = 8;
    static constexpr std::int32_t ACTION_POUR_BUCKET = 9;
    static constexpr std::int32_t ACTION_USE_TOOL = 10;
    static constexpr std::int32_t ACTION_INTERACT = 11;
    static constexpr std::int32_t ACTION_RETRIEVED = 12;
    static constexpr std::int32_t ACTION_DYED = 13;
    static constexpr std::int32_t ACTION_TRADED = 14;
    static constexpr std::int32_t ACTION_BRUSHING_COMPLETED = 15;

    std::int16_t itemId = 0;
    std::int32_t action = 0;

    /**
     * @generate-create-func
     */
    static CompletedUsingItemPacket create(std::int16_t itemId, std::int32_t action);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CompletedUsingItemPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
