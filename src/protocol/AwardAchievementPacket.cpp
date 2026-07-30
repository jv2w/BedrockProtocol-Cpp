/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AwardAchievementPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AwardAchievementPacket.h"

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

AwardAchievementPacket AwardAchievementPacket::create(std::int32_t achievementId)
{
    AwardAchievementPacket result;
    result.achievementId = achievementId;
    return result;
}

void AwardAchievementPacket::decodePayload(encoding::ByteBufferReader &in)
{
    achievementId = encoding::LE::readSignedInt(in);

}

void AwardAchievementPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedInt(out, achievementId);

}

bool AwardAchievementPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAwardAchievement(*this);
}

}  // namespace bedrock_protocol
