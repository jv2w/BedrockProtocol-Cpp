/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetPlayerInventoryOptionsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetPlayerInventoryOptionsPacket.h"

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

SetPlayerInventoryOptionsPacket SetPlayerInventoryOptionsPacket::create(types::inventory::InventoryLeftTab leftTab, types::inventory::InventoryRightTab rightTab, bool filtering, types::inventory::InventoryLayout inventoryLayout, types::inventory::InventoryLayout craftingLayout)
{
    SetPlayerInventoryOptionsPacket result;
    result.leftTab = leftTab;
    result.rightTab = rightTab;
    result.filtering = filtering;
    result.inventoryLayout = inventoryLayout;
    result.craftingLayout = craftingLayout;
    return result;
}

void SetPlayerInventoryOptionsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    leftTab = types::inventory::InventoryLeftTabFromPacket(encoding::VarInt::readSignedInt(in));
    rightTab = types::inventory::InventoryRightTabFromPacket(encoding::VarInt::readSignedInt(in));
    filtering = serializer::CommonTypes::getBool(in);
    inventoryLayout = types::inventory::InventoryLayoutFromPacket(encoding::VarInt::readSignedInt(in));
    craftingLayout = types::inventory::InventoryLayoutFromPacket(encoding::VarInt::readSignedInt(in));

}

void SetPlayerInventoryOptionsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(leftTab));
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(rightTab));
    serializer::CommonTypes::putBool(out, filtering);
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(inventoryLayout));
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(craftingLayout));

}

bool SetPlayerInventoryOptionsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetPlayerInventoryOptions(*this);
}

}  // namespace bedrock_protocol
