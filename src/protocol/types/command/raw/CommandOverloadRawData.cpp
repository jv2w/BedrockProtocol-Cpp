/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/CommandOverloadRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/CommandOverloadRawData.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command::raw {

using encoding::VarInt;
using serializer::CommonTypes;

CommandOverloadRawData CommandOverloadRawData::read(encoding::ByteBufferReader &in)
{
    const auto chaining = CommonTypes::getBool(in);
    std::vector<CommandParameterRawData> parameters;

    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        parameters.push_back(CommandParameterRawData::read(in));
    }

    return CommandOverloadRawData(chaining, std::move(parameters));
}

void CommandOverloadRawData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBool(out, chaining);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(parameters.size()));

    for (const auto &parameter : parameters) {
        parameter.write(out);
    }
}

}  // namespace bedrock_protocol::types::command::raw
