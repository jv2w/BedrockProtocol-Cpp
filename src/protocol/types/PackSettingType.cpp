/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PackSettingType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PackSettingType.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

PackSettingType PackSettingTypeFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return PackSettingType::FLOAT;
    case 1:
        return PackSettingType::BOOL;
    case 2:
        return PackSettingType::STRING;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for PackSettingType");
    }
}

}  // namespace bedrock_protocol::types
