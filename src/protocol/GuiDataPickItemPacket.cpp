/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/GuiDataPickItemPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/GuiDataPickItemPacket.h"

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

GuiDataPickItemPacket GuiDataPickItemPacket::create(std::string itemDescription, std::string itemEffects, std::int32_t hotbarSlot)
{
    GuiDataPickItemPacket result;
    result.itemDescription = std::move(itemDescription);
    result.itemEffects = std::move(itemEffects);
    result.hotbarSlot = hotbarSlot;
    return result;
}

void GuiDataPickItemPacket::decodePayload(encoding::ByteBufferReader &in)
{
    itemDescription = serializer::CommonTypes::getString(in);
    itemEffects = serializer::CommonTypes::getString(in);
    hotbarSlot = encoding::LE::readSignedInt(in);

}

void GuiDataPickItemPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, itemDescription);
    serializer::CommonTypes::putString(out, itemEffects);
    encoding::LE::writeSignedInt(out, hotbarSlot);

}

bool GuiDataPickItemPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleGuiDataPickItem(*this);
}

}  // namespace bedrock_protocol
