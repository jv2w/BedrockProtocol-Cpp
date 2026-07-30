/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/PropertySyncData.php
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

namespace bedrock_protocol::types::entity {

class PropertySyncData final {
public:
    /**
     * @param intProperties
     * @param floatProperties
     * @note Both maps are keyed by property index; the encode order is the insertion order, so they are
     *       stored as ordered key/value pairs rather than as hash maps.
     */
    PropertySyncData(std::vector<std::pair<std::uint32_t, std::int32_t>> intProperties,
                     std::vector<std::pair<std::uint32_t, float>> floatProperties)
        : intProperties(std::move(intProperties)), floatProperties(std::move(floatProperties))
    {
    }

    [[nodiscard]] const std::vector<std::pair<std::uint32_t, std::int32_t>> &getIntProperties() const
    {
        return intProperties;
    }

    [[nodiscard]] const std::vector<std::pair<std::uint32_t, float>> &getFloatProperties() const
    {
        return floatProperties;
    }

    static PropertySyncData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<std::pair<std::uint32_t, std::int32_t>> intProperties;
    std::vector<std::pair<std::uint32_t, float>> floatProperties;
};

}  // namespace bedrock_protocol::types::entity
