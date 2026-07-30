/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ArmorSlot.php
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

enum class ArmorSlot : std::int32_t {
    HEAD = 0,
    TORSO = 1,
    LEGS = 2,
    FEET = 3,
    BODY = 4,
};

/**
 * @throws PacketDecodeException
 */
ArmorSlot ArmorSlotFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
