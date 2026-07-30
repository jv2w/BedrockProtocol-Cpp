/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ServerboundLoadingScreenPacket.php
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
#include "bedrock_protocol/protocol/types/hud/LoadingScreenType.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class ServerboundLoadingScreenPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::SERVERBOUND_LOADING_SCREEN_PACKET;


    types::hud::LoadingScreenType loadingScreenType = types::hud::LoadingScreenType::UNKNOWN;
    std::optional<std::uint32_t> loadingScreenId = std::nullopt;

    /**
     * @generate-create-func
     */
    static ServerboundLoadingScreenPacket create(types::hud::LoadingScreenType loadingScreenType, std::optional<std::uint32_t> loadingScreenId);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "ServerboundLoadingScreenPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
