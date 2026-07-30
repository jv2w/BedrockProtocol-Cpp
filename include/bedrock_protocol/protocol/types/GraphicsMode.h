/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/GraphicsMode.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

enum class GraphicsMode : std::int32_t {
    SIMPLE = 0,
    FANCY = 1,
    ADVANCED = 2,
    RAY_TRACED = 3,
};

/**
 * @throws PacketDecodeException
 */
GraphicsMode GraphicsModeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
