/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetScoreboardIdentityPacket.php
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
#include "bedrock_protocol/protocol/types/ScoreboardIdentityPacketEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class SetScoreboardIdentityPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_SCOREBOARD_IDENTITY_PACKET;

    static constexpr std::uint8_t TYPE_REGISTER_IDENTITY = 0;
    static constexpr std::int32_t TYPE_CLEAR_IDENTITY = 1;

    std::uint8_t type = 0;
    /** @var ScoreboardIdentityPacketEntry[] */
    std::vector<types::ScoreboardIdentityPacketEntry> entries;

    /**
     * @generate-create-func
     */
    static SetScoreboardIdentityPacket create(std::uint8_t type, std::vector<types::ScoreboardIdentityPacketEntry> entries);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetScoreboardIdentityPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
