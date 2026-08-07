/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlaySoundPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlaySoundPacket.h"

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

PlaySoundPacket PlaySoundPacket::create(std::string soundName, float x, float y, float z, float volume, float pitch, std::int32_t loopCount, std::optional<std::uint64_t> serverSoundHandle)
{
    PlaySoundPacket result;
    result.soundName = std::move(soundName);
    result.x = x;
    result.y = y;
    result.z = z;
    result.volume = volume;
    result.pitch = pitch;
    result.loopCount = loopCount;
    result.serverSoundHandle = std::move(serverSoundHandle);
    return result;
}

void PlaySoundPacket::decodePayload(encoding::ByteBufferReader &in)
{
    soundName = serializer::CommonTypes::getString(in);
    const auto blockPosition = serializer::CommonTypes::getBlockPosition(in);
    // 8.0F, not 8: PHP's / always produces a float, but in C++ these operands are both int, so a
    // plain / 8 truncates and snaps the sound to whole-block coordinates. The deep round-trip suite
    // caught this because re-encoding a decoded packet then wrote different bytes.
    x = blockPosition.getX() / 8.0F;
    y = blockPosition.getY() / 8.0F;
    z = blockPosition.getZ() / 8.0F;
    volume = encoding::LE::readFloat(in);
    pitch = encoding::LE::readFloat(in);
    loopCount = encoding::VarInt::readSignedInt(in);
    serverSoundHandle = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readUnsignedLong(reader); });

}

void PlaySoundPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, soundName);
    serializer::CommonTypes::putBlockPosition(out, types::BlockPosition((int) (x * 8), (int) (y * 8), (int) (z * 8)));
    encoding::LE::writeFloat(out, volume);
    encoding::LE::writeFloat(out, pitch);
    encoding::VarInt::writeSignedInt(out, loopCount);
    serializer::CommonTypes::writeOptional(out, serverSoundHandle, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeUnsignedLong(writer, value); });

}

bool PlaySoundPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlaySound(*this);
}

}  // namespace bedrock_protocol
