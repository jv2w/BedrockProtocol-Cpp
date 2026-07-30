/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetDisplayObjectivePacket.php
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

class SetDisplayObjectivePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_DISPLAY_OBJECTIVE_PACKET;

    static constexpr std::string_view DISPLAY_SLOT_LIST = "list";
    static constexpr std::string_view DISPLAY_SLOT_SIDEBAR = "sidebar";
    static constexpr std::string_view DISPLAY_SLOT_BELOW_NAME = "belowname";
    static constexpr std::int32_t SORT_ORDER_ASCENDING = 0;
    static constexpr std::int32_t SORT_ORDER_DESCENDING = 1;

    std::string displaySlot;
    std::string objectiveName;
    std::string displayName;
    std::string criteriaName;
    std::int32_t sortOrder = 0;

    /**
     * @generate-create-func
     */
    static SetDisplayObjectivePacket create(std::string displaySlot, std::string objectiveName, std::string displayName, std::string criteriaName, std::int32_t sortOrder);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetDisplayObjectivePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
