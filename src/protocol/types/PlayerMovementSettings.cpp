/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerMovementSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PlayerMovementSettings.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

PlayerMovementSettings PlayerMovementSettings::read(encoding::ByteBufferReader &in)
{
    const auto rewindHistorySize = VarInt::readSignedInt(in);
    const auto serverAuthBlockBreaking = CommonTypes::getBool(in);
    return PlayerMovementSettings(rewindHistorySize, serverAuthBlockBreaking);
}

void PlayerMovementSettings::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, rewindHistorySize);
    CommonTypes::putBool(out, serverAuthoritativeBlockBreaking);
}

}  // namespace bedrock_protocol::types
