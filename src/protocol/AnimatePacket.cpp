/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AnimatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AnimatePacket.h"

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

AnimatePacket AnimatePacket::create(std::uint64_t actorRuntimeId, std::uint8_t action, float data, std::optional<std::string> swingSource)
{
    AnimatePacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.action = action;
    result.data = data;
    result.swingSource = std::move(swingSource);
    return result;
}

void AnimatePacket::decodePayload(encoding::ByteBufferReader &in)
{
    action = encoding::Byte::readUnsigned(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    data = encoding::LE::readFloat(in);
    swingSource = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });

}

void AnimatePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, action);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::LE::writeFloat(out, data);
    serializer::CommonTypes::writeOptional(out, swingSource, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putString(writer, value); });

}

bool AnimatePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAnimate(*this);
}

}  // namespace bedrock_protocol
