/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/CommandEnumRawData.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::command::raw {

class CommandEnumRawData final {
public:
    /**
     * @param valueIndexes
     */
    CommandEnumRawData(std::string name, std::vector<std::uint32_t> valueIndexes)
        : name(std::move(name)), valueIndexes(std::move(valueIndexes))
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::vector<std::uint32_t> &getValueIndexes() const
    {
        return valueIndexes;
    }

    static CommandEnumRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::vector<std::uint32_t> valueIndexes;
};

}  // namespace bedrock_protocol::types::command::raw
