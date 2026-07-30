/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * Deterministic value source for the deep round-trip suite.
 *
 * The existing PacketRoundTripTest only ever encodes DEFAULT-constructed packets, where every field
 * is 0 / "" / nullopt. That hides three whole classes of defect, because the wire image of a wrong
 * field is identical to the wire image of the right one:
 *
 *   - two fields swapped in decodePayload    (0 == 0)
 *   - the wrong integer width                (a varint32 and a varint64 both encode 0 as one byte)
 *   - a field decodePayload never reads      (re-encoding the default writes the same bytes back)
 *
 * ValueWell exists to destroy those hiding places. Every value it hands out is non-zero, is unique
 * within the packet being filled, and is deliberately shaped so that a mistake changes the BYTES.
 * The rules below are not stylistic - each one exists to expose a specific defect class, and the
 * comment on each says which.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>

#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::verify {

/**
 * A splitmix64 stream that hands out distinctive, per-packet-unique field values.
 *
 * Reseeded per packet from (seed, pid), so any single packet reproduces in isolation: a failure can
 * be re-run on its own with the same bytes rather than requiring the whole suite.
 *
 * Uniqueness is ENFORCED, not merely observed. A draw that collides with one already handed out for
 * the current packet is redrawn. That is what makes a field swap visible: if two fields of a packet
 * can never hold the same value, exchanging them must change the encoding.
 */
class ValueWell {
public:
    explicit ValueWell(std::uint64_t seed) : seed_(seed) { beginPacket(0); }

    /** Reseeds for a new packet and clears the uniqueness pool. */
    void beginPacket(std::uint32_t pid);

    /** Number of draws that could not be made unique after retrying. Non-zero is a suite bug. */
    [[nodiscard]] int getDuplicateCount() const noexcept { return duplicates_; }

    // Integers. Each carries a WIDTH FLOOR: a varint32/varint64 or LE16/LE32 mix-up only changes the
    // byte count once the value is big enough to need the extra bytes. Small values would hide it.
    std::uint8_t u8();    ///< 1..255
    std::int8_t i8();     ///< non-zero, alternating sign
    std::uint16_t u16();  ///< >= 0x0100
    std::int16_t i16();   ///< magnitude >= 0x0100, alternating sign
    std::uint32_t u32();  ///< >= 0x0001'0000
    std::int32_t i32();   ///< magnitude >= 0x0001'0000, alternating sign - also exercises zigzag
    std::uint64_t u64();  ///< >= 0x0000'0001'0000'0000, so a 32-bit truncation cannot go unnoticed
    std::int64_t i64();   ///< magnitude >= 0x0000'0001'0000'0000, alternating sign

    /**
     * An exactly-representable float in +/-[1, 2).
     *
     * Exact representation means the round-trip is bit-precise with no epsilon fudge, and a
     * float/double confusion changes the encoded length rather than merely the low mantissa bits.
     */
    float f32();

    /** Alternates. Booleans cannot be made unique - a swap between two bool fields stays invisible. */
    bool flag();

    /**
     * A self-describing, non-ASCII string: "<label>#<n>" followed by U+2713.
     *
     * Self-describing so a hex dump names the field that produced it. Non-ASCII and variable-length
     * so that a wrong length prefix (fixed-width vs varint) or a UTF-8 mishandling shows up.
     */
    std::string str(std::string_view label);

    math::Vector3 vec3();
    math::Vector2 vec2();
    uuid::Uuid uuid();

    /** Wraps a value in an engaged optional, so the "present" branch of readOptional is exercised. */
    template <typename T>
    std::optional<T> some(T value)
    {
        return std::optional<T>(std::move(value));
    }

    /**
     * Marks a deliberately hardcoded value and excludes it from the uniqueness pool.
     *
     * Some fields are discriminators: they must be LEGAL, not unique, because the decoder branches on
     * them. Spelling them pin(X) keeps that decision visible in the filler source instead of leaving
     * an unexplained literal that looks like an oversight.
     */
    template <typename T>
    static T pin(T value)
    {
        return value;
    }

private:
    std::uint64_t next();
    /** Returns true if the token was not already handed out for this packet. */
    bool claim(std::uint64_t token);
    bool nextSign();

    std::uint64_t seed_;
    std::uint64_t state_ = 0;
    std::uint64_t counter_ = 0;
    std::unordered_set<std::uint64_t> used_;
    unsigned signFlip_ = 0;
    int duplicates_ = 0;
};

}  // namespace bedrock_protocol::verify
