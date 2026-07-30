/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistTargetMode.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistTargetMode.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::camera {

CameraAimAssistTargetMode CameraAimAssistTargetModeFromPacket(std::int32_t value)
{
    switch (value) {
    case static_cast<std::int32_t>(CameraAimAssistTargetMode::ANGLE):
        return CameraAimAssistTargetMode::ANGLE;
    case static_cast<std::int32_t>(CameraAimAssistTargetMode::DISTANCE):
        return CameraAimAssistTargetMode::DISTANCE;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for CameraAimAssistTargetMode");
    }
}

}  // namespace bedrock_protocol::types::camera
