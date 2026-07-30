/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/MobEquipmentPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MobEquipmentPacket.h"

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

MobEquipmentPacket MobEquipmentPacket::create(std::uint64_t actorRuntimeId, types::inventory::ItemStackWrapper item, std::uint8_t inventorySlot, std::uint8_t hotbarSlot, std::uint8_t windowId)
{
    MobEquipmentPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.item = std::move(item);
    result.inventorySlot = inventorySlot;
    result.hotbarSlot = hotbarSlot;
    result.windowId = windowId;
    return result;
}

void MobEquipmentPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    item = serializer::CommonTypes::getNetworkItemStackDescriptor(in);
    inventorySlot = encoding::Byte::readUnsigned(in);
    hotbarSlot = encoding::Byte::readUnsigned(in);
    windowId = encoding::Byte::readUnsigned(in);

}

void MobEquipmentPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, item);
    encoding::Byte::writeUnsigned(out, inventorySlot);
    encoding::Byte::writeUnsigned(out, hotbarSlot);
    encoding::Byte::writeUnsigned(out, windowId);

}

bool MobEquipmentPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMobEquipment(*this);
}

}  // namespace bedrock_protocol
