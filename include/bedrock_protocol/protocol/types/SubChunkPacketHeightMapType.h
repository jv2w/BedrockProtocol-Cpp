/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPacketHeightMapType.php
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

class SubChunkPacketHeightMapType final {
public:
    SubChunkPacketHeightMapType() = delete;

    static constexpr std::uint8_t NO_DATA = 0;
    static constexpr std::uint8_t DATA = 1;
    static constexpr std::uint8_t ALL_TOO_HIGH = 2;
    static constexpr std::uint8_t ALL_TOO_LOW = 3;
    static constexpr std::uint8_t ALL_COPIED = 4;
};

}  // namespace bedrock_protocol::types
