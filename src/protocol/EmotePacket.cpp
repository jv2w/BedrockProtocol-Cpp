/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/EmotePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/EmotePacket.h"

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

EmotePacket EmotePacket::create(std::uint64_t actorRuntimeId, std::string emoteId, std::uint32_t emoteLengthTicks, std::string xboxUserId, std::string platformChatId, std::uint8_t flags)
{
    EmotePacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.emoteId = std::move(emoteId);
    result.emoteLengthTicks = emoteLengthTicks;
    result.xboxUserId = std::move(xboxUserId);
    result.platformChatId = std::move(platformChatId);
    result.flags = flags;
    return result;
}

void EmotePacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    emoteId = serializer::CommonTypes::getString(in);
    emoteLengthTicks = encoding::VarInt::readUnsignedInt(in);
    xboxUserId = serializer::CommonTypes::getString(in);
    platformChatId = serializer::CommonTypes::getString(in);
    flags = encoding::Byte::readUnsigned(in);

}

void EmotePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putString(out, emoteId);
    encoding::VarInt::writeUnsignedInt(out, emoteLengthTicks);
    serializer::CommonTypes::putString(out, xboxUserId);
    serializer::CommonTypes::putString(out, platformChatId);
    encoding::Byte::writeUnsigned(out, flags);

}

bool EmotePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleEmote(*this);
}

}  // namespace bedrock_protocol
