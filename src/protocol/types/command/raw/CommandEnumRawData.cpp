/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/CommandEnumRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/CommandEnumRawData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command::raw {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

CommandEnumRawData CommandEnumRawData::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    std::vector<std::uint32_t> valueIndexes;
    const auto size = VarInt::readUnsignedInt(in);

    for (std::uint32_t i = 0; i < size; i++) {
        valueIndexes.push_back(LE::readUnsignedInt(in));
    }

    return CommandEnumRawData(std::move(name), std::move(valueIndexes));
}

void CommandEnumRawData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(valueIndexes.size()));

    for (const auto &index : valueIndexes) {
        LE::writeUnsignedInt(out, index);
    }
}

}  // namespace bedrock_protocol::types::command::raw
