/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CameraAimAssistPacket.php
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
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistActionType.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistTargetMode.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CameraAimAssistPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CAMERA_AIM_ASSIST_PACKET;


    std::string presetId;
    math::Vector2 viewAngle;
    float distance = 0.0F;
    types::camera::CameraAimAssistTargetMode targetMode = types::camera::CameraAimAssistTargetMode::ANGLE;
    types::camera::CameraAimAssistActionType actionType = types::camera::CameraAimAssistActionType::SET;
    bool showDebugRender = false;

    /**
     * @generate-create-func
     */
    static CameraAimAssistPacket create(std::string presetId, math::Vector2 viewAngle, float distance, types::camera::CameraAimAssistTargetMode targetMode, types::camera::CameraAimAssistActionType actionType, bool showDebugRender);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CameraAimAssistPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
