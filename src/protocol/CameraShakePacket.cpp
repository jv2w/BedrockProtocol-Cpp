/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CameraShakePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CameraShakePacket.h"

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

CameraShakePacket CameraShakePacket::create(float intensity, float duration, std::uint8_t shakeType, std::uint8_t shakeAction)
{
    CameraShakePacket result;
    result.intensity = intensity;
    result.duration = duration;
    result.shakeType = shakeType;
    result.shakeAction = shakeAction;
    return result;
}

void CameraShakePacket::decodePayload(encoding::ByteBufferReader &in)
{
    intensity = encoding::LE::readFloat(in);
    duration = encoding::LE::readFloat(in);
    shakeType = encoding::Byte::readUnsigned(in);
    shakeAction = encoding::Byte::readUnsigned(in);

}

void CameraShakePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeFloat(out, intensity);
    encoding::LE::writeFloat(out, duration);
    encoding::Byte::writeUnsigned(out, shakeType);
    encoding::Byte::writeUnsigned(out, shakeAction);

}

bool CameraShakePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCameraShake(*this);
}

}  // namespace bedrock_protocol
