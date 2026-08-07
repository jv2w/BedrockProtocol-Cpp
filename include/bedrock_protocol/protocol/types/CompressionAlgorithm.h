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

    /**
     * Sentinel for "batches are not compressed". gophertunnel v1.58.0
     * minecraft/protocol/packet/network_settings.go:11 defines this as 0xffff; the value goes out in the
     * uint16 CompressionAlgorithm field of NetworkSettings, so 255 would read as an unknown algorithm.
     */
    static constexpr std::uint16_t NONE = 0xffff;
};

}  // namespace bedrock_protocol::types
