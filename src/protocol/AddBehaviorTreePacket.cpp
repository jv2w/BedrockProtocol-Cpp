/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AddBehaviorTreePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddBehaviorTreePacket.h"

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

AddBehaviorTreePacket AddBehaviorTreePacket::create(std::string behaviorTreeJson)
{
    AddBehaviorTreePacket result;
    result.behaviorTreeJson = std::move(behaviorTreeJson);
    return result;
}

void AddBehaviorTreePacket::decodePayload(encoding::ByteBufferReader &in)
{
    behaviorTreeJson = serializer::CommonTypes::getString(in);

}

void AddBehaviorTreePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, behaviorTreeJson);

}

bool AddBehaviorTreePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddBehaviorTree(*this);
}

}  // namespace bedrock_protocol
