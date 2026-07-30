/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/WorldPosition.php
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
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol::types {

class WorldPosition final {
public:
    WorldPosition(math::Vector3 position, std::int32_t dimension) : position(position), dimension(dimension) {}

    [[nodiscard]] const math::Vector3 &getPosition() const { return position; }

    [[nodiscard]] std::int32_t getDimension() const { return dimension; }

    static WorldPosition read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    math::Vector3 position;
    std::int32_t dimension;
};

}  // namespace bedrock_protocol::types
