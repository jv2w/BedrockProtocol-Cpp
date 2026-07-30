/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/hud/LoadingScreenType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::hud {

enum class LoadingScreenType : std::int32_t {
    UNKNOWN = 0,
    START_LOADING_SCREEN = 1,
    STOP_LOADING_SCREEN = 2,
};

/**
 * @throws PacketDecodeException
 */
LoadingScreenType LoadingScreenTypeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types::hud
