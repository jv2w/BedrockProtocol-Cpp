/*
 * This file is part of BedrockProtocol for Endstone.
 * Internal helper with no PHP counterpart: LE and BE are two independent classes in pmmp/encoding,
 * but their members are identical apart from byte order, so the C++ port shares one template.
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <bit>
#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/Endian.h"

namespace bedrock_protocol::encoding::detail {

/**
 * Fixed-width integer and IEEE-754 float primitives in a fixed byte order.
 *
 * @tparam Little true for little-endian (LE), false for big-endian (BE)
 */
template <bool Little>
struct FixedWidthCodec final {
    FixedWidthCodec() = delete;

private:
    /** @throws DataDecodeException */
    template <typename T>
    static T read(ByteBufferReader &in)
    {
        return fromBytes<T>(in.readByteArray(sizeof(T)).data(), Little);
    }

    template <typename T>
    static void write(ByteBufferWriter &out, T value)
    {
        const auto bytes = toBytes<T>(value, Little);
        out.writeByteArray(std::string_view(bytes.data(), bytes.size()));
    }

public:
    /** @throws DataDecodeException */
    static std::uint16_t readUnsignedShort(ByteBufferReader &in) { return read<std::uint16_t>(in); }
    /** @throws DataDecodeException */
    static std::int16_t readSignedShort(ByteBufferReader &in) { return static_cast<std::int16_t>(read<std::uint16_t>(in)); }
    /** @throws DataDecodeException */
    static std::uint32_t readUnsignedInt(ByteBufferReader &in) { return read<std::uint32_t>(in); }
    /** @throws DataDecodeException */
    static std::int32_t readSignedInt(ByteBufferReader &in) { return static_cast<std::int32_t>(read<std::uint32_t>(in)); }
    /** @throws DataDecodeException */
    static std::uint64_t readUnsignedLong(ByteBufferReader &in) { return read<std::uint64_t>(in); }
    /** @throws DataDecodeException */
    static std::int64_t readSignedLong(ByteBufferReader &in) { return static_cast<std::int64_t>(read<std::uint64_t>(in)); }
    /** @throws DataDecodeException */
    static float readFloat(ByteBufferReader &in) { return std::bit_cast<float>(read<std::uint32_t>(in)); }
    /** @throws DataDecodeException */
    static double readDouble(ByteBufferReader &in) { return std::bit_cast<double>(read<std::uint64_t>(in)); }

    static void writeUnsignedShort(ByteBufferWriter &out, std::uint16_t value) { write<std::uint16_t>(out, value); }
    static void writeSignedShort(ByteBufferWriter &out, std::int16_t value) { write<std::uint16_t>(out, static_cast<std::uint16_t>(value)); }
    static void writeUnsignedInt(ByteBufferWriter &out, std::uint32_t value) { write<std::uint32_t>(out, value); }
    static void writeSignedInt(ByteBufferWriter &out, std::int32_t value) { write<std::uint32_t>(out, static_cast<std::uint32_t>(value)); }
    static void writeUnsignedLong(ByteBufferWriter &out, std::uint64_t value) { write<std::uint64_t>(out, value); }
    static void writeSignedLong(ByteBufferWriter &out, std::int64_t value) { write<std::uint64_t>(out, static_cast<std::uint64_t>(value)); }
    static void writeFloat(ByteBufferWriter &out, float value) { write<std::uint32_t>(out, std::bit_cast<std::uint32_t>(value)); }
    static void writeDouble(ByteBufferWriter &out, double value) { write<std::uint64_t>(out, std::bit_cast<std::uint64_t>(value)); }
};

}  // namespace bedrock_protocol::encoding::detail
