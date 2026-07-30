/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/resourcepacks/ResourcePackType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::resourcepacks {

class ResourcePackType final {
private:
    ResourcePackType() = delete;  //NOOP

public:
    static constexpr std::uint8_t INVALID = 0;
    static constexpr std::uint8_t ADDON = 1;
    static constexpr std::uint8_t CACHED = 2;
    static constexpr std::uint8_t COPY_PROTECTED = 3;
    static constexpr std::uint8_t BEHAVIORS = 4;
    static constexpr std::uint8_t PERSONA_PIECE = 5;
    static constexpr std::uint8_t RESOURCES = 6;
    static constexpr std::uint8_t SKINS = 7;
    static constexpr std::uint8_t WORLD_TEMPLATE = 8;
};

}  // namespace bedrock_protocol::types::resourcepacks
