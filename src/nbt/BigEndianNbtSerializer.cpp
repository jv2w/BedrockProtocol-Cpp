/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/BigEndianNbtSerializer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/BigEndianNbtSerializer.h"

#include <algorithm>
#include <cstddef>
#include <string>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/nbt/NbtDataException.h"

namespace bedrock_protocol::nbt {

using encoding::BE;

std::uint16_t BigEndianNbtSerializer::readShort()
{
    return BE::readUnsignedShort(*in_);
}

std::int16_t BigEndianNbtSerializer::readSignedShort()
{
    return BE::readSignedShort(*in_);
}

void BigEndianNbtSerializer::writeShort(std::uint16_t v)
{
    BE::writeUnsignedShort(*out_, v);
}

std::int32_t BigEndianNbtSerializer::readInt()
{
    return BE::readSignedInt(*in_);
}

void BigEndianNbtSerializer::writeInt(std::int32_t v)
{
    BE::writeSignedInt(*out_, v);
}

std::int64_t BigEndianNbtSerializer::readLong()
{
    return BE::readSignedLong(*in_);
}

void BigEndianNbtSerializer::writeLong(std::int64_t v)
{
    BE::writeSignedLong(*out_, v);
}

float BigEndianNbtSerializer::readFloat()
{
    return BE::readFloat(*in_);
}

void BigEndianNbtSerializer::writeFloat(float v)
{
    BE::writeFloat(*out_, v);
}

double BigEndianNbtSerializer::readDouble()
{
    return BE::readDouble(*in_);
}

void BigEndianNbtSerializer::writeDouble(double v)
{
    BE::writeDouble(*out_, v);
}

std::vector<std::int32_t> BigEndianNbtSerializer::readIntArray()
{
    const auto len = readInt();
    if (len < 0) {
        throw NbtDataException("Array length cannot be less than zero (" + std::to_string(len) + " < 0)");
    }
    //PHP unpacks the whole run in one go with unpack("N*", ...); doing it element-wise here is equivalent.
    // Capped by what the buffer can actually supply, for the same reason as the little-endian
    // serializer: `len` is attacker-controlled and PHP does not pre-allocate. Each element is exactly
    // four bytes, so the unread length is a sound upper bound.
    std::vector<std::int32_t> unpacked;
    unpacked.reserve(std::min(static_cast<std::size_t>(len), in_->getUnreadLength() / 4));
    for (std::int32_t i = 0; i < len; ++i) {
        unpacked.push_back(BE::readSignedInt(*in_));
    }
    return unpacked;
}

void BigEndianNbtSerializer::writeIntArray(const std::vector<std::int32_t> &array)
{
    writeInt(static_cast<std::int32_t>(array.size()));
    for (const auto v : array) {
        BE::writeSignedInt(*out_, v);
    }
}

}  // namespace bedrock_protocol::nbt
