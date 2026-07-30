/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/BlockPosition.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol::types {

class BlockPosition final {
public:
    /** Not present in the PHP original; needed because LevelSettings::read() default-constructs the owner first. */
    BlockPosition() = default;

    BlockPosition(std::int32_t x, std::int32_t y, std::int32_t z) : x(x), y(y), z(z) {}

    [[nodiscard]] std::int32_t getX() const { return x; }

    [[nodiscard]] std::int32_t getY() const { return y; }

    [[nodiscard]] std::int32_t getZ() const { return z; }

    static BlockPosition fromVector3(const math::Vector3 &vector3)
    {
        return {vector3.getFloorX(), vector3.getFloorY(), vector3.getFloorZ()};
    }

    [[nodiscard]] bool equals(const BlockPosition &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    friend bool operator==(const BlockPosition &a, const BlockPosition &b) { return a.equals(b); }
    friend bool operator!=(const BlockPosition &a, const BlockPosition &b) { return !a.equals(b); }

private:
    std::int32_t x = 0;
    std::int32_t y = 0;
    std::int32_t z = 0;
};

}  // namespace bedrock_protocol::types
