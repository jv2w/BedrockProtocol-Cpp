/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateAdventureSettingsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateAdventureSettingsPacket.h"

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

UpdateAdventureSettingsPacket UpdateAdventureSettingsPacket::create(bool noAttackingMobs, bool noAttackingPlayers, bool worldImmutable, bool showNameTags, bool autoJump)
{
    UpdateAdventureSettingsPacket result;
    result.noAttackingMobs = noAttackingMobs;
    result.noAttackingPlayers = noAttackingPlayers;
    result.worldImmutable = worldImmutable;
    result.showNameTags = showNameTags;
    result.autoJump = autoJump;
    return result;
}

void UpdateAdventureSettingsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    noAttackingMobs = serializer::CommonTypes::getBool(in);
    noAttackingPlayers = serializer::CommonTypes::getBool(in);
    worldImmutable = serializer::CommonTypes::getBool(in);
    showNameTags = serializer::CommonTypes::getBool(in);
    autoJump = serializer::CommonTypes::getBool(in);

}

void UpdateAdventureSettingsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, noAttackingMobs);
    serializer::CommonTypes::putBool(out, noAttackingPlayers);
    serializer::CommonTypes::putBool(out, worldImmutable);
    serializer::CommonTypes::putBool(out, showNameTags);
    serializer::CommonTypes::putBool(out, autoJump);

}

bool UpdateAdventureSettingsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateAdventureSettings(*this);
}

}  // namespace bedrock_protocol
