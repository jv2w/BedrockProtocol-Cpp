/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/ItemDescriptorType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::recipe {

class ItemDescriptorType final {
public:
    ItemDescriptorType() = delete;

    static constexpr std::uint8_t INT_ID_META = 1;
    static constexpr std::uint8_t MOLANG = 2;
    static constexpr std::uint8_t TAG = 3;
    static constexpr std::uint8_t STRING_ID_META = 4;
    static constexpr std::uint8_t COMPLEX_ALIAS = 5;
};

}  // namespace bedrock_protocol::types::recipe
