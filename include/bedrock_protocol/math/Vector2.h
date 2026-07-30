/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/math src/Vector2.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cmath>
#include <cstdint>

namespace bedrock_protocol::math {

/** An immutable point or offset in 2D space. */
class Vector2 {
public:
    float x = 0.0F;
    float y = 0.0F;

    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}

    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }

    [[nodiscard]] std::int32_t getFloorX() const { return static_cast<std::int32_t>(std::floor(x)); }
    [[nodiscard]] std::int32_t getFloorY() const { return static_cast<std::int32_t>(std::floor(y)); }

    [[nodiscard]] Vector2 add(float dx, float dy) const { return {x + dx, y + dy}; }
    [[nodiscard]] Vector2 addVector(const Vector2 &other) const { return add(other.x, other.y); }
    [[nodiscard]] Vector2 subtract(float dx, float dy) const { return add(-dx, -dy); }
    [[nodiscard]] Vector2 subtractVector(const Vector2 &other) const { return add(-other.x, -other.y); }
    [[nodiscard]] Vector2 multiply(float number) const { return {x * number, y * number}; }
    [[nodiscard]] Vector2 divide(float number) const { return {x / number, y / number}; }

    [[nodiscard]] float lengthSquared() const { return x * x + y * y; }
    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    [[nodiscard]] bool equals(const Vector2 &other) const { return x == other.x && y == other.y; }

    friend bool operator==(const Vector2 &lhs, const Vector2 &rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const Vector2 &lhs, const Vector2 &rhs) { return !lhs.equals(rhs); }
};

}  // namespace bedrock_protocol::math
