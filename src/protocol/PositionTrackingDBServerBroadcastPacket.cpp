/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PositionTrackingDBServerBroadcastPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PositionTrackingDBServerBroadcastPacket.h"

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

PositionTrackingDBServerBroadcastPacket PositionTrackingDBServerBroadcastPacket::create(std::uint8_t action, std::int32_t trackingId, types::CacheableNbt<nbt::tag::CompoundTag> nbt)
{
    PositionTrackingDBServerBroadcastPacket result;
    result.action = action;
    result.trackingId = trackingId;
    result.nbt = std::move(nbt);
    return result;
}

void PositionTrackingDBServerBroadcastPacket::decodePayload(encoding::ByteBufferReader &in)
{
    action = encoding::Byte::readUnsigned(in);
    trackingId = encoding::VarInt::readSignedInt(in);
    nbt = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void PositionTrackingDBServerBroadcastPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, action);
    encoding::VarInt::writeSignedInt(out, trackingId);
    out.writeByteArray(nbt.getEncodedNbt());

}

bool PositionTrackingDBServerBroadcastPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePositionTrackingDBServerBroadcast(*this);
}

}  // namespace bedrock_protocol
