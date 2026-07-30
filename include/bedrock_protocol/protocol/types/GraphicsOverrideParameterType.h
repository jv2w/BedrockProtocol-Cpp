/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/GraphicsOverrideParameterType.php
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

enum class GraphicsOverrideParameterType : std::int32_t {
    SKY_ZENITH_COLOR = 0,
    SKY_HORIZON_COLOR = 1,
    HORIZON_BLEND_MIN = 2,
    HORIZON_BLEND_MAX = 3,
    HORIZON_BLEND_START = 4,
    HORIZON_BLEND_MIE_START = 5,
    RAYLEIGH_STRENGTH = 6,
    SUN_MIE_STRENGTH = 7,
    MOON_MIE_STRENGTH = 8,
    SUN_GLARE_SHAPE = 9,
    CHLOROPHYLL = 10,
    CDOM = 11,
    SUSPENDED_SEDIMENT = 12,
    WAVES_DEPTH = 13,
    WAVES_FREQUENCY = 14,
    WAVES_FREQUENCY_SCALING = 15,
    WAVES_SPEED = 16,
    WAVES_SPEED_SCALING = 17,
    WAVES_SHAPE = 18,
    WAVES_OCTAVES = 19,
    WAVES_MIX = 20,
    WAVES_PULL = 21,
    WAVES_DIRECTION_INCREMENT = 22,
    MIDTONES_CONTRAST = 23,
    HIGHLIGHTS_CONTRAST = 24,
    SHADOWS_CONTRAST = 25,
    HIGHLIGHTS_GAIN = 26,
    HIGHLIGHTS_GAMMA = 27,
    HIGHLIGHTS_OFFSET = 28,
    HIGHLIGHTS_SATURATION = 29,
    MIDTONES_GAIN = 30,
    MIDTONES_GAMMA = 31,
    MIDTONES_OFFSET = 32,
    MIDTONES_SATURATION = 33,
    SHADOWS_GAIN = 34,
    SHADOWS_GAMMA = 35,
    SHADOWS_OFFSET = 36,
    SHADOWS_SATURATION = 37,
    HIGHLIGHTS_MIN = 38,
    SHADOWS_MAX = 39,
    TEMPERATURE = 40,
    SUN_COLOR = 41,
    SUN_ILLUMINANCE = 42,
    MOON_COLOR = 43,
    MOON_ILLUMINANCE = 44,
    FLASH_COLOR = 45,
    FLASH_ILLUMINANCE = 46,
    AMBIENT_COLOR = 47,
    AMBIENT_ILLUMINANCE = 48,
};

/**
 * @throws PacketDecodeException
 */
GraphicsOverrideParameterType GraphicsOverrideParameterTypeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
