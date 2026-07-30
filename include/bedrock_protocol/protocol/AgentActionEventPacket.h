/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AgentActionEventPacket.php
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

/**
 * Used by code builder, exact purpose unclear
 */
class AgentActionEventPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::AGENT_ACTION_EVENT_PACKET;


    std::string requestId;
    /** @see AgentActionType */
    std::uint32_t action = 0;
    std::string responseJson;

    /**
     * @generate-create-func
     */
    static AgentActionEventPacket create(std::string requestId, std::uint32_t action, std::string responseJson);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AgentActionEventPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
