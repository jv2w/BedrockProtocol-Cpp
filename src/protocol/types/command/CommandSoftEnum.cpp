/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/CommandSoftEnum.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/CommandSoftEnum.h"

#include <cstdint>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command {

using encoding::VarInt;
using serializer::CommonTypes;

CommandSoftEnum CommandSoftEnum::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);

    std::vector<std::string> values;
    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        values.push_back(CommonTypes::getString(in));
    }

    return CommandSoftEnum(std::move(name), std::move(values));
}

void CommandSoftEnum::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(values.size()));
    for (const auto &value : values) {
        CommonTypes::putString(out, value);
    }
}

}  // namespace bedrock_protocol::types::command
