/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/LessonProgressPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LessonProgressPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

LessonProgressPacket LessonProgressPacket::create(std::int32_t action, std::int32_t score, std::string activityId)
{
    LessonProgressPacket result;
    result.action = action;
    result.score = score;
    result.activityId = std::move(activityId);
    return result;
}

void LessonProgressPacket::decodePayload(encoding::ByteBufferReader &in)
{
    action = encoding::VarInt::readSignedInt(in);
    score = encoding::VarInt::readSignedInt(in);
    activityId = serializer::CommonTypes::getString(in);

}

void LessonProgressPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, action);
    encoding::VarInt::writeSignedInt(out, score);
    serializer::CommonTypes::putString(out, activityId);

}

bool LessonProgressPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLessonProgress(*this);
}

}  // namespace bedrock_protocol
