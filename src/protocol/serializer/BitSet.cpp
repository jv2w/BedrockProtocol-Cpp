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

#include "bedrock_protocol/protocol/serializer/BitSet.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/Byte.h"

namespace bedrock_protocol::serializer {

using encoding::Byte;

BitSet::BitSet(std::int32_t length, std::vector<std::uint64_t> parts) : length(length)
{
    const auto expectedPartsCount = getExpectedPartsCount(length);
    const auto partsCount = static_cast<std::int32_t>(parts.size());

    if (partsCount > expectedPartsCount) {
        throw std::invalid_argument("Too many parts");
    }
    else if (partsCount < expectedPartsCount) {
        parts.resize(static_cast<std::size_t>(expectedPartsCount), 0);
    }

    this->parts = std::move(parts);
}

bool BitSet::get(std::int32_t index) const
{
    const auto [partIndex, bitIndex] = getPartIndex(index);

    return (parts[static_cast<std::size_t>(partIndex)] & (std::uint64_t{1} << bitIndex)) != 0;
}

void BitSet::set(std::int32_t index, bool value)
{
    const auto [partIndex, bitIndex] = getPartIndex(index);

    if (value) {
        parts[static_cast<std::size_t>(partIndex)] |= std::uint64_t{1} << bitIndex;
    }
    else {
        parts[static_cast<std::size_t>(partIndex)] &= ~(std::uint64_t{1} << bitIndex);
    }
}

std::pair<std::int32_t, std::int32_t> BitSet::getPartIndex(std::int32_t index) const
{
    if (index < 0 || index >= length) {
        throw std::invalid_argument("Index out of bounds");
    }

    return {
        index / INT_BITS,
        index % INT_BITS
    };
}

std::int32_t BitSet::getExpectedPartsCount(std::int32_t length)
{
    return (length + INT_BITS - 1) / INT_BITS;
}

BitSet BitSet::read(encoding::ByteBufferReader &in, std::int32_t length)
{
    std::vector<std::uint64_t> result{0};

    std::int32_t currentIndex = 0;
    std::int32_t currentShift = 0;

    for (std::int32_t i = 0; i < length; i += SHIFT) {
        const auto b = Byte::readUnsigned(in);
        const std::uint64_t bits = b & 0x7f;

        result[static_cast<std::size_t>(currentIndex)] |= bits << currentShift; //extra bits will be discarded
        auto nextShift = currentShift + SHIFT;
        if (nextShift >= INT_BITS) {
            nextShift -= INT_BITS;
            const auto rightShift = SHIFT - nextShift;
            ++currentIndex;
            result.resize(static_cast<std::size_t>(currentIndex) + 1, 0);
            result[static_cast<std::size_t>(currentIndex)] = bits >> rightShift;
        }
        currentShift = nextShift;

        if ((b & 0x80) == 0) {
            result.resize(std::min<std::size_t>(result.size(), static_cast<std::size_t>(getExpectedPartsCount(length))));
            return BitSet(length, std::move(result));
        }
    }

    result.resize(std::min<std::size_t>(result.size(), static_cast<std::size_t>(getExpectedPartsCount(length))));
    return BitSet(length, std::move(result));
}

void BitSet::write(encoding::ByteBufferWriter &out) const
{
    const auto &parts = this->parts;
    const auto length = this->length;

    std::int32_t currentIndex = 0;
    std::int32_t currentShift = 0;

    for (std::int32_t i = 0; i < length; i += SHIFT) {
        // Deliberate divergence from PHP: PHP's `$parts[$currentIndex] >> $currentShift` is an
        // arithmetic shift on a signed 64-bit int, so when bit 63 of the part is set (e.g. the
        // SNEAK_PRESSED_RAW auth-input flag) it sign-extends and the resulting all-ones low bits swallow
        // the contribution of the next part. This port keeps the parts unsigned so the shift is logical,
        // which is what the client expects and what BitSet::read() above produces.
        std::uint64_t bits = parts[static_cast<std::size_t>(currentIndex)] >> currentShift;
        auto nextShift = currentShift + SHIFT;
        if (nextShift >= INT_BITS) {
            nextShift -= INT_BITS;
            ++currentIndex;
            const std::uint64_t nextPart = static_cast<std::size_t>(currentIndex) < parts.size()
                                               ? parts[static_cast<std::size_t>(currentIndex)]
                                               : 0;
            bits |= nextPart << (SHIFT - nextShift);
        }
        currentShift = nextShift;

        const bool last = i + SHIFT >= length;
        bits |= last ? 0 : 0x80;

        Byte::writeUnsigned(out, static_cast<std::uint8_t>(bits));
        if (last) {
            break;
        }
    }
}

}  // namespace bedrock_protocol::serializer
