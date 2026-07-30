/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ChunkCacheBlob.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace bedrock_protocol::types {

class ChunkCacheBlob {
public:
    ChunkCacheBlob(std::uint64_t hash, std::string payload) : hash(hash), payload(std::move(payload)) {}

    [[nodiscard]] std::uint64_t getHash() const { return hash; }

    [[nodiscard]] const std::string &getPayload() const { return payload; }

private:
    std::uint64_t hash;
    std::string payload;
};

}  // namespace bedrock_protocol::types
