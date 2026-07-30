/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ResourcePacksInfoPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ResourcePacksInfoPacket.h"

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

ResourcePacksInfoPacket ResourcePacksInfoPacket::create(std::vector<types::resourcepacks::ResourcePackInfoEntry> resourcePackEntries, bool mustAccept, bool hasAddons, bool hasScripts, uuid::Uuid worldTemplateId, std::string worldTemplateVersion, bool forceDisableVibrantVisuals)
{
    ResourcePacksInfoPacket result;
    result.resourcePackEntries = std::move(resourcePackEntries);
    result.mustAccept = mustAccept;
    result.hasAddons = hasAddons;
    result.hasScripts = hasScripts;
    result.worldTemplateId = std::move(worldTemplateId);
    result.worldTemplateVersion = std::move(worldTemplateVersion);
    result.forceDisableVibrantVisuals = forceDisableVibrantVisuals;
    return result;
}

void ResourcePacksInfoPacket::decodePayload(encoding::ByteBufferReader &in)
{
    mustAccept = serializer::CommonTypes::getBool(in);
    hasAddons = serializer::CommonTypes::getBool(in);
    hasScripts = serializer::CommonTypes::getBool(in);
    forceDisableVibrantVisuals = serializer::CommonTypes::getBool(in);
    worldTemplateId = serializer::CommonTypes::getUUID(in);
    worldTemplateVersion = serializer::CommonTypes::getString(in);

    auto resourcePackCount = encoding::LE::readUnsignedShort(in);
    while (resourcePackCount-- > 0) {
        resourcePackEntries.push_back(types::resourcepacks::ResourcePackInfoEntry::read(in));
    }

}

void ResourcePacksInfoPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBool(out, mustAccept);
    serializer::CommonTypes::putBool(out, hasAddons);
    serializer::CommonTypes::putBool(out, hasScripts);
    serializer::CommonTypes::putBool(out, forceDisableVibrantVisuals);
    serializer::CommonTypes::putUUID(out, worldTemplateId);
    serializer::CommonTypes::putString(out, worldTemplateVersion);
    encoding::LE::writeUnsignedShort(out, static_cast<std::uint32_t>(resourcePackEntries.size()));
    for (const auto &entry : resourcePackEntries) {
        entry.write(out);
    }

}

bool ResourcePacksInfoPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleResourcePacksInfo(*this);
}

}  // namespace bedrock_protocol
