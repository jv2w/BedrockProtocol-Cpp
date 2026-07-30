/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/raw/ChainedSubCommandValueRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandValueRawData.h"

#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::command::raw {

using encoding::VarInt;

ChainedSubCommandValueRawData ChainedSubCommandValueRawData::read(encoding::ByteBufferReader &in)
{
    const auto nameIndex = VarInt::readUnsignedInt(in);
    const auto type = VarInt::readUnsignedInt(in);

    return ChainedSubCommandValueRawData(nameIndex, type);
}

void ChainedSubCommandValueRawData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, nameIndex);
    VarInt::writeUnsignedInt(out, type);
}

}  // namespace bedrock_protocol::types::command::raw
