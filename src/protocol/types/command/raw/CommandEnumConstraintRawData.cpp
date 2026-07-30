/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/CommandEnumConstraintRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/CommandEnumConstraintRawData.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::command::raw {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;

CommandEnumConstraintRawData CommandEnumConstraintRawData::read(encoding::ByteBufferReader &in)
{
    const auto affectedValueIndex = LE::readUnsignedInt(in);
    const auto enumIndex = LE::readUnsignedInt(in);

    std::vector<std::uint8_t> constraints;
    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; i++) {
        constraints.push_back(Byte::readUnsigned(in));
    }

    return CommandEnumConstraintRawData(affectedValueIndex, enumIndex, std::move(constraints));
}

void CommandEnumConstraintRawData::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, affectedValueIndex);
    LE::writeUnsignedInt(out, enumIndex);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(constraints.size()));
    for (const auto &constraint : constraints) {
        Byte::writeUnsigned(out, constraint);
    }
}

}  // namespace bedrock_protocol::types::command::raw
