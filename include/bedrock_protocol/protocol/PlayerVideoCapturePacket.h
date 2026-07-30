/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerVideoCapturePacket.php
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

class PlayerVideoCapturePacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::PLAYER_VIDEO_CAPTURE_PACKET;


    bool recording = false;
    std::optional<std::uint32_t> frameRate;
    std::optional<std::string> filePrefix;

    static PlayerVideoCapturePacket createStartRecording(std::uint32_t frameRate, std::string filePrefix);

    static PlayerVideoCapturePacket createStopRecording();

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "PlayerVideoCapturePacket"; }
    bool handle(PacketHandlerInterface &handler) override;

private:
    /**
     * @generate-create-func
     */
    static PlayerVideoCapturePacket create(bool recording, std::optional<std::uint32_t> frameRate, std::optional<std::string> filePrefix);

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
