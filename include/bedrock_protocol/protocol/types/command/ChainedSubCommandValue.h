/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/ChainedSubCommandValue.php
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

namespace bedrock_protocol::types::command {

class ChainedSubCommandValue final {
public:
    ChainedSubCommandValue(std::string name, std::uint32_t type) : name(std::move(name)), type(type) {}

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    /**
     * @see AvailableCommandsPacket::ARG_TYPE_*
     */
    [[nodiscard]] std::uint32_t getType() const
    {
        return type;
    }

private:
    std::string name;
    std::uint32_t type;
};

}  // namespace bedrock_protocol::types::command
