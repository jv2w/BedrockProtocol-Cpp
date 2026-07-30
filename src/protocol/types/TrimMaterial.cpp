/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/TrimMaterial.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/TrimMaterial.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

TrimMaterial TrimMaterial::read(encoding::ByteBufferReader &in)
{
    auto materialId = CommonTypes::getString(in);
    auto color = CommonTypes::getString(in);
    auto itemId = CommonTypes::getString(in);
    return TrimMaterial(std::move(materialId), std::move(color), std::move(itemId));
}

void TrimMaterial::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, materialId);
    CommonTypes::putString(out, color);
    CommonTypes::putString(out, itemId);
}

}  // namespace bedrock_protocol::types
