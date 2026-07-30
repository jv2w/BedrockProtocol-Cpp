/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NpcDialoguePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/NpcDialoguePacket.h"

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

NpcDialoguePacket NpcDialoguePacket::create(std::int64_t npcActorUniqueId, std::int32_t actionType, std::string dialogue, std::string sceneName, std::string npcName, std::string actionJson)
{
    NpcDialoguePacket result;
    result.npcActorUniqueId = npcActorUniqueId;
    result.actionType = actionType;
    result.dialogue = std::move(dialogue);
    result.sceneName = std::move(sceneName);
    result.npcName = std::move(npcName);
    result.actionJson = std::move(actionJson);
    return result;
}

void NpcDialoguePacket::decodePayload(encoding::ByteBufferReader &in)
{
    npcActorUniqueId = encoding::LE::readSignedLong(in); //WHY NOT USING STANDARD METHODS, MOJANG
    actionType = encoding::VarInt::readSignedInt(in);
    dialogue = serializer::CommonTypes::getString(in);
    sceneName = serializer::CommonTypes::getString(in);
    npcName = serializer::CommonTypes::getString(in);
    actionJson = serializer::CommonTypes::getString(in);

}

void NpcDialoguePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedLong(out, npcActorUniqueId);
    encoding::VarInt::writeSignedInt(out, actionType);
    serializer::CommonTypes::putString(out, dialogue);
    serializer::CommonTypes::putString(out, sceneName);
    serializer::CommonTypes::putString(out, npcName);
    serializer::CommonTypes::putString(out, actionJson);

}

bool NpcDialoguePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleNpcDialogue(*this);
}

}  // namespace bedrock_protocol
