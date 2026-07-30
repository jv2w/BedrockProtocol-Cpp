/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerBlockActionWithBlockInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/PlayerBlockActionWithBlockInfo.h"

#include <stdexcept>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/PlayerAction.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

PlayerBlockActionWithBlockInfo::PlayerBlockActionWithBlockInfo(const std::int32_t actionType,
                                                              const BlockPosition &blockPosition,
                                                              const std::int32_t face)
    : actionType(actionType), blockPosition(blockPosition), face(face)
{
    if (!isValidActionType(actionType)) {
        throw std::invalid_argument("Invalid action type for PlayerBlockActionWithBlockInfo");
    }
}

PlayerBlockActionWithBlockInfo PlayerBlockActionWithBlockInfo::read(encoding::ByteBufferReader &in,
                                                                   const std::int32_t actionType)
{
    const auto blockPosition = CommonTypes::getBlockPosition(in);
    const auto face = VarInt::readSignedInt(in);
    return PlayerBlockActionWithBlockInfo(actionType, blockPosition, face);
}

void PlayerBlockActionWithBlockInfo::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBlockPosition(out, blockPosition);
    VarInt::writeSignedInt(out, face);
}

bool PlayerBlockActionWithBlockInfo::isValidActionType(const std::int32_t actionType)
{
    switch (actionType) {
    case PlayerAction::ABORT_BREAK:
    case PlayerAction::START_BREAK:
    // PHP also lists PlayerAction::CRACK_BLOCK here, which is the same value (18) as CRACK_BREAK;
    // duplicate case labels are illegal in C++, so only one is listed. Behaviour is identical.
    case PlayerAction::CRACK_BREAK:
    case PlayerAction::PREDICT_DESTROY_BLOCK:
    case PlayerAction::CONTINUE_DESTROY_BLOCK:
        return true;
    default:
        return false;
    }
}

}  // namespace bedrock_protocol::types
