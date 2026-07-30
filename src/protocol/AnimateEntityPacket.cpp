/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AnimateEntityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AnimateEntityPacket.h"

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

AnimateEntityPacket AnimateEntityPacket::create(std::string animation, std::string nextState, std::string stopExpression, std::int32_t stopExpressionVersion, std::string controller, float blendOutTime, std::vector<std::uint64_t> actorRuntimeIds)
{
    AnimateEntityPacket result;
    result.animation = std::move(animation);
    result.nextState = std::move(nextState);
    result.stopExpression = std::move(stopExpression);
    result.stopExpressionVersion = stopExpressionVersion;
    result.controller = std::move(controller);
    result.blendOutTime = blendOutTime;
    result.actorRuntimeIds = std::move(actorRuntimeIds);
    return result;
}

void AnimateEntityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    animation = serializer::CommonTypes::getString(in);
    nextState = serializer::CommonTypes::getString(in);
    stopExpression = serializer::CommonTypes::getString(in);
    stopExpressionVersion = encoding::LE::readSignedInt(in);
    controller = serializer::CommonTypes::getString(in);
    blendOutTime = encoding::LE::readFloat(in);
    actorRuntimeIds.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        actorRuntimeIds.push_back(serializer::CommonTypes::getActorRuntimeId(in));
    }

}

void AnimateEntityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, animation);
    serializer::CommonTypes::putString(out, nextState);
    serializer::CommonTypes::putString(out, stopExpression);
    encoding::LE::writeSignedInt(out, stopExpressionVersion);
    serializer::CommonTypes::putString(out, controller);
    encoding::LE::writeFloat(out, blendOutTime);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(actorRuntimeIds.size()));
    for (const auto &id : actorRuntimeIds) {
        serializer::CommonTypes::putActorRuntimeId(out, id);
    }

}

bool AnimateEntityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAnimateEntity(*this);
}

}  // namespace bedrock_protocol
