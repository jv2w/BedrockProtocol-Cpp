/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/NpcRequestPacket.php
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

class NpcRequestPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::NPC_REQUEST_PACKET;

    static constexpr std::int32_t REQUEST_SET_ACTIONS = 0;
    static constexpr std::int32_t REQUEST_EXECUTE_ACTION = 1;
    static constexpr std::int32_t REQUEST_EXECUTE_CLOSING_COMMANDS = 2;
    static constexpr std::int32_t REQUEST_SET_NAME = 3;
    static constexpr std::int32_t REQUEST_SET_SKIN = 4;
    static constexpr std::int32_t REQUEST_SET_INTERACTION_TEXT = 5;
    static constexpr std::int32_t REQUEST_EXECUTE_OPENING_COMMANDS = 6;

    std::uint64_t actorRuntimeId = 0;
    std::uint8_t requestType = 0;
    std::string commandString;
    std::uint8_t actionIndex = 0;
    std::string sceneName;

    /**
     * @generate-create-func
     */
    static NpcRequestPacket create(std::uint64_t actorRuntimeId, std::uint8_t requestType, std::string commandString, std::uint8_t actionIndex, std::string sceneName);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "NpcRequestPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
