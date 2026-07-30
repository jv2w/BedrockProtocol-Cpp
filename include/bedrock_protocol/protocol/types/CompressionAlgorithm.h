/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/CompressionAlgorithm.php
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
 * List of supported compression algorithms for compressing packet batches.
 */
class CompressionAlgorithm final {
private:
    CompressionAlgorithm()
    {
        //NOOP
    }

public:
    static constexpr std::uint16_t ZLIB = 0;
    static constexpr std::uint16_t SNAPPY = 1;

    static constexpr std::uint16_t NONE = 255;
};

}  // namespace bedrock_protocol::types
