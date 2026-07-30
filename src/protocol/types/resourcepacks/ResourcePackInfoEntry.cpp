/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/resourcepacks/ResourcePackInfoEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/resourcepacks/ResourcePackInfoEntry.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::resourcepacks {

using encoding::LE;
using serializer::CommonTypes;

void ResourcePackInfoEntry::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putUUID(out, packId);
    CommonTypes::putString(out, version);
    LE::writeUnsignedLong(out, sizeBytes);
    CommonTypes::putString(out, encryptionKey);
    CommonTypes::putString(out, subPackName);
    CommonTypes::putString(out, contentId);
    CommonTypes::putBool(out, hasScripts_);
    CommonTypes::putBool(out, isAddonPack_);
    CommonTypes::putBool(out, isRtxCapable_);
    CommonTypes::putString(out, cdnUrl);
}

ResourcePackInfoEntry ResourcePackInfoEntry::read(encoding::ByteBufferReader &in)
{
    const auto uuid = CommonTypes::getUUID(in);
    auto version = CommonTypes::getString(in);
    const auto sizeBytes = LE::readUnsignedLong(in);
    auto encryptionKey = CommonTypes::getString(in);
    auto subPackName = CommonTypes::getString(in);
    auto contentId = CommonTypes::getString(in);
    const auto hasScripts = CommonTypes::getBool(in);
    const auto isAddonPack = CommonTypes::getBool(in);
    const auto rtxCapable = CommonTypes::getBool(in);
    auto cdnUrl = CommonTypes::getString(in);
    return ResourcePackInfoEntry(uuid, std::move(version), sizeBytes, std::move(encryptionKey),
                                 std::move(subPackName), std::move(contentId), hasScripts, isAddonPack, rtxCapable,
                                 std::move(cdnUrl));
}

}  // namespace bedrock_protocol::types::resourcepacks
