/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerVideoCapturePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerVideoCapturePacket.h"

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

PlayerVideoCapturePacket PlayerVideoCapturePacket::create(bool recording, std::optional<std::uint32_t> frameRate, std::optional<std::string> filePrefix)
{
    PlayerVideoCapturePacket result;
    result.recording = recording;
    result.frameRate = std::move(frameRate);
    result.filePrefix = std::move(filePrefix);
    return result;
}

PlayerVideoCapturePacket PlayerVideoCapturePacket::createStartRecording(std::uint32_t frameRate, std::string filePrefix)
{
    return create(true, frameRate, std::move(filePrefix));
}

PlayerVideoCapturePacket PlayerVideoCapturePacket::createStopRecording()
{
    return create(false, std::nullopt, std::nullopt);
}

void PlayerVideoCapturePacket::decodePayload(encoding::ByteBufferReader &in)
{
    recording = serializer::CommonTypes::getBool(in);
    if (recording) {
        frameRate = encoding::LE::readUnsignedInt(in);
        filePrefix = serializer::CommonTypes::getString(in);
    }

}

void PlayerVideoCapturePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, recording);
    if (recording) {
        if (!frameRate.has_value()) { // this should never be the case
            throw std::logic_error("PlayerUpdateEntityOverridesPacket with recording=true require a frame rate to be provided");
        }

        if (!filePrefix.has_value()) { // this should never be the case
            throw std::logic_error("PlayerUpdateEntityOverridesPacket with recording=true require a file prefix to be provided");
        }

        encoding::LE::writeUnsignedInt(out, *frameRate);
        serializer::CommonTypes::putString(out, *filePrefix);
    }
}

bool PlayerVideoCapturePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerVideoCapture(*this);
}

}  // namespace bedrock_protocol
