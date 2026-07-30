/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AbilitiesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AbilitiesData.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;

AbilitiesData AbilitiesData::decode(encoding::ByteBufferReader &in)
{
    const auto targetActorUniqueId = LE::readSignedLong(in);  //WHY IS THIS NON-STANDARD?
    const auto playerPermission = Byte::readUnsigned(in);
    const auto commandPermission = Byte::readUnsigned(in);

    std::vector<AbilitiesLayer> abilityLayers;
    for (std::uint8_t i = 0, len = Byte::readUnsigned(in); i < len; i++) {
        abilityLayers.push_back(AbilitiesLayer::decode(in));
    }

    return AbilitiesData(commandPermission, playerPermission, targetActorUniqueId, std::move(abilityLayers));
}

void AbilitiesData::encode(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedLong(out, targetActorUniqueId);
    Byte::writeUnsigned(out, playerPermission);
    Byte::writeUnsigned(out, commandPermission);

    Byte::writeUnsigned(out, static_cast<std::uint8_t>(abilityLayers.size()));
    for (const auto &abilityLayer : abilityLayers) {
        abilityLayer.encode(out);
    }
}

}  // namespace bedrock_protocol::types
