/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/OnScreenTextureAnimationPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/OnScreenTextureAnimationPacket.h"

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

OnScreenTextureAnimationPacket OnScreenTextureAnimationPacket::create(std::uint32_t effectId)
{
    OnScreenTextureAnimationPacket result;
    result.effectId = effectId;
    return result;
}

void OnScreenTextureAnimationPacket::decodePayload(encoding::ByteBufferReader &in)
{
    effectId = encoding::LE::readUnsignedInt(in);

}

void OnScreenTextureAnimationPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedInt(out, effectId);

}

bool OnScreenTextureAnimationPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleOnScreenTextureAnimation(*this);
}

}  // namespace bedrock_protocol
