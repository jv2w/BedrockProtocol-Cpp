/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ContainerRegistryCleanupPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ContainerRegistryCleanupPacket.h"

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

ContainerRegistryCleanupPacket ContainerRegistryCleanupPacket::create(std::vector<types::inventory::FullContainerName> removedContainers)
{
    ContainerRegistryCleanupPacket result;
    result.removedContainers = std::move(removedContainers);
    return result;
}

void ContainerRegistryCleanupPacket::decodePayload(encoding::ByteBufferReader &in)
{
    removedContainers.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        removedContainers.push_back(types::inventory::FullContainerName::read(in));
    }

}

void ContainerRegistryCleanupPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(removedContainers.size()));
    for (const auto &container : removedContainers) {
        container.write(out);
    }

}

bool ContainerRegistryCleanupPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleContainerRegistryCleanup(*this);
}

}  // namespace bedrock_protocol
