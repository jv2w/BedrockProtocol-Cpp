/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/DimensionNameIds.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string_view>

namespace bedrock_protocol::types {

struct DimensionNameIds final {
    DimensionNameIds() = delete;

    static constexpr std::string_view OVERWORLD = "minecraft:overworld";
    static constexpr std::string_view NETHER = "minecraft:nether";
    static constexpr std::string_view THE_END = "minecraft:the_end";
};

}  // namespace bedrock_protocol::types
