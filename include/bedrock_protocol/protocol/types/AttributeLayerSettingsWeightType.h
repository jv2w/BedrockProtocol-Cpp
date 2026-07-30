/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeLayerSettingsWeightType.php
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

/**
 * @see AttributeLayerSettingsWeight
 */
class AttributeLayerSettingsWeightType final {
public:
    AttributeLayerSettingsWeightType() = delete;

    static constexpr std::int32_t FLOAT = 0;
    static constexpr std::int32_t STRING = 1;
};

}  // namespace bedrock_protocol::types
