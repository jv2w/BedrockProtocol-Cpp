/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/NbtStreamWriter.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace bedrock_protocol::nbt {

/**
 * @internal
 */
class NbtStreamWriter {
public:
    virtual ~NbtStreamWriter() = default;

    virtual void writeByte(std::uint8_t v) = 0;

    virtual void writeShort(std::uint16_t v) = 0;

    virtual void writeInt(std::int32_t v) = 0;

    virtual void writeLong(std::int64_t v) = 0;

    virtual void writeFloat(float v) = 0;

    virtual void writeDouble(double v) = 0;

    virtual void writeByteArray(std::string_view v) = 0;

    /**
     * @throws InvalidTagValueException if the string is too long
     */
    virtual void writeString(std::string_view v) = 0;

    virtual void writeIntArray(const std::vector<std::int32_t> &array) = 0;
};

}  // namespace bedrock_protocol::nbt
