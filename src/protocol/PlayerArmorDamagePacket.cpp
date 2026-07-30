/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerArmorDamagePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerArmorDamagePacket.h"

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

PlayerArmorDamagePacket PlayerArmorDamagePacket::create(std::vector<types::ArmorSlotAndDamagePair> armorSlotAndDamagePairs)
{
    PlayerArmorDamagePacket result;
    result.armorSlotAndDamagePairs = std::move(armorSlotAndDamagePairs);
    return result;
}

void PlayerArmorDamagePacket::decodePayload(encoding::ByteBufferReader &in)
{
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        armorSlotAndDamagePairs.push_back(types::ArmorSlotAndDamagePair::read(in));
    }

}

void PlayerArmorDamagePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(armorSlotAndDamagePairs.size()));
    for (const auto &pair : armorSlotAndDamagePairs) {
        pair.write(out);
    }

}

bool PlayerArmorDamagePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerArmorDamage(*this);
}

}  // namespace bedrock_protocol
