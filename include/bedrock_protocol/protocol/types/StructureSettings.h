/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/StructureSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>

#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol::types {

class StructureSettings {
public:
    std::string paletteName;
    bool ignoreEntities = false;
    bool ignoreBlocks = false;
    bool allowNonTickingChunks = false;
    BlockPosition dimensions;
    BlockPosition offset;
    std::int64_t lastTouchedByPlayerID = 0;
    std::uint8_t rotation = 0;
    std::uint8_t mirror = 0;
    std::uint8_t animationMode = 0;
    float animationSeconds = 0.0f;
    float integrityValue = 0.0f;
    std::uint32_t integritySeed = 0;
    math::Vector3 pivot;
};

}  // namespace bedrock_protocol::types
