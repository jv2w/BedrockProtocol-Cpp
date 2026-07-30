/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the subset of ramsey/uuid that BedrockProtocol relies on
 * (UuidInterface::getBytes/toString, Uuid::fromBytes/fromString/uuid4).
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bedrock_protocol::uuid {

/**
 * A 128-bit RFC 4122 UUID, stored as 16 raw bytes in network (big-endian) order.
 *
 * Note that the protocol does NOT put these bytes on the wire in this order — see
 * CommonTypes::getUUID/putUUID, which swap the two 8-byte halves.
 */
class Uuid {
public:
    /** Creates the nil UUID (all bits zero). */
    Uuid() = default;

    explicit Uuid(const std::array<std::uint8_t, 16> &bytes) : bytes_(bytes) {}

    /**
     * Creates a UUID from its 16-byte binary representation.
     *
     * @throws std::invalid_argument if the input is not exactly 16 bytes long
     */
    static Uuid fromBytes(std::string_view bytes);

    /**
     * Creates a UUID from its canonical 8-4-4-4-12 hexadecimal representation.
     *
     * @throws std::invalid_argument if the input is not a well-formed UUID string
     */
    static Uuid fromString(std::string_view string);

    /** Generates a random (version 4) UUID. */
    static Uuid uuid4();

    /** Returns the 16-byte binary representation. */
    [[nodiscard]] std::string getBytes() const;

    /** Returns the canonical lowercase 8-4-4-4-12 hexadecimal representation. */
    [[nodiscard]] std::string toString() const;

    [[nodiscard]] const std::array<std::uint8_t, 16> &getRawBytes() const noexcept { return bytes_; }

    [[nodiscard]] bool equals(const Uuid &other) const { return bytes_ == other.bytes_; }

    friend bool operator==(const Uuid &lhs, const Uuid &rhs) { return lhs.equals(rhs); }
    friend bool operator!=(const Uuid &lhs, const Uuid &rhs) { return !lhs.equals(rhs); }

private:
    std::array<std::uint8_t, 16> bytes_{};
};

}  // namespace bedrock_protocol::uuid
