/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/EntityMetadataTypes.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::entity {

class EntityMetadataTypes final {
    EntityMetadataTypes() = delete;

public:
    static constexpr std::int32_t BYTE = 0;
    static constexpr std::int32_t SHORT = 1;
    static constexpr std::int32_t INT = 2;
    static constexpr std::int32_t FLOAT = 3;
    static constexpr std::int32_t STRING = 4;
    static constexpr std::int32_t COMPOUND_TAG = 5;
    static constexpr std::int32_t POS = 6;
    static constexpr std::int32_t LONG = 7;
    static constexpr std::int32_t VECTOR3F = 8;
};

}  // namespace bedrock_protocol::types::entity
