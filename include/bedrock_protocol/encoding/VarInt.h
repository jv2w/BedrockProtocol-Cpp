/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pmmp/encoding (VarInt)
 * Reference semantics: pocketmine/binaryutils src/Binary.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <stdexcept>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/DataDecodeException.h"

namespace bedrock_protocol::encoding {

/**
 * Base-128 variable-length integer primitives.
 *
 * Signed variants use ZigZag encoding, which maps small-magnitude negative numbers onto small
 * unsigned values so that they stay short on the wire.
 */
struct VarInt final {
    VarInt() = delete;

    /**
     * Reads a 32-bit unsigned VarInt.
     *
     * @throws DataDecodeException if the buffer runs out, or the value did not terminate after 5 bytes
     */
    static std::uint32_t readUnsignedInt(ByteBufferReader &in)
    {
        std::uint32_t value = 0;
        for (int shift = 0; shift <= 28; shift += 7) {
            const auto b = Byte::readUnsigned(in);
            value |= static_cast<std::uint32_t>(b & 0x7f) << shift;
            if ((b & 0x80) == 0) {
                return value;
            }
        }
        throw DataDecodeException("VarInt did not terminate after 5 bytes!");
    }

    /**
     * Reads a 32-bit signed (ZigZag) VarInt.
     *
     * @throws DataDecodeException
     */
    static std::int32_t readSignedInt(ByteBufferReader &in)
    {
        const auto raw = readUnsignedInt(in);
        return static_cast<std::int32_t>((raw >> 1) ^ (~(raw & 1) + 1));
    }

    /**
     * Reads a 64-bit unsigned VarInt.
     *
     * @throws DataDecodeException if the buffer runs out, or the value did not terminate after 10 bytes
     */
    static std::uint64_t readUnsignedLong(ByteBufferReader &in)
    {
        std::uint64_t value = 0;
        for (int shift = 0; shift <= 63; shift += 7) {
            const auto b = Byte::readUnsigned(in);
            value |= static_cast<std::uint64_t>(b & 0x7f) << shift;
            if ((b & 0x80) == 0) {
                return value;
            }
        }
        throw DataDecodeException("VarLong did not terminate after 10 bytes!");
    }

    /**
     * Reads a 64-bit signed (ZigZag) VarInt.
     *
     * @throws DataDecodeException
     */
    static std::int64_t readSignedLong(ByteBufferReader &in)
    {
        const auto raw = readUnsignedLong(in);
        return static_cast<std::int64_t>((raw >> 1) ^ (~(raw & 1) + 1));
    }

    static void writeUnsignedInt(ByteBufferWriter &out, std::uint32_t value)
    {
        for (int i = 0; i < 5; ++i) {
            const auto bits = static_cast<std::uint8_t>(value & 0x7f);
            value >>= 7;
            if (value != 0) {
                Byte::writeUnsigned(out, static_cast<std::uint8_t>(bits | 0x80));
            }
            else {
                Byte::writeUnsigned(out, bits);
                return;
            }
        }
    }

    static void writeSignedInt(ByteBufferWriter &out, std::int32_t value)
    {
        const auto raw = static_cast<std::uint32_t>(value);
        writeUnsignedInt(out, (raw << 1) ^ static_cast<std::uint32_t>(value >> 31));
    }

    static void writeUnsignedLong(ByteBufferWriter &out, std::uint64_t value)
    {
        for (int i = 0; i < 10; ++i) {
            const auto bits = static_cast<std::uint8_t>(value & 0x7f);
            value >>= 7;
            if (value != 0) {
                Byte::writeUnsigned(out, static_cast<std::uint8_t>(bits | 0x80));
            }
            else {
                Byte::writeUnsigned(out, bits);
                return;
            }
        }
    }

    static void writeSignedLong(ByteBufferWriter &out, std::int64_t value)
    {
        const auto raw = static_cast<std::uint64_t>(value);
        writeUnsignedLong(out, (raw << 1) ^ static_cast<std::uint64_t>(value >> 63));
    }
};

}  // namespace bedrock_protocol::encoding
