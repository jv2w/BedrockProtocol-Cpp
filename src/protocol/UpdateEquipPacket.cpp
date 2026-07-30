/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UpdateEquipPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UpdateEquipPacket.h"

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

UpdateEquipPacket UpdateEquipPacket::create(std::uint8_t windowId, std::uint8_t windowType, std::int32_t windowSlotCount, std::int64_t actorUniqueId, types::CacheableNbt<nbt::tag::CompoundTag> nbt)
{
    UpdateEquipPacket result;
    result.windowId = windowId;
    result.windowType = windowType;
    result.windowSlotCount = windowSlotCount;
    result.actorUniqueId = actorUniqueId;
    result.nbt = std::move(nbt);
    return result;
}

void UpdateEquipPacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::Byte::readUnsigned(in);
    windowType = encoding::Byte::readUnsigned(in);
    windowSlotCount = encoding::VarInt::readSignedInt(in);
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    nbt = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void UpdateEquipPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, windowId);
    encoding::Byte::writeUnsigned(out, windowType);
    encoding::VarInt::writeSignedInt(out, windowSlotCount);
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    out.writeByteArray(nbt.getEncodedNbt());

}

bool UpdateEquipPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUpdateEquip(*this);
}

}  // namespace bedrock_protocol
