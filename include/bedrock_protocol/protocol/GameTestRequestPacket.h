/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/GameTestRequestPacket.php
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

class GameTestRequestPacket final : public DataPacket, public ServerboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::GAME_TEST_REQUEST_PACKET;

    static constexpr std::int32_t ROTATION_0 = 0;
    static constexpr std::int32_t ROTATION_90 = 1;
    static constexpr std::int32_t ROTATION_180 = 2;
    static constexpr std::int32_t ROTATION_270 = 3;

    std::int32_t maxTestsPerBatch = 0;
    std::int32_t repeatCount = 0;
    std::uint8_t rotation = 0;
    bool stopOnFailure = false;
    types::BlockPosition testPosition;
    std::int32_t testsPerRow = 0;
    std::string testName;

    /**
     * @generate-create-func
     */
    static GameTestRequestPacket create(std::int32_t maxTestsPerBatch, std::int32_t repeatCount, std::uint8_t rotation, bool stopOnFailure, types::BlockPosition testPosition, std::int32_t testsPerRow, std::string testName);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "GameTestRequestPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
