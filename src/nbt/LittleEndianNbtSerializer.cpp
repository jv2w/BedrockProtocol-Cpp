/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/LittleEndianNbtSerializer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/nbt/LittleEndianNbtSerializer.h"

#include <algorithm>
#include <cstddef>
#include <string>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/nbt/NbtDataException.h"

namespace bedrock_protocol::nbt {

using encoding::LE;

std::uint16_t LittleEndianNbtSerializer::readShort()
{
    return LE::readUnsignedShort(*in_);
}

std::int16_t LittleEndianNbtSerializer::readSignedShort()
{
    return LE::readSignedShort(*in_);
}

void LittleEndianNbtSerializer::writeShort(std::uint16_t v)
{
    LE::writeUnsignedShort(*out_, v);
}

std::int32_t LittleEndianNbtSerializer::readInt()
{
    return LE::readSignedInt(*in_);
}

void LittleEndianNbtSerializer::writeInt(std::int32_t v)
{
    LE::writeSignedInt(*out_, v);
}

std::int64_t LittleEndianNbtSerializer::readLong()
{
    return LE::readSignedLong(*in_);
}

void LittleEndianNbtSerializer::writeLong(std::int64_t v)
{
    LE::writeSignedLong(*out_, v);
}

float LittleEndianNbtSerializer::readFloat()
{
    return LE::readFloat(*in_);
}

void LittleEndianNbtSerializer::writeFloat(float v)
{
    LE::writeFloat(*out_, v);
}

double LittleEndianNbtSerializer::readDouble()
{
    return LE::readDouble(*in_);
}

void LittleEndianNbtSerializer::writeDouble(double v)
{
    LE::writeDouble(*out_, v);
}

std::vector<std::int32_t> LittleEndianNbtSerializer::readIntArray()
{
    const auto len = readInt();
    if (len < 0) {
        throw NbtDataException("Array length cannot be less than zero (" + std::to_string(len) + " < 0)");
    }
    //PHP unpacks the whole run in one go with unpack("V*", ...); doing it element-wise here is equivalent.
    // The reserve is capped by what the buffer can actually supply. `len` is attacker-controlled (this
    // path is reachable from the wire through ItemStackExtraData) and PHP does not pre-allocate at all,
    // so reserving on `len` alone turned a 6-byte TAG_Int_Array into an ~8 GiB request. Each element is
    // exactly four bytes, which makes the unread length a sound upper bound.
    std::vector<std::int32_t> unpacked;
    unpacked.reserve(std::min(static_cast<std::size_t>(len), in_->getUnreadLength() / 4));
    for (std::int32_t i = 0; i < len; ++i) {
        unpacked.push_back(LE::readSignedInt(*in_));
    }
    return unpacked;
}

void LittleEndianNbtSerializer::writeIntArray(const std::vector<std::int32_t> &array)
{
    writeInt(static_cast<std::int32_t>(array.size()));
    for (const auto v : array) {
        LE::writeSignedInt(*out_, v);
    }
}

}  // namespace bedrock_protocol::nbt
