/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pmmp/encoding (Byte)
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::encoding {

/** Single-byte primitives. All members are static; this type is never instantiated. */
struct Byte final {
    Byte() = delete;

    /** @throws DataDecodeException */
    static std::uint8_t readUnsigned(ByteBufferReader &in)
    {
        return static_cast<std::uint8_t>(in.readByteArray(1)[0]);
    }

    /** @throws DataDecodeException */
    static std::int8_t readSigned(ByteBufferReader &in)
    {
        return static_cast<std::int8_t>(readUnsigned(in));
    }

    static void writeUnsigned(ByteBufferWriter &out, std::uint8_t value)
    {
        const auto byte = static_cast<char>(value);
        out.writeByteArray(std::string_view(&byte, 1));
    }

    static void writeSigned(ByteBufferWriter &out, std::int8_t value)
    {
        writeUnsigned(out, static_cast<std::uint8_t>(value));
    }
};

}  // namespace bedrock_protocol::encoding
