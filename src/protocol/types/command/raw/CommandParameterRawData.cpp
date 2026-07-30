/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/raw/CommandParameterRawData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/raw/CommandParameterRawData.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::command::raw {

using encoding::Byte;
using encoding::LE;
using serializer::CommonTypes;

CommandParameterRawData CommandParameterRawData::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    const auto typeInfo = LE::readUnsignedInt(in);
    const auto optional = CommonTypes::getBool(in);
    const auto flags = Byte::readUnsigned(in);

    return CommandParameterRawData(std::move(name), typeInfo, optional, flags);
}

void CommandParameterRawData::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    LE::writeUnsignedInt(out, typeInfo);
    CommonTypes::putBool(out, optional);
    Byte::writeUnsigned(out, flags);
}

}  // namespace bedrock_protocol::types::command::raw
