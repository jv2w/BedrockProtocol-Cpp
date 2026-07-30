/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/InventoryContentPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/InventoryContentPacket.h"

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

InventoryContentPacket InventoryContentPacket::create(std::uint32_t windowId, std::vector<types::inventory::ItemStackWrapper> items, std::optional<types::inventory::FullContainerName> containerName, std::optional<types::inventory::ItemStackWrapper> storage)
{
    InventoryContentPacket result;
    result.windowId = windowId;
    result.items = std::move(items);
    result.containerName = std::move(containerName);
    result.storage = std::move(storage);
    return result;
}

void InventoryContentPacket::decodePayload(encoding::ByteBufferReader &in)
{
    windowId = encoding::VarInt::readUnsignedInt(in);
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; ++i) {
        items.push_back(serializer::CommonTypes::getNetworkItemStackDescriptor(in));
    }
    containerName = types::inventory::FullContainerName::read(in);
    storage = serializer::CommonTypes::getNetworkItemStackDescriptor(in);

}

void InventoryContentPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, windowId);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(items.size()));
    for (const auto &item : items) {
        serializer::CommonTypes::putNetworkItemStackDescriptor(out, item);
    }
    containerName.value().write(out);
    serializer::CommonTypes::putNetworkItemStackDescriptor(out, storage.value());

}

bool InventoryContentPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleInventoryContent(*this);
}

}  // namespace bedrock_protocol
