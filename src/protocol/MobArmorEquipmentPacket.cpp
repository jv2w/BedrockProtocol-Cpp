/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/MobArmorEquipmentPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/MobArmorEquipmentPacket.h"

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

MobArmorEquipmentPacket MobArmorEquipmentPacket::create(std::uint64_t actorRuntimeId, types::inventory::ItemStackWrapper head, types::inventory::ItemStackWrapper chest, types::inventory::ItemStackWrapper legs, types::inventory::ItemStackWrapper feet, types::inventory::ItemStackWrapper body)
{
    MobArmorEquipmentPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.head = std::move(head);
    result.chest = std::move(chest);
    result.legs = std::move(legs);
    result.feet = std::move(feet);
    result.body = std::move(body);
    return result;
}

void MobArmorEquipmentPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    head = serializer::CommonTypes::getNetworkItemStackDescriptor(in);
    chest = serializer::CommonTypes::getNetworkItemStackDescriptor(in);
    legs = serializer::CommonTypes::getNetworkItemStackDescriptor(in);
    feet = serializer::CommonTypes::getNetworkItemStackDescriptor(in);
    body = serializer::CommonTypes::getNetworkItemStackDescriptor(in);

}

void MobArmorEquipmentPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, head);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, chest);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, legs);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, feet);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, body);

}

bool MobArmorEquipmentPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleMobArmorEquipment(*this);
}

}  // namespace bedrock_protocol
