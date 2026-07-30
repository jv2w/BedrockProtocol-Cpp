/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/ChainedSubCommandRawData.php
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
#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandValueRawData.h"

namespace bedrock_protocol::types::command::raw {

class ChainedSubCommandRawData final {
public:
    /**
     * @param valueData
     */
    ChainedSubCommandRawData(std::string name, std::vector<ChainedSubCommandValueRawData> valueData)
        : name(std::move(name)), valueData(std::move(valueData))
    {
    }

    [[nodiscard]] const std::string &getName() const
    {
        return name;
    }

    [[nodiscard]] const std::vector<ChainedSubCommandValueRawData> &getValueData() const
    {
        return valueData;
    }

    static ChainedSubCommandRawData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::vector<ChainedSubCommandValueRawData> valueData;
};

}  // namespace bedrock_protocol::types::command::raw
