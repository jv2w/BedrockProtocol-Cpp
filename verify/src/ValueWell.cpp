/*
 * This file is part of BedrockProtocol for Endstone.
 */

#include "bedrock_protocol/verify/ValueWell.h"

#include <array>
#include <cstring>

namespace bedrock_protocol::verify {

namespace {

/** Retries before a draw is declared un-uniquable. Collisions are rare; 64 is far past pathological. */
constexpr int MAX_UNIQUE_ATTEMPTS = 64;

/** Widens any signed value to a token that cannot collide with the same magnitude of opposite sign. */
std::uint64_t signedToken(std::int64_t value)
{
    return static_cast<std::uint64_t>(value);
}

std::uint64_t floatToken(float value)
{
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    // Tagged so a float never collides with the integer of the same bit pattern.
    return 0xF10A'7000'0000'0000ULL | bits;
}

std::uint64_t stringToken(std::string_view value)
{
    // FNV-1a. Only needs to separate the handful of strings in one packet, not to resist collisions.
    std::uint64_t hash = 0xcbf29ce484222325ULL;
    for (const char c : value) {
        hash ^= static_cast<unsigned char>(c);
        hash *= 0x100000001b3ULL;
    }
    return 0x5747'0000'0000'0000ULL | (hash >> 16);
}

}  // namespace

void ValueWell::beginPacket(std::uint32_t pid)
{
    // Mixing the pid into the seed makes each packet's byte image independent of the order the suite
    // happens to run in, so a single failing packet reproduces on its own.
    std::uint64_t mixed = seed_ ^ (static_cast<std::uint64_t>(pid) * 0x9E3779B97F4A7C15ULL);
    mixed ^= mixed >> 33;
    mixed *= 0xFF51AFD7ED558CCDULL;
    state_ = mixed ^ (mixed >> 29);

    used_.clear();
    counter_ = 0;
    signFlip_ = 0;
    // duplicates_ deliberately accumulates across packets: it is a suite-wide health counter.
}

std::uint64_t ValueWell::next()
{
    state_ += 0x9E3779B97F4A7C15ULL;
    std::uint64_t z = state_;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

bool ValueWell::claim(std::uint64_t token)
{
    return used_.insert(token).second;
}

bool ValueWell::nextSign()
{
    return (signFlip_++ & 1u) == 0u;
}

std::uint8_t ValueWell::u8()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        const auto value = static_cast<std::uint8_t>(next() % 255u + 1u);  // 1..255, never 0
        if (claim(value)) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::uint8_t>(next() % 255u + 1u);
}

std::int8_t ValueWell::i8()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        const auto magnitude = static_cast<std::int8_t>(next() % 127u + 1u);
        const std::int8_t value = nextSign() ? magnitude : static_cast<std::int8_t>(-magnitude);
        if (claim(signedToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::int8_t>(next() % 127u + 1u);
}

std::uint16_t ValueWell::u16()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        // >= 0x0100 so a byte-vs-short mistake cannot round-trip by accident.
        const auto value = static_cast<std::uint16_t>((next() & 0xFFFFu) | 0x0100u);
        if (claim(value)) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::uint16_t>((next() & 0xFFFFu) | 0x0100u);
}

std::int16_t ValueWell::i16()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        const auto magnitude = static_cast<std::int16_t>((next() & 0x3FFFu) | 0x0100u);
        const std::int16_t value = nextSign() ? magnitude : static_cast<std::int16_t>(-magnitude);
        if (claim(signedToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::int16_t>((next() & 0x3FFFu) | 0x0100u);
}

std::uint32_t ValueWell::u32()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        // >= 0x0001'0000 forces a 3-byte varint minimum: a varint/LE mix-up changes the length.
        const auto value = static_cast<std::uint32_t>((next() & 0x7FFFFFFFu) | 0x00010000u);
        if (claim(value)) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::uint32_t>((next() & 0x7FFFFFFFu) | 0x00010000u);
}

std::int32_t ValueWell::i32()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        const auto magnitude = static_cast<std::int32_t>((next() & 0x3FFFFFFFu) | 0x00010000u);
        // Alternating sign is what separates a zigzag varint from a plain one: they agree on
        // non-negative values and diverge on negative ones.
        const std::int32_t value = nextSign() ? magnitude : -magnitude;
        if (claim(signedToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::int32_t>((next() & 0x3FFFFFFFu) | 0x00010000u);
}

std::uint64_t ValueWell::u64()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        // Above 2^32, so silently truncating to 32 bits cannot survive the round-trip.
        const std::uint64_t value = (next() & 0x7FFF'FFFF'FFFFULL) | 0x0000'0001'0000'0000ULL;
        if (claim(value)) {
            return value;
        }
    }
    ++duplicates_;
    return (next() & 0x7FFF'FFFF'FFFFULL) | 0x0000'0001'0000'0000ULL;
}

