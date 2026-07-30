/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SetDisplayObjectivePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetDisplayObjectivePacket.h"

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

SetDisplayObjectivePacket SetDisplayObjectivePacket::create(std::string displaySlot, std::string objectiveName, std::string displayName, std::string criteriaName, std::int32_t sortOrder)
{
    SetDisplayObjectivePacket result;
    result.displaySlot = std::move(displaySlot);
    result.objectiveName = std::move(objectiveName);
    result.displayName = std::move(displayName);
    result.criteriaName = std::move(criteriaName);
    result.sortOrder = sortOrder;
    return result;
}

void SetDisplayObjectivePacket::decodePayload(encoding::ByteBufferReader &in)
{
    displaySlot = serializer::CommonTypes::getString(in);
    objectiveName = serializer::CommonTypes::getString(in);
    displayName = serializer::CommonTypes::getString(in);
    criteriaName = serializer::CommonTypes::getString(in);
    sortOrder = encoding::VarInt::readSignedInt(in);

}

void SetDisplayObjectivePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, displaySlot);
    serializer::CommonTypes::putString(out, objectiveName);
    serializer::CommonTypes::putString(out, displayName);
    serializer::CommonTypes::putString(out, criteriaName);
    encoding::VarInt::writeSignedInt(out, sortOrder);

}

bool SetDisplayObjectivePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetDisplayObjective(*this);
}

}  // namespace bedrock_protocol
