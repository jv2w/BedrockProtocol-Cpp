/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/InventorySlotPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/InventorySlotPacket.h"

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

InventorySlotPacket InventorySlotPacket::create(std::uint32_t windowId, std::uint32_t inventorySlot, std::optional<types::inventory::FullContainerName> containerName, std::optional<types::inventory::ItemStackWrapper> storage, types::inventory::ItemStackWrapper item)
{
    InventorySlotPacket result;
    result.windowId = windowId;
    result.inventorySlot = inventorySlot;
    result.containerName = std::move(containerName);
    result.storage = std::move(storage);
    result.item = std::move(item);
    return result;
}

void InventorySlotPacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::VarInt::readUnsignedInt(in);
    inventorySlot = encoding::VarInt::readUnsignedInt(in);
    containerName = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return types::inventory::FullContainerName::read(reader); });
    storage = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getNetworkItemStackDescriptor(reader); });
    item = serializer::CommonTypes::getNetworkItemStackDescriptor(in);

}

void InventorySlotPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, windowId);
    encoding::VarInt::writeUnsignedInt(out, inventorySlot);
    serializer::CommonTypes::writeOptional(out, containerName, [](encoding::ByteBufferWriter &out, const types::inventory::FullContainerName &v) { v.write(out); });
    serializer::CommonTypes::writeOptional(out, storage, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putNetworkItemStackDescriptor(writer, value); });
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, item);

}

bool InventorySlotPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleInventorySlot(*this);
}

}  // namespace bedrock_protocol
