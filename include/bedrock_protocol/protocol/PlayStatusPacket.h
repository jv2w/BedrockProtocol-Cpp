/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayStatusPacket.php
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

class PlayStatusPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PLAY_STATUS_PACKET;

    static constexpr std::int32_t LOGIN_SUCCESS = 0;
    static constexpr std::int32_t LOGIN_FAILED_CLIENT = 1;
    static constexpr std::int32_t LOGIN_FAILED_SERVER = 2;
    static constexpr std::int32_t PLAYER_SPAWN = 3;
    static constexpr std::int32_t LOGIN_FAILED_INVALID_TENANT = 4;
    static constexpr std::int32_t LOGIN_FAILED_VANILLA_EDU = 5;
    static constexpr std::int32_t LOGIN_FAILED_EDU_VANILLA = 6;
    static constexpr std::int32_t LOGIN_FAILED_SERVER_FULL = 7;
    static constexpr std::int32_t LOGIN_FAILED_EDITOR_VANILLA = 8;
    static constexpr std::int32_t LOGIN_FAILED_VANILLA_EDITOR = 9;

    std::uint32_t status = 0;

    /**
     * @generate-create-func
     */
    static PlayStatusPacket create(std::uint32_t status);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PlayStatusPacket"; }
    [[nodiscard]] bool canBeSentBeforeLogin() const override { return true; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
