/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundUpdateSoundDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundUpdateSoundDataPacket.h"

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

ClientboundUpdateSoundDataPacket ClientboundUpdateSoundDataPacket::create(std::uint64_t serverSoundHandle, std::optional<types::SoundDataUpdate> stop, std::optional<types::SoundDataUpdate> setVolume, std::optional<types::SoundDataUpdate> setPitch, std::optional<types::SoundDataUpdate> fade, std::optional<types::SoundDataUpdate> seekTo, std::optional<types::SoundDataUpdate> pause, std::optional<types::SoundDataUpdate> resume)
{
    ClientboundUpdateSoundDataPacket result;
    result.serverSoundHandle = serverSoundHandle;
    result.stop = stop;
    result.setVolume = setVolume;
    result.setPitch = setPitch;
    result.fade = fade;
    result.seekTo = seekTo;
    result.pause = pause;
    result.resume = resume;
    return result;
}

namespace {
std::optional<types::SoundDataUpdate> getSoundDataUpdate(encoding::ByteBufferReader &in)
{
    return serializer::CommonTypes::getBool(in) ? std::optional(types::SoundDataUpdate::read(in)) : std::nullopt;
}

void putSoundDataUpdate(encoding::ByteBufferWriter &out, const std::optional<types::SoundDataUpdate> &update)
{
    serializer::CommonTypes::putBool(out, update.has_value());
    if (update.has_value()) {
        update->write(out);
    }
}
}  // namespace

void ClientboundUpdateSoundDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    //gophertunnel v1.58.0 minecraft/protocol/packet/clientbound_update_sound_data.go:25-34.
    serverSoundHandle = encoding::LE::readUnsignedLong(in);
    stop = getSoundDataUpdate(in);
    setVolume = getSoundDataUpdate(in);
    setPitch = getSoundDataUpdate(in);
    fade = getSoundDataUpdate(in);
    seekTo = getSoundDataUpdate(in);
    pause = getSoundDataUpdate(in);
    resume = getSoundDataUpdate(in);

}

void ClientboundUpdateSoundDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedLong(out, serverSoundHandle);
    putSoundDataUpdate(out, stop);
    putSoundDataUpdate(out, setVolume);
    putSoundDataUpdate(out, setPitch);
    putSoundDataUpdate(out, fade);
    putSoundDataUpdate(out, seekTo);
    putSoundDataUpdate(out, pause);
    putSoundDataUpdate(out, resume);

}

bool ClientboundUpdateSoundDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundUpdateSoundData(*this);
}

}  // namespace bedrock_protocol
