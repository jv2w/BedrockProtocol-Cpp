/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/CommandParameterRawData.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::command::raw {

class CommandParameterRawData final {
public:
    CommandParameterRawData(std::string name, std::uint32_t typeInfo, bool optional, std::uint8_t flags)
        : name(std::move(name)), typeInfo(typeInfo), optional(optional), flags(flags)
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] std::uint32_t getTypeInfo() const
    {
        return typeInfo;
    }

    [[nodiscard]] bool isOptional() const
    {
        return optional;
    }

    [[nodiscard]] std::uint8_t getFlags() const
    {
        return flags;
    }

    static CommandParameterRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::uint32_t typeInfo;
    bool optional;
    std::uint8_t flags;
};

}  // namespace bedrock_protocol::types::command::raw
