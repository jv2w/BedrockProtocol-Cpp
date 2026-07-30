/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/EmotePacket.php
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

class EmotePacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::EMOTE_PACKET;

    static constexpr std::int32_t FLAG_SERVER = 1 << 0;
    static constexpr std::int32_t FLAG_MUTE_ANNOUNCEMENT = 1 << 1;

    std::uint64_t actorRuntimeId = 0;
    std::string emoteId;
    std::uint32_t emoteLengthTicks = 0;
    std::string xboxUserId;
    std::string platformChatId;
    std::uint8_t flags = 0;

    /**
     * @generate-create-func
     */
    static EmotePacket create(std::uint64_t actorRuntimeId, std::string emoteId, std::uint32_t emoteLengthTicks, std::string xboxUserId, std::string platformChatId, std::uint8_t flags);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "EmotePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
