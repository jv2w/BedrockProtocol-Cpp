/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CreatePhotoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CreatePhotoPacket.h"

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

CreatePhotoPacket CreatePhotoPacket::create(std::int64_t actorUniqueId, std::string photoName, std::string photoItemName)
{
    CreatePhotoPacket result;
    result.actorUniqueId = actorUniqueId;
    result.photoName = std::move(photoName);
    result.photoItemName = std::move(photoItemName);
    return result;
}

void CreatePhotoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = encoding::LE::readSignedLong(in); //why be consistent mojang ?????
    photoName = serializer::CommonTypes::getString(in);
    photoItemName = serializer::CommonTypes::getString(in);

}

void CreatePhotoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedLong(out, actorUniqueId);
    serializer::CommonTypes::putString(out, photoName);
    serializer::CommonTypes::putString(out, photoItemName);

}

bool CreatePhotoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCreatePhoto(*this);
}

}  // namespace bedrock_protocol
