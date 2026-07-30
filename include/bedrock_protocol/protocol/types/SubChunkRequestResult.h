/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SubChunkRequestResult.php
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

class SubChunkRequestResult final {
public:
    SubChunkRequestResult() = delete;

    static constexpr std::uint8_t SUCCESS = 1;
    //why even respond at all in these cases? ...
    static constexpr std::uint8_t NO_SUCH_CHUNK = 2;
    static constexpr std::uint8_t WRONG_DIMENSION = 3;
    static constexpr std::uint8_t NULL_PLAYER = 4;
    static constexpr std::uint8_t Y_INDEX_OUT_OF_BOUNDS = 5;
    static constexpr std::uint8_t SUCCESS_ALL_AIR = 6;
};

}  // namespace bedrock_protocol::types
