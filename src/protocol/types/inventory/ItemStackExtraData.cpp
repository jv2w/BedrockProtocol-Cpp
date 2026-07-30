/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/ItemStackExtraData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/inventory/ItemStackExtraData.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/nbt/LittleEndianNbtSerializer.h"
#include "bedrock_protocol/nbt/NbtDataException.h"
#include "bedrock_protocol/nbt/TreeRoot.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::inventory {

using encoding::Byte;
using encoding::LE;

ItemStackExtraData ItemStackExtraData::read(encoding::ByteBufferReader &in)
{
    const auto nbtLen = LE::readSignedShort(in);

    std::optional<::bedrock_protocol::nbt::tag::CompoundTag> compound;
    if (nbtLen == -1) {
        const auto nbtDataVersion = Byte::readUnsigned(in);
        if (nbtDataVersion != 1) {
            throw PacketDecodeException("Unexpected NBT data version " + std::to_string(nbtDataVersion));
        }
        //Deviation from PHP: the PHP original decodes from a copy of the buffer with a by-ref offset and
        //restores that offset onto the reader in a finally block. This port's serializer reads straight
        //through the reader, so the reader is already positioned past the NBT (or at the failure point).
        try {
            compound = ::bedrock_protocol::nbt::LittleEndianNbtSerializer().read(in, 512).mustGetCompoundTag();
        }
        catch (const ::bedrock_protocol::nbt::NbtDataException &e) {
            throw PacketDecodeException::wrap(e, "Failed decoding NBT root");
        }
    }
    else if (nbtLen != 0) {
        throw PacketDecodeException("Unexpected fake NBT length " + std::to_string(nbtLen));
    }

    std::vector<std::string> canPlaceOn;
    //TODO: apparently this is not correct as of 1.21.50
    for (std::uint32_t i = 0, canPlaceOnCount = LE::readUnsignedInt(in); i < canPlaceOnCount; ++i) {
        canPlaceOn.emplace_back(in.readByteArray(LE::readUnsignedShort(in)));
    }

    std::vector<std::string> canDestroy;
    for (std::uint32_t i = 0, canDestroyCount = LE::readUnsignedInt(in); i < canDestroyCount; ++i) {
        canDestroy.emplace_back(in.readByteArray(LE::readUnsignedShort(in)));
    }

    return {std::move(compound), std::move(canPlaceOn), std::move(canDestroy)};
}

void ItemStackExtraData::write(encoding::ByteBufferWriter &out) const
{
    if (nbt.has_value()) {
        LE::writeSignedShort(out, static_cast<std::int16_t>(0xffff));
        Byte::writeUnsigned(out, 1);  //TODO: NBT data version (?)
        out.writeByteArray(::bedrock_protocol::nbt::LittleEndianNbtSerializer().write(
            ::bedrock_protocol::nbt::TreeRoot(std::make_unique<::bedrock_protocol::nbt::tag::CompoundTag>(*nbt))));
    }
    else {
        LE::writeSignedShort(out, 0);
    }

    LE::writeUnsignedInt(out, static_cast<std::uint32_t>(canPlaceOn.size()));
    for (const auto &entry : canPlaceOn) {
        LE::writeUnsignedShort(out, static_cast<std::uint16_t>(entry.size()));
        out.writeByteArray(entry);
    }
    LE::writeUnsignedInt(out, static_cast<std::uint32_t>(canDestroy.size()));
    for (const auto &entry : canDestroy) {
        LE::writeUnsignedShort(out, static_cast<std::uint16_t>(entry.size()));
        out.writeByteArray(entry);
    }
}

}  // namespace bedrock_protocol::types::inventory
