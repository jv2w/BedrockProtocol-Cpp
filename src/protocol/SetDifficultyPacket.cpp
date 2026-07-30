/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetDifficultyPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetDifficultyPacket.h"

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

SetDifficultyPacket SetDifficultyPacket::create(std::uint32_t difficulty)
{
    SetDifficultyPacket result;
    result.difficulty = difficulty;
    return result;
}

void SetDifficultyPacket::decodePayload(encoding::ByteBufferReader &in)
{
    difficulty = encoding::VarInt::readUnsignedInt(in);

}

void SetDifficultyPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, difficulty);

}

bool SetDifficultyPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetDifficulty(*this);
}

}  // namespace bedrock_protocol
