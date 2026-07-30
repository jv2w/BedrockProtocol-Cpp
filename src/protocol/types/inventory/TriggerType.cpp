/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/TriggerType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/TriggerType.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::inventory {

TriggerType TriggerTypeFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return TriggerType::UNKNOWN;
    case 1:
        return TriggerType::PLAYER_INPUT;
    case 2:
        return TriggerType::SIMULATION_TICK;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for TriggerType");
    }
}

}  // namespace bedrock_protocol::types::inventory
