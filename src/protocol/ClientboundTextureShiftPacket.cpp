/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ClientboundTextureShiftPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundTextureShiftPacket.h"

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

ClientboundTextureShiftPacket ClientboundTextureShiftPacket::create(std::uint8_t actionId, std::string collectionName, std::string fromStep, std::string toStep, std::vector<std::string> allSteps, std::uint64_t currentLengthTicks, std::uint64_t totalLengthTicks, bool enabled)
{
    ClientboundTextureShiftPacket result;
    result.actionId = actionId;
    result.collectionName = std::move(collectionName);
    result.fromStep = std::move(fromStep);
    result.toStep = std::move(toStep);
    result.allSteps = std::move(allSteps);
    result.currentLengthTicks = currentLengthTicks;
    result.totalLengthTicks = totalLengthTicks;
    result.enabled = enabled;
    return result;
}

void ClientboundTextureShiftPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actionId = encoding::Byte::readUnsigned(in);
    collectionName = serializer::CommonTypes::getString(in);
    fromStep = serializer::CommonTypes::getString(in);
    toStep = serializer::CommonTypes::getString(in);

    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        allSteps.push_back(serializer::CommonTypes::getString(in));
    }

    currentLengthTicks = encoding::VarInt::readUnsignedLong(in);
    totalLengthTicks = encoding::VarInt::readUnsignedLong(in);
    enabled = serializer::CommonTypes::getBool(in);

}

void ClientboundTextureShiftPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, actionId);
    serializer::CommonTypes::putString(out, collectionName);
    serializer::CommonTypes::putString(out, fromStep);
    serializer::CommonTypes::putString(out, toStep);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(allSteps.size()));
    for (const auto &step : allSteps) {
        serializer::CommonTypes::putString(out, step);
    }

    encoding::VarInt::writeUnsignedLong(out, currentLengthTicks);
    encoding::VarInt::writeUnsignedLong(out, totalLengthTicks);
    serializer::CommonTypes::putBool(out, enabled);

}

bool ClientboundTextureShiftPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundTextureShift(*this);
}

}  // namespace bedrock_protocol
