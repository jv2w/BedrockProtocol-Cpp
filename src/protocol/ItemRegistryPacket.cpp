/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ItemRegistryPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ItemRegistryPacket.h"

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

ItemRegistryPacket ItemRegistryPacket::create(std::vector<types::ItemTypeEntry> entries)
{
    ItemRegistryPacket result;
    result.entries = std::move(entries);
    return result;
}

void ItemRegistryPacket::decodePayload(encoding::ByteBufferReader &in)
{
    entries.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        const auto stringId = serializer::CommonTypes::getString(in);
        const auto numericId = encoding::LE::readSignedShort(in);
        const auto isComponentBased = serializer::CommonTypes::getBool(in);
        const auto version = encoding::VarInt::readSignedInt(in);
        const auto nbt = serializer::CommonTypes::getNbtCompoundRoot(in);
        entries.push_back(types::ItemTypeEntry(stringId, numericId, isComponentBased, version, types::CacheableNbt<nbt::tag::CompoundTag>(nbt)));
    }

}

void ItemRegistryPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(entries.size()));
    for (const auto &entry : entries) {
        serializer::CommonTypes::putString(out, entry.getStringId());
        encoding::LE::writeSignedShort(out, entry.getNumericId());
        serializer::CommonTypes::putBool(out, entry.isComponentBased());
        encoding::VarInt::writeSignedInt(out, entry.getVersion());
        out.writeByteArray(entry.getComponentNbt().getEncodedNbt());
    }

}

bool ItemRegistryPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleItemRegistry(*this);
}

}  // namespace bedrock_protocol
