/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the subset of ramsey/uuid that BedrockProtocol relies on.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/uuid/Uuid.h"

#include <random>

namespace bedrock_protocol::uuid {
namespace {

constexpr char HEX_DIGITS[] = "0123456789abcdef";

/** Positions of the four hyphens in the canonical representation. */
constexpr std::size_t HYPHEN_POSITIONS[] = {8, 13, 18, 23};

int hexValue(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1;
}

}  // namespace

Uuid Uuid::fromBytes(std::string_view bytes)
{
    if (bytes.size() != 16) {
        throw std::invalid_argument("UUID must be exactly 16 bytes, got " + std::to_string(bytes.size()));
    }
    std::array<std::uint8_t, 16> raw{};
    for (std::size_t i = 0; i < 16; ++i) {
        raw[i] = static_cast<std::uint8_t>(bytes[i]);
    }
    return Uuid(raw);
}

Uuid Uuid::fromString(std::string_view string)
{
    if (string.size() != 36) {
        throw std::invalid_argument("Malformed UUID string: expected 36 characters, got " +
                                    std::to_string(string.size()));
    }
    for (const auto position : HYPHEN_POSITIONS) {
        if (string[position] != '-') {
            throw std::invalid_argument("Malformed UUID string: expected '-' at offset " + std::to_string(position));
        }
    }

    std::array<std::uint8_t, 16> raw{};
    std::size_t byteIndex = 0;
    for (std::size_t i = 0; i < string.size(); ++i) {
        if (string[i] == '-') {
            continue;
        }
        const int high = hexValue(string[i]);
        const int low = hexValue(string[++i]);
        if (high < 0 || low < 0) {
            throw std::invalid_argument("Malformed UUID string: not a hexadecimal digit");
        }
        raw[byteIndex++] = static_cast<std::uint8_t>((high << 4) | low);
    }
    return Uuid(raw);
}

Uuid Uuid::uuid4()
{
    static thread_local std::mt19937_64 engine{std::random_device{}()};
    std::uniform_int_distribution<std::uint64_t> distribution;

    std::array<std::uint8_t, 16> raw{};
    for (std::size_t half = 0; half < 2; ++half) {
        auto value = distribution(engine);
        for (std::size_t i = 0; i < 8; ++i) {
            raw[half * 8 + i] = static_cast<std::uint8_t>(value & 0xff);
            value >>= 8;
        }
    }
    raw[6] = static_cast<std::uint8_t>((raw[6] & 0x0f) | 0x40);  // version 4
    raw[8] = static_cast<std::uint8_t>((raw[8] & 0x3f) | 0x80);  // RFC 4122 variant
    return Uuid(raw);
}

std::string Uuid::getBytes() const
{
    std::string result(16, '\0');
    for (std::size_t i = 0; i < 16; ++i) {
        result[i] = static_cast<char>(bytes_[i]);
    }
    return result;
}

std::string Uuid::toString() const
{
    std::string result;
    result.reserve(36);
    for (std::size_t i = 0; i < 16; ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            result.push_back('-');
        }
        result.push_back(HEX_DIGITS[bytes_[i] >> 4]);
        result.push_back(HEX_DIGITS[bytes_[i] & 0x0f]);
    }
    return result;
}

}  // namespace bedrock_protocol::uuid
