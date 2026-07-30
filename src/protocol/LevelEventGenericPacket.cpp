/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/LevelEventGenericPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LevelEventGenericPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/serializer/NetworkNbtSerializer.h"

namespace bedrock_protocol {

LevelEventGenericPacket LevelEventGenericPacket::create(std::int32_t eventId, std::unique_ptr<nbt::tag::Tag> eventData)
{
    LevelEventGenericPacket result;
    result.eventId = eventId;
    result.eventData = std::move(eventData);
    return result;
}

void LevelEventGenericPacket::decodePayload(encoding::ByteBufferReader &in)
{
    eventId = encoding::VarInt::readSignedInt(in);
    try {
        eventData = (serializer::NetworkNbtSerializer()).readHeadless(in, nbt::NBT::TAG_Compound);
    } catch (const nbt::NbtDataException &e) {
        throw PacketDecodeException::wrap(e, getName());
    }
}

void LevelEventGenericPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, eventId);
    out.writeByteArray((serializer::NetworkNbtSerializer()).writeHeadless(*eventData));

}

bool LevelEventGenericPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLevelEventGeneric(*this);
}

}  // namespace bedrock_protocol
