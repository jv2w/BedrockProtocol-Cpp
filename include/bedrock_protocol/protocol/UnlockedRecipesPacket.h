/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/UnlockedRecipesPacket.php
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

class UnlockedRecipesPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::UNLOCKED_RECIPES_PACKET;

    static constexpr std::int32_t TYPE_EMPTY = 0;
    static constexpr std::int32_t TYPE_INITIALLY_UNLOCKED = 1;
    static constexpr std::int32_t TYPE_NEWLY_UNLOCKED = 2;
    static constexpr std::int32_t TYPE_REMOVE = 3;
    static constexpr std::int32_t TYPE_REMOVE_ALL = 4;

    std::uint32_t type = 0;
    /** @var string[] */
    std::vector<std::string> recipes;

    /**
     * @generate-create-func
     */
    static UnlockedRecipesPacket create(std::uint32_t type, std::vector<std::string> recipes);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "UnlockedRecipesPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
