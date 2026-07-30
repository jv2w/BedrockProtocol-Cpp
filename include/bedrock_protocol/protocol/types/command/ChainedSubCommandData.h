/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/ChainedSubCommandData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/types/command/ChainedSubCommandValue.h"

namespace bedrock_protocol::types::command {

class ChainedSubCommandData final {
public:
    /**
     * @param values
     */
    ChainedSubCommandData(std::string name, std::vector<ChainedSubCommandValue> values)
        : name(std::move(name)), values(std::move(values))
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::vector<ChainedSubCommandValue> &getValues() const
    {
        return values;
    }

private:
    std::string name;
    std::vector<ChainedSubCommandValue> values;
};

}  // namespace bedrock_protocol::types::command
