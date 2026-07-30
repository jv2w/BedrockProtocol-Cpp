/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/NetworkNbtSerializer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/serializer/NetworkNbtSerializer.h"

#include <cstddef>
#include <string>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/nbt/NbtDataException.h"

namespace bedrock_protocol::serializer {

using encoding::LE;
using encoding::VarInt;

std::uint16_t NetworkNbtSerializer::readShort()
{
    return LE::readUnsignedShort(*in_);
}

std::int16_t NetworkNbtSerializer::readSignedShort()
{
    return LE::readSignedShort(*in_);
}

void NetworkNbtSerializer::writeShort(std::uint16_t v)
{
    LE::writeUnsignedShort(*out_, v);
}

std::int32_t NetworkNbtSerializer::readInt()
{
    return VarInt::readSignedInt(*in_);
}

void NetworkNbtSerializer::writeInt(std::int32_t v)
{
    VarInt::writeSignedInt(*out_, v);
}

std::int64_t NetworkNbtSerializer::readLong()
{
    return VarInt::readSignedLong(*in_);
}

void NetworkNbtSerializer::writeLong(std::int64_t v)
{
    VarInt::writeSignedLong(*out_, v);
}

std::string NetworkNbtSerializer::readString()
{
    return std::string(in_->readByteArray(checkReadStringLength(VarInt::readUnsignedInt(*in_))));
}

void NetworkNbtSerializer::writeString(std::string_view v)
{
    VarInt::writeUnsignedInt(*out_, static_cast<std::uint32_t>(checkWriteStringLength(v.size())));
    out_->writeByteArray(v);
}

float NetworkNbtSerializer::readFloat()
{
    return LE::readFloat(*in_);
}

void NetworkNbtSerializer::writeFloat(float v)
{
    LE::writeFloat(*out_, v);
}

double NetworkNbtSerializer::readDouble()
{
    return LE::readDouble(*in_);
}

void NetworkNbtSerializer::writeDouble(double v)
{
    LE::writeDouble(*out_, v);
}

std::vector<std::int32_t> NetworkNbtSerializer::readIntArray()
{
    const auto len = readInt();  //varint
    if (len < 0) {
        throw nbt::NbtDataException("Array length cannot be less than zero (" + std::to_string(len) + " < 0)");
    }
    // No reserve() here: `len` is attacker-controlled (up to 2^31-1) and PHP does not pre-allocate, so
    // reserving up front turns a 5-byte payload into a multi-gigabyte allocation. Growing as we read
    // means the read fails on the truncated buffer long before the memory is committed.
    std::vector<std::int32_t> ret;
    for (std::int32_t i = 0; i < len; ++i) {
        ret.push_back(readInt());  //varint
    }

    return ret;
}

void NetworkNbtSerializer::writeIntArray(const std::vector<std::int32_t> &array)
{
    writeInt(static_cast<std::int32_t>(array.size()));  //varint
    for (const auto v : array) {
        writeInt(v);  //varint
    }
}

}  // namespace bedrock_protocol::serializer
