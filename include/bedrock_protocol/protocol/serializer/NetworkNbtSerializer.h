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

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/nbt/BaseNbtSerializer.h"

namespace bedrock_protocol::serializer {

class NetworkNbtSerializer : public nbt::BaseNbtSerializer {
public:
    std::uint16_t readShort() override;

    std::int16_t readSignedShort() override;

    void writeShort(std::uint16_t v) override;

    std::int32_t readInt() override;

    void writeInt(std::int32_t v) override;

    std::int64_t readLong() override;

    void writeLong(std::int64_t v) override;

    std::string readString() override;

    void writeString(std::string_view v) override;

    float readFloat() override;

    void writeFloat(float v) override;

    double readDouble() override;

    void writeDouble(double v) override;

    std::vector<std::int32_t> readIntArray() override;

    void writeIntArray(const std::vector<std::int32_t> &array) override;
};

}  // namespace bedrock_protocol::serializer
