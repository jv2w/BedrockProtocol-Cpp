/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/GameTestResultsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/GameTestResultsPacket.h"

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

GameTestResultsPacket GameTestResultsPacket::create(bool success, std::string error, std::string testName)
{
    GameTestResultsPacket result;
    result.success = success;
    result.error = std::move(error);
    result.testName = std::move(testName);
    return result;
}

void GameTestResultsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    success = serializer::CommonTypes::getBool(in);
    error = serializer::CommonTypes::getString(in);
    testName = serializer::CommonTypes::getString(in);

}

void GameTestResultsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, success);
    serializer::CommonTypes::putString(out, error);
    serializer::CommonTypes::putString(out, testName);

}

bool GameTestResultsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleGameTestResults(*this);
}

}  // namespace bedrock_protocol
