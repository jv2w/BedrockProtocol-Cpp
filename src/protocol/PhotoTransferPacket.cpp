/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PhotoTransferPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PhotoTransferPacket.h"

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

PhotoTransferPacket PhotoTransferPacket::create(std::string photoName, std::string photoData, std::string bookId, std::uint8_t type, std::uint8_t sourceType, std::int64_t ownerActorUniqueId, std::string newPhotoName)
{
    PhotoTransferPacket result;
    result.photoName = std::move(photoName);
    result.photoData = std::move(photoData);
    result.bookId = std::move(bookId);
    result.type = type;
    result.sourceType = sourceType;
    result.ownerActorUniqueId = ownerActorUniqueId;
    result.newPhotoName = std::move(newPhotoName);
    return result;
}

void PhotoTransferPacket::decodePayload(encoding::ByteBufferReader &in)
{
    photoName = serializer::CommonTypes::getString(in);
    photoData = serializer::CommonTypes::getString(in);
    bookId = serializer::CommonTypes::getString(in);
    type = encoding::Byte::readUnsigned(in);
    sourceType = encoding::Byte::readUnsigned(in);
    ownerActorUniqueId = encoding::LE::readSignedLong(in); //...............
    newPhotoName = serializer::CommonTypes::getString(in);

}

void PhotoTransferPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, photoName);
    serializer::CommonTypes::putString(out, photoData);
    serializer::CommonTypes::putString(out, bookId);
    encoding::Byte::writeUnsigned(out, type);
    encoding::Byte::writeUnsigned(out, sourceType);
    encoding::LE::writeSignedLong(out, ownerActorUniqueId);
    serializer::CommonTypes::putString(out, newPhotoName);

}

bool PhotoTransferPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePhotoTransfer(*this);
}

}  // namespace bedrock_protocol
