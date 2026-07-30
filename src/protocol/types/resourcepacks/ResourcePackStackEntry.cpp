/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/resourcepacks/ResourcePackStackEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/resourcepacks/ResourcePackStackEntry.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::resourcepacks {

using serializer::CommonTypes;

void ResourcePackStackEntry::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, packId);
    CommonTypes::putString(out, version);
    CommonTypes::putString(out, subPackName);
}

ResourcePackStackEntry ResourcePackStackEntry::read(encoding::ByteBufferReader &in)
{
    auto packId = CommonTypes::getString(in);
    auto version = CommonTypes::getString(in);
    auto subPackName = CommonTypes::getString(in);
    return ResourcePackStackEntry(std::move(packId), std::move(version), std::move(subPackName));
}

}  // namespace bedrock_protocol::types::resourcepacks
