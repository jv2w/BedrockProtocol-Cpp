/*
 * This file is part of BedrockProtocol-Cpp.
 * Added for 1.26.40: gophertunnel v1.58.0 minecraft/protocol/sound.go has no PHP counterpart.
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/SoundDataUpdate.h"

#include <stdexcept>
#include <string>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;

SoundDataUpdate SoundDataUpdate::read(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/sound.go:30-45.
    SoundDataUpdate result;
    result.type = VarInt::readUnsignedInt(in);
    switch (result.type) {
    case TYPE_STOP:
    case TYPE_PAUSE:
    case TYPE_RESUME:
        break;
    case TYPE_SET_VOLUME:
        result.volume = LE::readFloat(in);
        break;
    case TYPE_SET_PITCH:
        result.pitch = LE::readFloat(in);
        break;
    case TYPE_FADE:
        result.duration = LE::readFloat(in);
        result.targetVolume = LE::readFloat(in);
        break;
    case TYPE_SEEK_TO:
        result.seconds = LE::readFloat(in);
        break;
    default:
        throw PacketDecodeException("Unknown sound data update type " + std::to_string(result.type));
    }

    return result;
}

void SoundDataUpdate::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, type);
    switch (type) {
    case TYPE_STOP:
    case TYPE_PAUSE:
    case TYPE_RESUME:
        break;
    case TYPE_SET_VOLUME:
        LE::writeFloat(out, volume);
        break;
    case TYPE_SET_PITCH:
        LE::writeFloat(out, pitch);
        break;
    case TYPE_FADE:
        LE::writeFloat(out, duration);
        LE::writeFloat(out, targetVolume);
        break;
    case TYPE_SEEK_TO:
        LE::writeFloat(out, seconds);
        break;
    default:
        throw std::invalid_argument("Unknown sound data update type " + std::to_string(type));
    }
}

}  // namespace bedrock_protocol::types
