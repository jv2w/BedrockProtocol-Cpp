/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MovementEffectType.php
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

enum class MovementEffectType : std::int32_t {
    INVALID = -1,
    GLIDE_BOOST = 0,
};

/**
 * @throws PacketDecodeException
 */
MovementEffectType MovementEffectTypeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
