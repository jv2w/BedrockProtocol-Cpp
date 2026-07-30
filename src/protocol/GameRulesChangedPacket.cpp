/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/GameRulesChangedPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/GameRulesChangedPacket.h"

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

GameRulesChangedPacket GameRulesChangedPacket::create(std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>> gameRules)
{
    GameRulesChangedPacket result;
    result.gameRules = std::move(gameRules);
    return result;
}

void GameRulesChangedPacket::decodePayload(encoding::ByteBufferReader &in)
{
    gameRules = serializer::CommonTypes::getGameRules(in, false);

}

void GameRulesChangedPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putGameRules(out, gameRules, false);

}

bool GameRulesChangedPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleGameRulesChanged(*this);
}

}  // namespace bedrock_protocol
