/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/GameTestRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/GameTestRequestPacket.h"

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

GameTestRequestPacket GameTestRequestPacket::create(std::int32_t maxTestsPerBatch, std::int32_t repeatCount, std::uint8_t rotation, bool stopOnFailure, types::BlockPosition testPosition, std::int32_t testsPerRow, std::string testName)
{
    GameTestRequestPacket result;
    result.maxTestsPerBatch = maxTestsPerBatch;
    result.repeatCount = repeatCount;
    result.rotation = rotation;
    result.stopOnFailure = stopOnFailure;
    result.testPosition = std::move(testPosition);
    result.testsPerRow = testsPerRow;
    result.testName = std::move(testName);
    return result;
}

void GameTestRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    maxTestsPerBatch = encoding::VarInt::readSignedInt(in);
    repeatCount = encoding::VarInt::readSignedInt(in);
    rotation = encoding::Byte::readUnsigned(in);
    stopOnFailure = serializer::CommonTypes::getBool(in);
    testPosition = serializer::CommonTypes::getBlockPosition(in);
    testsPerRow = encoding::VarInt::readSignedInt(in);
    testName = serializer::CommonTypes::getString(in);

}

void GameTestRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, maxTestsPerBatch);
    encoding::VarInt::writeSignedInt(out, repeatCount);
    encoding::Byte::writeUnsigned(out, rotation);
    serializer::CommonTypes::putBool(out, stopOnFailure);
    serializer::CommonTypes::putBlockPosition(out, testPosition);
    encoding::VarInt::writeSignedInt(out, testsPerRow);
    serializer::CommonTypes::putString(out, testName);

}

bool GameTestRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleGameTestRequest(*this);
}

}  // namespace bedrock_protocol
