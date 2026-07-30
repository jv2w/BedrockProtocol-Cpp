/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandSoftEnum.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::command {

class CommandSoftEnum final {
public:
    /**
     * @param values
     */
    CommandSoftEnum(std::string name, std::vector<std::string> values)
        : name(std::move(name)), values(std::move(values))
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::vector<std::string> &getValues() const
    {
        return values;
    }

    static CommandSoftEnum read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::vector<std::string> values;
};

}  // namespace bedrock_protocol::types::command
