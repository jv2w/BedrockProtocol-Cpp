/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock_protocol/protocol/types/command/ChainedSubCommandData.h"
#include "bedrock_protocol/protocol/types/command/CommandHardEnum.h"
#include "bedrock_protocol/protocol/types/command/CommandOverload.h"

namespace bedrock_protocol::types::command {

class CommandData {
public:
    /**
     * @param overloads
     * @param chainedSubCommandData
     * @note PHP takes `int|string $permission`; C++ splits this into two constructor overloads.
     */
    CommandData(std::string name, std::string description, std::uint16_t flags, std::int32_t permission,
                std::optional<CommandHardEnum> aliases, std::vector<CommandOverload> overloads,
                std::vector<ChainedSubCommandData> chainedSubCommandData);

    /**
     * @param overloads
     * @param chainedSubCommandData
     */
    CommandData(std::string name, std::string description, std::uint16_t flags, std::string permission,
                std::optional<CommandHardEnum> aliases, std::vector<CommandOverload> overloads,
                std::vector<ChainedSubCommandData> chainedSubCommandData);

    virtual ~CommandData() = default;

    std::string name;
    std::string description;
    std::uint16_t flags;
    std::optional<CommandHardEnum> aliases;
    std::vector<CommandOverload> overloads;
    std::vector<ChainedSubCommandData> chainedSubCommandData;

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

    [[nodiscard]] const std::optional<CommandHardEnum> &getAliases() const
    {
        return aliases;
    }

    [[nodiscard]] const std::vector<CommandOverload> &getOverloads() const
    {
        return overloads;
    }

    [[nodiscard]] const std::vector<ChainedSubCommandData> &getChainedSubCommandData() const
    {
        return chainedSubCommandData;
    }

private:
    std::string permission;
};

}  // namespace bedrock_protocol::types::command
