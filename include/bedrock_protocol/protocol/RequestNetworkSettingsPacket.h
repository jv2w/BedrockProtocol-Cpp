/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/RequestNetworkSettingsPacket.php
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
 * This is the first packet sent in a game session. It contains the client's protocol version.
 * The server is expected to respond to this with network settings, which will instruct the client which compression
 * type to use, amongst other things.
 */
class RequestNetworkSettingsPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::REQUEST_NETWORK_SETTINGS_PACKET;


    std::uint32_t protocolVersion = 0;

    /**
     * @generate-create-func
     */
    static RequestNetworkSettingsPacket create(std::uint32_t protocolVersion);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "RequestNetworkSettingsPacket"; }
    [[nodiscard]] bool canBeSentBeforeLogin() const override { return true; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
