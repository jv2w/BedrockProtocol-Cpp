/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayerSkinPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerSkinPacket.h"

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

PlayerSkinPacket PlayerSkinPacket::create(uuid::Uuid uuid, std::string oldSkinName, std::string newSkinName, types::skin::SkinData skin)
{
    PlayerSkinPacket result;
    result.uuid = std::move(uuid);
    result.oldSkinName = std::move(oldSkinName);
    result.newSkinName = std::move(newSkinName);
    result.skin = std::move(skin);
    return result;
}

void PlayerSkinPacket::decodePayload(encoding::ByteBufferReader &in)
{
    uuid = serializer::CommonTypes::getUUID(in);
    skin = serializer::CommonTypes::getSkin(in);
    newSkinName = serializer::CommonTypes::getString(in);
    oldSkinName = serializer::CommonTypes::getString(in);
    skin->setVerified(serializer::CommonTypes::getBool(in));

}

void PlayerSkinPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putUUID(out, uuid);
    //PHP throws Error when encoding a packet whose typed $skin was never initialised; dereferencing an
    //empty optional would instead be undefined behaviour, so use value() to keep the PHP failure mode.
    serializer::CommonTypes::putSkin(out, skin.value());
    serializer::CommonTypes::putString(out, newSkinName);
    serializer::CommonTypes::putString(out, oldSkinName);
    serializer::CommonTypes::putBool(out, skin.value().isVerified());

}

bool PlayerSkinPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerSkin(*this);
}

}  // namespace bedrock_protocol
