/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/CommandEnumConstraintRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::command::raw {

class CommandEnumConstraintRawData final {
public:
    /**
     * @param constraints
     */
    CommandEnumConstraintRawData(std::uint32_t affectedValueIndex, std::uint32_t enumIndex,
                                 std::vector<std::uint8_t> constraints)
        : affectedValueIndex(affectedValueIndex), enumIndex(enumIndex), constraints(std::move(constraints))
    {
    }

    [[nodiscard]] std::uint32_t getAffectedValueIndex() const
    {
        return affectedValueIndex;
    }

    [[nodiscard]] std::uint32_t getEnumIndex() const
    {
        return enumIndex;
    }

    [[nodiscard]] const std::vector<std::uint8_t> &getConstraints() const
    {
        return constraints;
    }

    static CommandEnumConstraintRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t affectedValueIndex;
    std::uint32_t enumIndex;
    std::vector<std::uint8_t> constraints;
};

}  // namespace bedrock_protocol::types::command::raw
