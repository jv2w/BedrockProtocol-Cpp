/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandOverload.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/types/command/CommandParameter.h"

namespace bedrock_protocol::types::command {

class CommandOverload final {
public:
    /**
     * @param parameters
     */
    CommandOverload(bool chaining, std::vector<CommandParameter> parameters)
        : chaining(chaining), parameters(std::move(parameters))
    {
    }

    [[nodiscard]] bool isChaining() const
    {
        return chaining;
    }

    [[nodiscard]] const std::vector<CommandParameter> &getParameters() const
    {
        return parameters;
    }

private:
    bool chaining;
    std::vector<CommandParameter> parameters;
};

}  // namespace bedrock_protocol::types::command
