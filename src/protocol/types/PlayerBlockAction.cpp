/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerBlockAction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

PlayerBlockAction PlayerBlockAction::read(encoding::ByteBufferReader &in)
{
    // player.go:159-162 - action, position and face, in that order, for every action type.
    const auto actionType = VarInt::readSignedInt(in);
    const auto blockPosition = CommonTypes::getBlockPosition(in);
    const auto face = VarInt::readSignedInt(in);

    return {actionType, blockPosition, face};
}

void PlayerBlockAction::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeSignedInt(out, actionType);
    CommonTypes::putBlockPosition(out, blockPosition);
    VarInt::writeSignedInt(out, face);
}

}  // namespace bedrock_protocol::types
