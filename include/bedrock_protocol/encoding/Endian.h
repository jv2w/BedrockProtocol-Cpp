/*
 * This file is part of BedrockProtocol-Cpp.
 * Internal helper with no PHP counterpart: PHP's pack()/unpack() handle endianness natively,
 * whereas the C++ port has to convert explicitly.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
#include <type_traits>

/** Endianness conversion and raw fixed-width transfer helpers shared by LE and BE. */
namespace bedrock_protocol::encoding::detail {

template <typename T>
[[nodiscard]] constexpr T byteSwap(T value) noexcept
{
    static_assert(std::is_unsigned_v<T>, "byteSwap operates on unsigned integers only");
    if constexpr (sizeof(T) == 1) {
        return value;
    }
    else {
        auto bytes = std::bit_cast<std::array<std::uint8_t, sizeof(T)>>(value);
        for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
            const auto tmp = bytes[i];
            bytes[i] = bytes[sizeof(T) - 1 - i];
            bytes[sizeof(T) - 1 - i] = tmp;
        }
        return std::bit_cast<T>(bytes);
    }
}

/** Reinterprets `length` bytes at `source` as the unsigned integer T, honouring the requested order. */
template <typename T>
[[nodiscard]] inline T fromBytes(const char *source, bool little) noexcept
{
    T value{};
    std::memcpy(&value, source, sizeof(T));
    const bool hostLittle = std::endian::native == std::endian::little;
    return hostLittle == little ? value : byteSwap(value);
}

/** Serialises the unsigned integer T into `sizeof(T)` bytes in the requested order. */
template <typename T>
[[nodiscard]] inline std::array<char, sizeof(T)> toBytes(T value, bool little) noexcept
{
    const bool hostLittle = std::endian::native == std::endian::little;
    const T ordered = hostLittle == little ? value : byteSwap(value);
    std::array<char, sizeof(T)> bytes{};
    std::memcpy(bytes.data(), &ordered, sizeof(T));
    return bytes;
}

}  // namespace bedrock_protocol::encoding::detail
