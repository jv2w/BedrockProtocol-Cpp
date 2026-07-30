/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ContainerClosePacket.php
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

class ContainerClosePacket final : public DataPacket, public ClientboundPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CONTAINER_CLOSE_PACKET;


    std::uint8_t windowId = 0;
    std::uint8_t windowType = 0;
    bool server = false;

    /**
     * @generate-create-func
     */
    static ContainerClosePacket create(std::uint8_t windowId, std::uint8_t windowType, bool server);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ContainerClosePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
