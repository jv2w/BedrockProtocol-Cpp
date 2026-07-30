/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/GraphicsOverrideParameterType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/GraphicsOverrideParameterType.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

GraphicsOverrideParameterType GraphicsOverrideParameterTypeFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return GraphicsOverrideParameterType::SKY_ZENITH_COLOR;
    case 1:
        return GraphicsOverrideParameterType::SKY_HORIZON_COLOR;
    case 2:
        return GraphicsOverrideParameterType::HORIZON_BLEND_MIN;
    case 3:
        return GraphicsOverrideParameterType::HORIZON_BLEND_MAX;
    case 4:
        return GraphicsOverrideParameterType::HORIZON_BLEND_START;
    case 5:
        return GraphicsOverrideParameterType::HORIZON_BLEND_MIE_START;
    case 6:
        return GraphicsOverrideParameterType::RAYLEIGH_STRENGTH;
    case 7:
        return GraphicsOverrideParameterType::SUN_MIE_STRENGTH;
    case 8:
        return GraphicsOverrideParameterType::MOON_MIE_STRENGTH;
    case 9:
        return GraphicsOverrideParameterType::SUN_GLARE_SHAPE;
    case 10:
        return GraphicsOverrideParameterType::CHLOROPHYLL;
    case 11:
        return GraphicsOverrideParameterType::CDOM;
    case 12:
        return GraphicsOverrideParameterType::SUSPENDED_SEDIMENT;
    case 13:
        return GraphicsOverrideParameterType::WAVES_DEPTH;
    case 14:
        return GraphicsOverrideParameterType::WAVES_FREQUENCY;
    case 15:
        return GraphicsOverrideParameterType::WAVES_FREQUENCY_SCALING;
    case 16:
        return GraphicsOverrideParameterType::WAVES_SPEED;
    case 17:
        return GraphicsOverrideParameterType::WAVES_SPEED_SCALING;
    case 18:
        return GraphicsOverrideParameterType::WAVES_SHAPE;
    case 19:
        return GraphicsOverrideParameterType::WAVES_OCTAVES;
    case 20:
        return GraphicsOverrideParameterType::WAVES_MIX;
    case 21:
        return GraphicsOverrideParameterType::WAVES_PULL;
    case 22:
        return GraphicsOverrideParameterType::WAVES_DIRECTION_INCREMENT;
    case 23:
        return GraphicsOverrideParameterType::MIDTONES_CONTRAST;
    case 24:
        return GraphicsOverrideParameterType::HIGHLIGHTS_CONTRAST;
    case 25:
        return GraphicsOverrideParameterType::SHADOWS_CONTRAST;
    case 26:
        return GraphicsOverrideParameterType::HIGHLIGHTS_GAIN;
    case 27:
        return GraphicsOverrideParameterType::HIGHLIGHTS_GAMMA;
    case 28:
        return GraphicsOverrideParameterType::HIGHLIGHTS_OFFSET;
    case 29:
        return GraphicsOverrideParameterType::HIGHLIGHTS_SATURATION;
    case 30:
        return GraphicsOverrideParameterType::MIDTONES_GAIN;
    case 31:
        return GraphicsOverrideParameterType::MIDTONES_GAMMA;
    case 32:
        return GraphicsOverrideParameterType::MIDTONES_OFFSET;
    case 33:
        return GraphicsOverrideParameterType::MIDTONES_SATURATION;
    case 34:
        return GraphicsOverrideParameterType::SHADOWS_GAIN;
    case 35:
        return GraphicsOverrideParameterType::SHADOWS_GAMMA;
    case 36:
        return GraphicsOverrideParameterType::SHADOWS_OFFSET;
    case 37:
        return GraphicsOverrideParameterType::SHADOWS_SATURATION;
    case 38:
        return GraphicsOverrideParameterType::HIGHLIGHTS_MIN;
    case 39:
        return GraphicsOverrideParameterType::SHADOWS_MAX;
    case 40:
        return GraphicsOverrideParameterType::TEMPERATURE;
    case 41:
        return GraphicsOverrideParameterType::SUN_COLOR;
    case 42:
        return GraphicsOverrideParameterType::SUN_ILLUMINANCE;
    case 43:
        return GraphicsOverrideParameterType::MOON_COLOR;
    case 44:
        return GraphicsOverrideParameterType::MOON_ILLUMINANCE;
    case 45:
        return GraphicsOverrideParameterType::FLASH_COLOR;
    case 46:
        return GraphicsOverrideParameterType::FLASH_ILLUMINANCE;
    case 47:
        return GraphicsOverrideParameterType::AMBIENT_COLOR;
    case 48:
        return GraphicsOverrideParameterType::AMBIENT_ILLUMINANCE;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) +
                                    " for GraphicsOverrideParameterType");
    }
}

}  // namespace bedrock_protocol::types
