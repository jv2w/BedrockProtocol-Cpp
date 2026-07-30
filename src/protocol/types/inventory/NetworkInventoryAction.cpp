/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/NetworkInventoryAction.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/NetworkInventoryAction.h"

#include <stdexcept>
#include <string>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
using encoding::VarInt;
using serializer::CommonTypes;

NetworkInventoryAction &NetworkInventoryAction::readAuthInput(encoding::ByteBufferReader &in)
{
    sourceType = VarInt::readUnsignedInt(in);

    switch (sourceType) {
    case SOURCE_CONTAINER:
        windowId = VarInt::readSignedInt(in);
        break;
    case SOURCE_WORLD:
        sourceFlags = VarInt::readUnsignedInt(in);
        break;
    case SOURCE_CREATIVE:
        break;
    case SOURCE_TODO:
        windowId = VarInt::readSignedInt(in);
        break;
    default:
        throw PacketDecodeException("Unknown inventory action source type " + std::to_string(sourceType));
    }

    inventorySlot = VarInt::readUnsignedInt(in);
    oldItem = CommonTypes::getItemStackWrapper(in);
    newItem = CommonTypes::getItemStackWrapper(in);

    return *this;
}

void NetworkInventoryAction::writeAuthInput(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, sourceType);

    switch (sourceType) {
    case SOURCE_CONTAINER:
        if (!windowId.has_value()) {
            throw std::logic_error("WindowID must be set for SOURCE_CONTAINER");
        }
        VarInt::writeSignedInt(out, *windowId);
        break;
    case SOURCE_WORLD:
        if (!sourceFlags.has_value()) {
            throw std::logic_error("SourceFlags must be set for SOURCE_WORLD");
        }
        VarInt::writeUnsignedInt(out, *sourceFlags);
        break;
    case SOURCE_CREATIVE:
        break;
    case SOURCE_TODO:
        if (!windowId.has_value()) {
            throw std::logic_error("WindowID must be set for SOURCE_TODO");
        }
        VarInt::writeSignedInt(out, *windowId);
        break;
    default:
        throw std::invalid_argument("Unknown inventory action source type " + std::to_string(sourceType));
    }

    VarInt::writeUnsignedInt(out, inventorySlot);
    CommonTypes::putItemStackWrapper(out, oldItem);
    CommonTypes::putItemStackWrapper(out, newItem);
}

NetworkInventoryAction &NetworkInventoryAction::readTransaction(encoding::ByteBufferReader &in)
{
    sourceType = VarInt::readUnsignedInt(in);

    if (Byte::readUnsigned(in) != 1) {
        throw PacketDecodeException("Inconsistent optional state for windowId");
    }
    windowId = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &in) { return static_cast<std::int32_t>(Byte::readSigned(in)); });

    if (Byte::readUnsigned(in) != 1) {
        throw PacketDecodeException("Inconsistent optional state for sourceFlags");
    }
    sourceFlags = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return VarInt::readUnsignedInt(in); });

    inventorySlot = VarInt::readUnsignedInt(in);
    oldItem = CommonTypes::getNetworkItemStackDescriptor(in);
    newItem = CommonTypes::getNetworkItemStackDescriptor(in);

    return *this;
}

void NetworkInventoryAction::writeTransaction(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, sourceType);

    Byte::writeUnsigned(out, 1);
    CommonTypes::writeOptional(out, windowId, [](encoding::ByteBufferWriter &out, std::int32_t value) {
        Byte::writeSigned(out, static_cast<std::int8_t>(value));
    });

    Byte::writeUnsigned(out, 1);
    CommonTypes::writeOptional(out, sourceFlags,
                               [](encoding::ByteBufferWriter &out, std::uint32_t value) { VarInt::writeUnsignedInt(out, value); });

    VarInt::writeUnsignedInt(out, inventorySlot);
    CommonTypes::putNetworkItemStackDescriptor(out, oldItem);
    CommonTypes::putNetworkItemStackDescriptor(out, newItem);
}

}  // namespace bedrock_protocol::types::inventory
