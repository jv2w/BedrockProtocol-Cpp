/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ContainerSetDataPacket.php
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

class ContainerSetDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CONTAINER_SET_DATA_PACKET;

    static constexpr std::int32_t PROPERTY_FURNACE_SMELT_PROGRESS = 0;
    static constexpr std::int32_t PROPERTY_FURNACE_REMAINING_FUEL_TIME = 1;
    static constexpr std::int32_t PROPERTY_FURNACE_MAX_FUEL_TIME = 2;
    static constexpr std::int32_t PROPERTY_FURNACE_STORED_XP = 3;
    static constexpr std::int32_t PROPERTY_FURNACE_FUEL_AUX = 4;
    static constexpr std::int32_t PROPERTY_BREWING_STAND_BREW_TIME = 0;
    static constexpr std::int32_t PROPERTY_BREWING_STAND_FUEL_AMOUNT = 1;
    static constexpr std::int32_t PROPERTY_BREWING_STAND_FUEL_TOTAL = 2;

    std::uint8_t windowId = 0;
    std::int32_t property = 0;
    std::int32_t value = 0;

    /**
     * @generate-create-func
     */
    static ContainerSetDataPacket create(std::uint8_t windowId, std::int32_t property, std::int32_t value);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ContainerSetDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
