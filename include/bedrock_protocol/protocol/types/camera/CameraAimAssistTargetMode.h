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

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::camera {

enum class CameraAimAssistTargetMode : std::int32_t {
    ANGLE = 0,
    DISTANCE = 1,
};

/**
 * @throws PacketDecodeException
 */
CameraAimAssistTargetMode CameraAimAssistTargetModeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::camera
