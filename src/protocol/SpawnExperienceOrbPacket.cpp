/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/SpawnExperienceOrbPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SpawnExperienceOrbPacket.h"

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

SpawnExperienceOrbPacket SpawnExperienceOrbPacket::create(math::Vector3 position, std::int32_t amount)
{
    SpawnExperienceOrbPacket result;
    result.position = std::move(position);
    result.amount = amount;
    return result;
}

void SpawnExperienceOrbPacket::decodePayload(encoding::ByteBufferReader &in)
{
    position = serializer::CommonTypes::getVector3(in);
    amount = encoding::VarInt::readSignedInt(in);

}

void SpawnExperienceOrbPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putVector3(out, position);
    encoding::VarInt::writeSignedInt(out, amount);

}

bool SpawnExperienceOrbPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSpawnExperienceOrb(*this);
}

}  // namespace bedrock_protocol
