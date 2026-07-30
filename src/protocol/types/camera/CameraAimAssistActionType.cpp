/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/camera/CameraAimAssistActionType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraAimAssistActionType.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::camera {

CameraAimAssistActionType CameraAimAssistActionTypeFromPacket(std::int32_t value)
{
    switch (value) {
    case static_cast<std::int32_t>(CameraAimAssistActionType::SET):
        return CameraAimAssistActionType::SET;
    case static_cast<std::int32_t>(CameraAimAssistActionType::CLEAR):
        return CameraAimAssistActionType::CLEAR;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for CameraAimAssistActionType");
    }
}

}  // namespace bedrock_protocol::types::camera
