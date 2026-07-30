/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CommandBlockUpdatePacket.php
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
#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CommandBlockUpdatePacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::COMMAND_BLOCK_UPDATE_PACKET;


    bool isBlock = false;
    types::BlockPosition blockPosition;
    std::uint32_t commandBlockMode = 0;
    bool isRedstoneMode = false;
    bool isConditional = false;
    std::uint64_t minecartActorRuntimeId = 0;
    std::string command;
    std::string lastOutput;
    std::string name;
    std::string filteredName;
    bool shouldTrackOutput = false;
    std::uint32_t tickDelay = 0;
    bool executeOnFirstTick = false;

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CommandBlockUpdatePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
