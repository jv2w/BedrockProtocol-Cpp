/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/PlayerToggleCrafterSlotRequestPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerToggleCrafterSlotRequestPacket.h"

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

PlayerToggleCrafterSlotRequestPacket PlayerToggleCrafterSlotRequestPacket::create(types::BlockPosition position, std::uint8_t slot, bool disabled)
{
    PlayerToggleCrafterSlotRequestPacket result;
    result.position = std::move(position);
    result.slot = slot;
    result.disabled = disabled;
    return result;
}

void PlayerToggleCrafterSlotRequestPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto x = encoding::LE::readSignedInt(in);
    const auto y = encoding::LE::readSignedInt(in);
    const auto z = encoding::LE::readSignedInt(in);
    position = types::BlockPosition(x, y, z);
    slot = encoding::Byte::readUnsigned(in);
    disabled = serializer::CommonTypes::getBool(in);

}

void PlayerToggleCrafterSlotRequestPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeSignedInt(out, position.getX());
    encoding::LE::writeSignedInt(out, position.getY());
    encoding::LE::writeSignedInt(out, position.getZ());
    encoding::Byte::writeUnsigned(out, slot);
    serializer::CommonTypes::putBool(out, disabled);

}

bool PlayerToggleCrafterSlotRequestPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerToggleCrafterSlotRequest(*this);
}

}  // namespace bedrock_protocol
