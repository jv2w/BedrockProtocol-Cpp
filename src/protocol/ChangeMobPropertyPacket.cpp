/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ChangeMobPropertyPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ChangeMobPropertyPacket.h"

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

ChangeMobPropertyPacket ChangeMobPropertyPacket::create(std::int64_t actorUniqueId, std::string propertyName, bool boolValue, std::string stringValue, std::int32_t intValue, float floatValue)
{
    ChangeMobPropertyPacket result;
    result.actorUniqueId = actorUniqueId;
    result.propertyName = std::move(propertyName);
    result.boolValue = boolValue;
    result.stringValue = std::move(stringValue);
    result.intValue = intValue;
    result.floatValue = floatValue;
    return result;
}

ChangeMobPropertyPacket ChangeMobPropertyPacket::makeBoolValue(std::int64_t actorUniqueId, std::string propertyName, bool value)
{
    return create(actorUniqueId, propertyName, value, "", 0, 0);
}

ChangeMobPropertyPacket ChangeMobPropertyPacket::makeStringValue(std::int64_t actorUniqueId, std::string propertyName, std::string value)
{
    return create(actorUniqueId, propertyName, false, value, 0, 0);
}

ChangeMobPropertyPacket ChangeMobPropertyPacket::makeIntValue(std::int64_t actorUniqueId, std::string propertyName, std::int32_t value)
{
    return create(actorUniqueId, propertyName, false, "", value, 0);
}

ChangeMobPropertyPacket ChangeMobPropertyPacket::makeFloatValue(std::int64_t actorUniqueId, std::string propertyName, float value)
{
    return create(actorUniqueId, propertyName, false, "", 0, value);
}

void ChangeMobPropertyPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    propertyName = serializer::CommonTypes::getString(in);
    boolValue = serializer::CommonTypes::getBool(in);
    stringValue = serializer::CommonTypes::getString(in);
    intValue = encoding::VarInt::readSignedInt(in);
    floatValue = encoding::LE::readFloat(in);

}

void ChangeMobPropertyPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putString(out, propertyName);
    serializer::CommonTypes::putBool(out, boolValue);
    serializer::CommonTypes::putString(out, stringValue);
    encoding::VarInt::writeSignedInt(out, intValue);
    encoding::LE::writeFloat(out, floatValue);

}

bool ChangeMobPropertyPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleChangeMobProperty(*this);
}

}  // namespace bedrock_protocol
