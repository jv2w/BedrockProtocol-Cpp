/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetScorePacket.php
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
#include "bedrock_protocol/protocol/types/ScorePacketEntry.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class SetScorePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SET_SCORE_PACKET;

    static constexpr std::int32_t TYPE_CHANGE = 0;
    static constexpr std::uint8_t TYPE_REMOVE = 1;

    std::uint8_t type = 0;
    /** @var ScorePacketEntry[] */
    std::vector<types::ScorePacketEntry> entries;

    /**
     * @generate-create-func
     */
    static SetScorePacket create(std::uint8_t type, std::vector<types::ScorePacketEntry> entries);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "SetScorePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
