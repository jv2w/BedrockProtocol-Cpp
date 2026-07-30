/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/math src/Vector3.php
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

/**
 * An immutable point or offset in 3D space.
 *
 * Only the subset of PocketMine's Vector3 that the protocol actually uses is ported; the geometry
 * helpers (ray tracing, side lookups) belong to the server, not to the wire format.
 */
class Vector3 {
public:
    float x = 0.0F;
    float y = 0.0F;
    float z = 0.0F;

    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    [[nodiscard]] float getX() const { return x; }
    [[nodiscard]] float getY() const { return y; }
    [[nodiscard]] float getZ() const { return z; }

    [[nodiscard]] std::int32_t getFloorX() const { return static_cast<std::int32_t>(std::floor(x)); }
    [[nodiscard]] std::int32_t getFloorY() const { return static_cast<std::int32_t>(std::floor(y)); }
    [[nodiscard]] std::int32_t getFloorZ() const { return static_cast<std::int32_t>(std::floor(z)); }

    [[nodiscard]] Vector3 add(float dx, float dy, float dz) const { return {x + dx, y + dy, z + dz}; }
    [[nodiscard]] Vector3 addVector(const Vector3 &other) const { return add(other.x, other.y, other.z); }
    [[nodiscard]] Vector3 subtract(float dx, float dy, float dz) const { return add(-dx, -dy, -dz); }
    [[nodiscard]] Vector3 subtractVector(const Vector3 &other) const { return add(-other.x, -other.y, -other.z); }
    [[nodiscard]] Vector3 multiply(float number) const { return {x * number, y * number, z * number}; }
    [[nodiscard]] Vector3 divide(float number) const { return {x / number, y / number, z / number}; }

    [[nodiscard]] float lengthSquared() const { return x * x + y * y + z * z; }
    [[nodiscard]] float length() const { return std::sqrt(lengthSquared()); }

    [[nodiscard]] bool equals(const Vector3 &other) const { return x == other.x && y == other.y && z == other.z; }

    friend bool operator==(const Vector3 &lhs, const Vector3 &rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const Vector3 &lhs, const Vector3 &rhs) { return !lhs.equals(rhs); }
};

}  // namespace bedrock_protocol::math
