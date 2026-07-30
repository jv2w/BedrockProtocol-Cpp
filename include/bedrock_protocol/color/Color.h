/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/color src/Color.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::color {

/** An immutable RGBA colour, each channel held as an 8-bit value. */
class Color {
public:
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0xff) : a_(a), r_(r), g_(g), b_(b) {}

    /** Returns the alpha (opacity) value of this colour. */
    [[nodiscard]] std::uint8_t getA() const { return a_; }
    /** Retuns the red value of this colour. */
    [[nodiscard]] std::uint8_t getR() const { return r_; }
    /** Returns the green value of this colour. */
    [[nodiscard]] std::uint8_t getG() const { return g_; }
    /** Returns the blue value of this colour. */
    [[nodiscard]] std::uint8_t getB() const { return b_; }

    /** Returns a Color from the supplied RGB colour code (opaque). */
    static Color fromRGB(std::uint32_t code)
    {
        return {static_cast<std::uint8_t>((code >> 16) & 0xff), static_cast<std::uint8_t>((code >> 8) & 0xff),
                static_cast<std::uint8_t>(code & 0xff)};
    }

    /** Returns a Color from the supplied ARGB colour code. */
    static Color fromARGB(std::uint32_t code)
    {
        return {static_cast<std::uint8_t>((code >> 16) & 0xff), static_cast<std::uint8_t>((code >> 8) & 0xff),
                static_cast<std::uint8_t>(code & 0xff), static_cast<std::uint8_t>((code >> 24) & 0xff)};
    }

    /** Returns an ARGB 32-bit colour value. */
    [[nodiscard]] std::uint32_t toARGB() const
    {
        return (static_cast<std::uint32_t>(a_) << 24) | (static_cast<std::uint32_t>(r_) << 16) |
               (static_cast<std::uint32_t>(g_) << 8) | static_cast<std::uint32_t>(b_);
    }

    /** Returns a Color from the supplied RGBA colour code. */
    static Color fromRGBA(std::uint32_t code)
    {
        return {static_cast<std::uint8_t>((code >> 24) & 0xff), static_cast<std::uint8_t>((code >> 16) & 0xff),
                static_cast<std::uint8_t>((code >> 8) & 0xff), static_cast<std::uint8_t>(code & 0xff)};
    }

    /** Returns an RGBA 32-bit colour value. */
    [[nodiscard]] std::uint32_t toRGBA() const
    {
        return (static_cast<std::uint32_t>(r_) << 24) | (static_cast<std::uint32_t>(g_) << 16) |
               (static_cast<std::uint32_t>(b_) << 8) | static_cast<std::uint32_t>(a_);
    }

    [[nodiscard]] bool equals(const Color &other) const
    {
        return r_ == other.r_ && g_ == other.g_ && b_ == other.b_ && a_ == other.a_;
    }

    friend bool operator==(const Color &lhs, const Color &rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const Color &lhs, const Color &rhs) { return !lhs.equals(rhs); }

private:
    std::uint8_t a_;
    std::uint8_t r_;
    std::uint8_t g_;
    std::uint8_t b_;
};

}  // namespace bedrock_protocol::color
