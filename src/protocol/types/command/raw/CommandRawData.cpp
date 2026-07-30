/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/CommandRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/CommandRawData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command::raw {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

CommandRawData CommandRawData::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    auto description = CommonTypes::getString(in);
    const auto flags = LE::readUnsignedShort(in);
    auto permission = CommonTypes::getString(in);
    const auto aliasEnumIndex = LE::readSignedInt(in); //may be -1 for not set

    std::vector<std::uint32_t> chainedSubCommandDataIndexes;
    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        chainedSubCommandDataIndexes.push_back(LE::readUnsignedInt(in));
    }

    std::vector<CommandOverloadRawData> overloads;
    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        overloads.push_back(CommandOverloadRawData::read(in));
    }

    return CommandRawData(std::move(name), std::move(description), flags, std::move(permission), aliasEnumIndex,
                          std::move(chainedSubCommandDataIndexes), std::move(overloads));
}

void CommandRawData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    CommonTypes::putString(out, description);
    LE::writeUnsignedShort(out, flags);
    CommonTypes::putString(out, permission);
    LE::writeSignedInt(out, aliasEnumIndex);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(chainedSubCommandDataIndexes.size()));
    for (const auto &index : chainedSubCommandDataIndexes) {
        LE::writeUnsignedInt(out, index);
    }

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(overloads.size()));
    for (const auto &overload : overloads) {
        overload.write(out);
    }
}

}  // namespace bedrock_protocol::types::command::raw
