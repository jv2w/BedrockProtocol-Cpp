/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/CommandRawData.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandOverloadRawData.h"

namespace bedrock_protocol::types::command::raw {

class CommandRawData final {
public:
    /**
     * @param chainedSubCommandDataIndexes
     * @param overloads
     */
    CommandRawData(std::string name, std::string description, std::uint16_t flags, std::string permission,
                   std::int32_t aliasEnumIndex, std::vector<std::uint32_t> chainedSubCommandDataIndexes,
                   std::vector<CommandOverloadRawData> overloads)
        : name(std::move(name)), description(std::move(description)), flags(flags), permission(std::move(permission)),
          aliasEnumIndex(aliasEnumIndex), chainedSubCommandDataIndexes(std::move(chainedSubCommandDataIndexes)),
          overloads(std::move(overloads))
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::string &getDescription() const
    {
        return description;
    }

    [[nodiscard]] std::uint16_t getFlags() const
    {
        return flags;
    }

    [[nodiscard]] const std::string &getPermission() const
    {
        return permission;
    }

    [[nodiscard]] std::int32_t getAliasEnumIndex() const
    {
        return aliasEnumIndex;
    }

    [[nodiscard]] const std::vector<std::uint32_t> &getChainedSubCommandDataIndexes() const
    {
        return chainedSubCommandDataIndexes;
    }

    [[nodiscard]] const std::vector<CommandOverloadRawData> &getOverloads() const
    {
        return overloads;
    }

    static CommandRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::string description;
    std::uint16_t flags;
    std::string permission;
    std::int32_t aliasEnumIndex;
    std::vector<std::uint32_t> chainedSubCommandDataIndexes;
    std::vector<CommandOverloadRawData> overloads;
};

}  // namespace bedrock_protocol::types::command::raw
