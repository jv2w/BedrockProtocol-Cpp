/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueMap.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValue.h"
#include "bedrock_protocol/protocol/types/cereal/DynamicValueType.h"

namespace bedrock_protocol::types::cereal {

class DynamicValueMap final : public DynamicValue {
public:
    static constexpr std::uint32_t ID = DynamicValueType::MAP;

    /**
     * @param value
     * @note Keyed by string; values may be null. Encoding depends on iteration order, so this is a list of pairs.
     */
    explicit DynamicValueMap(std::vector<std::pair<std::string, std::unique_ptr<DynamicValue>>> value)
        : value(std::move(value))
    {
    }

    /**
     * @note Keyed by string; values may be null.
     */
    [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<DynamicValue>>> &getValue() const
    {
        return value;
    }

    [[nodiscard]] std::uint32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::unique_ptr<DynamicValue> clone() const override;

    /** @param depth nesting level of the map itself; its values sit one level deeper. */
    static DynamicValueMap readValue(encoding::ByteBufferReader &in, int depth = 0);

protected:
    void writeValue(encoding::ByteBufferWriter &out) const override;

private:
    std::vector<std::pair<std::string, std::unique_ptr<DynamicValue>>> value;
};

}  // namespace bedrock_protocol::types::cereal
