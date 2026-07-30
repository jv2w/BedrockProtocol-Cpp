/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/CommandOverloadRawData.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandParameterRawData.h"

namespace bedrock_protocol::types::command::raw {

class CommandOverloadRawData final {
public:
    /**
     * @param parameters
     */
    CommandOverloadRawData(bool chaining, std::vector<CommandParameterRawData> parameters)
        : chaining(chaining), parameters(std::move(parameters))
    {
    }

    [[nodiscard]] bool isChaining() const
    {
        return chaining;
    }

    [[nodiscard]] const std::vector<CommandParameterRawData> &getParameters() const
    {
        return parameters;
    }

    static CommandOverloadRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    bool chaining;
    std::vector<CommandParameterRawData> parameters;
};

}  // namespace bedrock_protocol::types::command::raw