std::int64_t ValueWell::i64()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        const auto magnitude = static_cast<std::int64_t>((next() & 0x3FFF'FFFF'FFFFULL) | 0x0000'0001'0000'0000ULL);
        const std::int64_t value = nextSign() ? magnitude : -magnitude;
        if (claim(signedToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return static_cast<std::int64_t>((next() & 0x3FFF'FFFF'FFFFULL) | 0x0000'0001'0000'0000ULL);
}

float ValueWell::f32()
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        // 1 + n/32768 is exact in binary32 for every n in [0, 32768): the mantissa has room to spare,
        // so the round-trip is bit-precise and needs no epsilon comparison.
        const auto steps = static_cast<std::uint32_t>(next() % 32768u);
        const float magnitude = 1.0F + static_cast<float>(steps) / 32768.0F;
        const float value = nextSign() ? magnitude : -magnitude;
        if (claim(floatToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return 1.5F;
}

bool ValueWell::flag()
{
    // Not claimed: with only two values, uniqueness is impossible. A swap between two bool fields is
    // the one field-order defect this suite cannot see, and that limitation is stated in the report.
    return (next() & 1u) != 0u;
}

std::string ValueWell::str(std::string_view label)
{
    for (int attempt = 0; attempt < MAX_UNIQUE_ATTEMPTS; ++attempt) {
        std::string value;
        value.reserve(label.size() + 12);
        value.append(label);
        value.push_back('#');
        value.append(std::to_string(next() % 100000u));
        // U+2713 CHECK MARK. A multi-byte code point makes a byte-length/char-length confusion in the
        // length prefix visible, which a pure ASCII string would not.
        value.append("\xE2\x9C\x93");
        if (claim(stringToken(value))) {
            return value;
        }
    }
    ++duplicates_;
    return std::string(label) + "#dup\xE2\x9C\x93";
}

math::Vector3 ValueWell::vec3()
{
    const float x = f32();
    const float y = f32();
    const float z = f32();
    return {x, y, z};
}

math::Vector2 ValueWell::vec2()
{
    const float x = f32();
    const float y = f32();
    return {x, y};
}

uuid::Uuid ValueWell::uuid()
{
    std::array<std::uint8_t, 16> bytes{};
    for (std::size_t i = 0; i < bytes.size(); i += 8) {
        const std::uint64_t chunk = next();
        for (std::size_t j = 0; j < 8; ++j) {
            bytes[i + j] = static_cast<std::uint8_t>(chunk >> (j * 8));
        }
    }
    // Not version-tagged on purpose: the wire format stores 16 raw bytes and must not reinterpret
    // them, so an arbitrary pattern is the stricter test.
    claim(0xDD1D'0000'0000'0000ULL | (static_cast<std::uint64_t>(bytes[0]) << 8) | bytes[15]);
    return uuid::Uuid(bytes);
}

}  // namespace bedrock_protocol::verify
