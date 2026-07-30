/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NetworkSettingsPacket.php
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
 * This is the first packet sent by the server in a game session, in response to a network settings request (only if
 * protocol versions are a match). It includes values for things like which compression algorithm to use, size threshold
 * for compressing packets, and more.
 */
class NetworkSettingsPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::NETWORK_SETTINGS_PACKET;

    static constexpr std::int32_t COMPRESS_NOTHING = 0;
    static constexpr std::int32_t COMPRESS_EVERYTHING = 1;

    std::uint16_t compressionThreshold = 0;
    std::uint16_t compressionAlgorithm = 0;
    bool enableClientThrottling = false;
    std::uint8_t clientThrottleThreshold = 0;
    float clientThrottleScalar = 0.0F;

    /**
     * @generate-create-func
     */
    static NetworkSettingsPacket create(std::uint16_t compressionThreshold, std::uint16_t compressionAlgorithm, bool enableClientThrottling, std::uint8_t clientThrottleThreshold, float clientThrottleScalar);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "NetworkSettingsPacket"; }
    [[nodiscard]] bool canBeSentBeforeLogin() const override { return true; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
