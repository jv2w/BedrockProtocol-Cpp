/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/PropertySyncData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/entity/PropertySyncData.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"

namespace bedrock_protocol::types::entity {

using encoding::LE;
using encoding::VarInt;

PropertySyncData PropertySyncData::read(encoding::ByteBufferReader &in)
{
    std::vector<std::pair<std::uint32_t, std::int32_t>> intProperties;
    std::vector<std::pair<std::uint32_t, float>> floatProperties;

    for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; ++i) {
        // the key is read before the value, matching PHP's evaluation order for $array[key] = value
        const auto key = VarInt::readUnsignedInt(in);
        intProperties.emplace_back(key, VarInt::readSignedInt(in));
    }
    for (std::uint32_t i = 0, count = VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto key = VarInt::readUnsignedInt(in);
        floatProperties.emplace_back(key, LE::readFloat(in));
    }

    return PropertySyncData(std::move(intProperties), std::move(floatProperties));
}

void PropertySyncData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(intProperties.size()));
    for (const auto &[key, value] : intProperties) {
        VarInt::writeUnsignedInt(out, key);
        VarInt::writeSignedInt(out, value);
    }
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(floatProperties.size()));
    for (const auto &[key, value] : floatProperties) {
        VarInt::writeUnsignedInt(out, key);
        LE::writeFloat(out, value);
    }
}

}  // namespace bedrock_protocol::types::entity
