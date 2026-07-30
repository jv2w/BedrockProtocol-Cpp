/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeValueColorType.php
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
 * @see AttributeValueColor
 */
class AttributeValueColorType final {
public:
    AttributeValueColorType() = delete;

    static constexpr std::int32_t STRING = 0;
    static constexpr std::int32_t ARRAY = 1;
};

}  // namespace bedrock_protocol::types
