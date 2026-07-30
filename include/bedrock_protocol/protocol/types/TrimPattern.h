/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/TrimPattern.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class TrimPattern final {
public:
    TrimPattern(std::string itemId, std::string patternId)
        : itemId(std::move(itemId)), patternId(std::move(patternId))
    {
    }

    [[nodiscard]] const std::string &getItemId() const { return itemId; }

    [[nodiscard]] const std::string &getPatternId() const { return patternId; }

    static TrimPattern read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string itemId;
    std::string patternId;
};

}  // namespace bedrock_protocol::types
