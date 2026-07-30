/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ShowStoreOfferPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ShowStoreOfferPacket.h"

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

ShowStoreOfferPacket ShowStoreOfferPacket::create(uuid::Uuid offerId, types::ShowStoreOfferRedirectType redirectType)
{
    ShowStoreOfferPacket result;
    result.offerId = std::move(offerId);
    result.redirectType = redirectType;
    return result;
}

void ShowStoreOfferPacket::decodePayload(encoding::ByteBufferReader &in)
{
    offerId = serializer::CommonTypes::getUUID(in);
    redirectType = types::ShowStoreOfferRedirectTypeFromPacket(encoding::Byte::readUnsigned(in));

}

void ShowStoreOfferPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putUUID(out, offerId);
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(redirectType));

}

bool ShowStoreOfferPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleShowStoreOffer(*this);
}

}  // namespace bedrock_protocol
