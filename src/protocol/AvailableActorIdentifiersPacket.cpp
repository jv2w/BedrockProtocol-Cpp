/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AvailableActorIdentifiersPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AvailableActorIdentifiersPacket.h"

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

AvailableActorIdentifiersPacket AvailableActorIdentifiersPacket::create(types::CacheableNbt<nbt::tag::CompoundTag> identifiers)
{
    AvailableActorIdentifiersPacket result;
    result.identifiers = std::move(identifiers);
    return result;
}

void AvailableActorIdentifiersPacket::decodePayload(encoding::ByteBufferReader &in)
{
    identifiers = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void AvailableActorIdentifiersPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    //PHP throws Error when encoding a packet whose typed property was never initialised; operator-> on an
    //empty optional would instead be undefined behaviour, so use value() to keep the PHP failure mode.
    out.writeByteArray(identifiers.value().getEncodedNbt());

}

bool AvailableActorIdentifiersPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAvailableActorIdentifiers(*this);
}

}  // namespace bedrock_protocol
