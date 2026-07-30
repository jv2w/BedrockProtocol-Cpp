/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CommandBlockUpdatePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CommandBlockUpdatePacket.h"

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

void CommandBlockUpdatePacket::decodePayload(encoding::ByteBufferReader &in)
{
    isBlock = serializer::CommonTypes::getBool(in);

    if (isBlock) {
        blockPosition = serializer::CommonTypes::getBlockPosition(in);
        commandBlockMode = encoding::VarInt::readUnsignedInt(in);
        isRedstoneMode = serializer::CommonTypes::getBool(in);
        isConditional = serializer::CommonTypes::getBool(in);
    }
    else {
        //Minecart with command block
        minecartActorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    }

    command = serializer::CommonTypes::getString(in);
    lastOutput = serializer::CommonTypes::getString(in);
    name = serializer::CommonTypes::getString(in);
    filteredName = serializer::CommonTypes::getString(in);
    shouldTrackOutput = serializer::CommonTypes::getBool(in);
    tickDelay = encoding::LE::readUnsignedInt(in);
    executeOnFirstTick = serializer::CommonTypes::getBool(in);

}

void CommandBlockUpdatePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, isBlock);

    if (isBlock) {
        serializer::CommonTypes::putBlockPosition(out, blockPosition);
        encoding::VarInt::writeUnsignedInt(out, commandBlockMode);
        serializer::CommonTypes::putBool(out, isRedstoneMode);
        serializer::CommonTypes::putBool(out, isConditional);
    }
    else {
        serializer::CommonTypes::putActorRuntimeId(out, minecartActorRuntimeId);
    }

    serializer::CommonTypes::putString(out, command);
    serializer::CommonTypes::putString(out, lastOutput);
    serializer::CommonTypes::putString(out, name);
    serializer::CommonTypes::putString(out, filteredName);
    serializer::CommonTypes::putBool(out, shouldTrackOutput);
    encoding::LE::writeUnsignedInt(out, tickDelay);
    serializer::CommonTypes::putBool(out, executeOnFirstTick);

}

bool CommandBlockUpdatePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCommandBlockUpdate(*this);
}

}  // namespace bedrock_protocol
