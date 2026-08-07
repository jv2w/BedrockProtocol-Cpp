/*
 * This file is part of BedrockProtocol-Cpp.
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

    //gophertunnel minecraft/protocol/item_descriptor.go:19-24 - the table was renumbered in 1.26.40 and the
    //int-ID, deferred and complex-alias descriptors no longer exist. Slot 1 is the string-name descriptor.
    static constexpr std::uint8_t INVALID = 0;
    static constexpr std::uint8_t STRING_ID_META = 1;
    static constexpr std::uint8_t MOLANG = 2;
    static constexpr std::uint8_t TAG = 3;
};

}  // namespace bedrock_protocol::types::recipe
