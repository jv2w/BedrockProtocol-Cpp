/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/StopSoundPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/StopSoundPacket.h"

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

StopSoundPacket StopSoundPacket::create(std::string soundName, bool stopAll, bool stopLegacyMusic)
{
    StopSoundPacket result;
    result.soundName = std::move(soundName);
    result.stopAll = stopAll;
    result.stopLegacyMusic = stopLegacyMusic;
    return result;
}

void StopSoundPacket::decodePayload(encoding::ByteBufferReader &in)
{
    soundName = serializer::CommonTypes::getString(in);
    stopAll = serializer::CommonTypes::getBool(in);
    stopLegacyMusic = serializer::CommonTypes::getBool(in);

}

void StopSoundPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, soundName);
    serializer::CommonTypes::putBool(out, stopAll);
    serializer::CommonTypes::putBool(out, stopLegacyMusic);

}

bool StopSoundPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleStopSound(*this);
}

}  // namespace bedrock_protocol
