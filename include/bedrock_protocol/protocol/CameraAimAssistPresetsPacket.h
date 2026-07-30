/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CameraAimAssistPresetsPacket.php
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
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategory.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPreset.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CameraAimAssistPresetsPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CAMERA_AIM_ASSIST_PRESETS_PACKET;


    /** @var CameraAimAssistCategory[] */
    std::vector<types::camera::CameraAimAssistCategory> categories;
    /** @var CameraAimAssistPreset[] */
    std::vector<types::camera::CameraAimAssistPreset> presets;
    std::uint8_t operation = 0;

    /**
     * @generate-create-func
     */
    static CameraAimAssistPresetsPacket create(std::vector<types::camera::CameraAimAssistCategory> categories, std::vector<types::camera::CameraAimAssistPreset> presets, std::uint8_t operation);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CameraAimAssistPresetsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
