/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SpawnSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SpawnSettings.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

SpawnSettings SpawnSettings::read(encoding::ByteBufferReader &in)
{
    const auto biomeType = LE::readUnsignedShort(in);
    auto biomeName = CommonTypes::getString(in);
    const auto dimension = VarInt::readSignedInt(in);

    return SpawnSettings(biomeType, std::move(biomeName), dimension);
}

void SpawnSettings::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedShort(out, biomeType);
    CommonTypes::putString(out, biomeName);
    VarInt::writeSignedInt(out, dimension);
}

}  // namespace bedrock_protocol::types
