/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/ChainedSubCommandValueRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::command::raw {

class ChainedSubCommandValueRawData final {
public:
    ChainedSubCommandValueRawData(std::uint32_t nameIndex, std::uint32_t type) : nameIndex(nameIndex), type(type) {}

    [[nodiscard]] std::uint32_t getNameIndex() const
    {
        return nameIndex;
    }

    [[nodiscard]] std::uint32_t getType() const
    {
        return type;
    }

    static ChainedSubCommandValueRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t nameIndex;
    std::uint32_t type;
};

}  // namespace bedrock_protocol::types::command::raw
