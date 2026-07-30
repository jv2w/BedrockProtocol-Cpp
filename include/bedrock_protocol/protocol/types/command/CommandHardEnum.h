/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandHardEnum.php
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
#include <variant>
#include <vector>

#include "bedrock_protocol/protocol/types/command/ConstrainedEnumValue.h"

namespace bedrock_protocol::types::command {

class CommandHardEnum {
public:
    /**
     * A single enum value: either a plain string, or a value carrying constraints.
     */
    using Value = std::variant<std::string, ConstrainedEnumValue>;

    /**
     * @param values
     */
    CommandHardEnum(std::string name, std::vector<Value> values) : name(std::move(name)), values(std::move(values)) {}

    virtual ~CommandHardEnum() = default;

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::vector<Value> &getValues() const
    {
        return values;
    }

private:
    std::string name;
    std::vector<Value> values;
};

}  // namespace bedrock_protocol::types::command
