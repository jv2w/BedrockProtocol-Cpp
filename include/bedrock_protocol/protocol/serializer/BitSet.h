/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/BitSet.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::serializer {

/**
 * Variable-length bitset.
 *
 * @note PHP stores the parts as native `int` (signed 64-bit). The C++ port uses `std::uint64_t` so
 * that the shifts used by read()/write() are well-defined; only the low 7 bits of each shifted part
 * ever reach the wire, so the encoded bytes are identical either way.
 */
class BitSet {
public:
    /**
     * @note PHP has no default constructor; C++ needs one so that BitSet can be a packet field.
     * A default-constructed BitSet has length 0 and no parts.
     */
    BitSet() = default;

    /**
     * @param parts
     */
    explicit BitSet(std::int32_t length, std::vector<std::uint64_t> parts = {});

    [[nodiscard]] bool get(std::int32_t index) const;

    void set(std::int32_t index, bool value);

    /**
     * @internal
     */
    [[nodiscard]] std::int32_t getPartsCount() const
    {
        return static_cast<std::int32_t>(parts.size());
    }

    /**
     * @throws DataDecodeException
     */
    static BitSet read(encoding::ByteBufferReader &in, std::int32_t length);

    void write(encoding::ByteBufferWriter &out) const;

    [[nodiscard]] std::int32_t getLength() const
    {
        return length;
    }

    [[nodiscard]] bool equals(const BitSet &that) const
    {
        return length == that.length && parts == that.parts;
    }

    [[nodiscard]] bool operator==(const BitSet &that) const
    {
        return equals(that);
    }

    [[nodiscard]] bool operator!=(const BitSet &that) const
    {
        return !equals(that);
    }

private:
    static constexpr std::int32_t INT_BITS = 64; // PHP: PHP_INT_SIZE * 8
    static constexpr std::int32_t SHIFT = 7;

    /**
     * Returns the part index and the bit index within that part for a given bit index.
     */
    [[nodiscard]] std::pair<std::int32_t, std::int32_t> getPartIndex(std::int32_t index) const;

    static std::int32_t getExpectedPartsCount(std::int32_t length);

    std::int32_t length = 0;
    std::vector<std::uint64_t> parts;
};

}  // namespace bedrock_protocol::serializer
