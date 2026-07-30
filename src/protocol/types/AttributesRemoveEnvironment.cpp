/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributesRemoveEnvironment.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributesRemoveEnvironment.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

AttributesRemoveEnvironment AttributesRemoveEnvironment::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    const auto dimension = VarInt::readUnsignedInt(in);

    std::vector<std::string> attributes;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        attributes.push_back(CommonTypes::getString(in));
    }

    return AttributesRemoveEnvironment(std::move(name), dimension, std::move(attributes));
}

void AttributesRemoveEnvironment::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    VarInt::writeUnsignedInt(out, dimension);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(attributes.size()));
    for (const auto &attribute : attributes) {
        CommonTypes::putString(out, attribute);
    }
}

}  // namespace bedrock_protocol::types
