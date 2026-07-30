/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SpawnSettings.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class SpawnSettings final {
public:
    static constexpr std::uint16_t BIOME_TYPE_DEFAULT = 0;
    static constexpr std::uint16_t BIOME_TYPE_USER_DEFINED = 1;

    /** Not present in the PHP original; needed because LevelSettings::read() default-constructs the owner first. */
    SpawnSettings() = default;

    SpawnSettings(std::uint16_t biomeType, std::string biomeName, std::int32_t dimension)
        : biomeType(biomeType), biomeName(std::move(biomeName)), dimension(dimension)
    {
    }

    [[nodiscard]] std::uint16_t getBiomeType() const
    {
        return biomeType;
    }

    [[nodiscard]] const std::string &getBiomeName() const
    {
        return biomeName;
    }

    /**
     * @see DimensionIds
     */
    [[nodiscard]] std::int32_t getDimension() const
    {
        return dimension;
    }

    static SpawnSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint16_t biomeType = 0;
    std::string biomeName;
    std::int32_t dimension = 0;
};

}  // namespace bedrock_protocol::types
