/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ResourcePackStackPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePackStackPacket.h"

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

ResourcePackStackPacket ResourcePackStackPacket::create(std::vector<types::resourcepacks::ResourcePackStackEntry> resourcePackStack, bool mustAccept, std::string baseGameVersion, types::Experiments experiments, bool useVanillaEditorPacks)
{
    ResourcePackStackPacket result;
    result.resourcePackStack = std::move(resourcePackStack);
    result.mustAccept = mustAccept;
    result.baseGameVersion = std::move(baseGameVersion);
    result.experiments = std::move(experiments);
    result.useVanillaEditorPacks = useVanillaEditorPacks;
    return result;
}

void ResourcePackStackPacket::decodePayload(encoding::ByteBufferReader &in)
{
    mustAccept = serializer::CommonTypes::getBool(in);

    auto resourcePackCount = encoding::VarInt::readUnsignedInt(in);
    while (resourcePackCount-- > 0) {
        resourcePackStack.push_back(types::resourcepacks::ResourcePackStackEntry::read(in));
    }

    baseGameVersion = serializer::CommonTypes::getString(in);
    experiments = types::Experiments::read(in);
    useVanillaEditorPacks = serializer::CommonTypes::getBool(in);

}

void ResourcePackStackPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, mustAccept);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(resourcePackStack.size()));
    for (const auto &entry : resourcePackStack) {
        entry.write(out);
    }

    serializer::CommonTypes::putString(out, baseGameVersion);
    experiments.write(out);
    serializer::CommonTypes::putBool(out, useVanillaEditorPacks);

}

bool ResourcePackStackPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePackStack(*this);
}

}  // namespace bedrock_protocol
