/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/UpdateAttribute.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/entity/UpdateAttribute.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::entity {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

UpdateAttribute UpdateAttribute::read(encoding::ByteBufferReader &in)
{
    const auto min = LE::readFloat(in);
    const auto max = LE::readFloat(in);
    const auto current = LE::readFloat(in);
    const auto defaultMin = LE::readFloat(in);
    const auto defaultMax = LE::readFloat(in);
    const auto defaultValue = LE::readFloat(in);
    std::string id{CommonTypes::getString(in)};

    std::vector<AttributeModifier> modifiers;
    for (std::uint32_t j = 0, modifierCount = VarInt::readUnsignedInt(in); j < modifierCount; j++) {
        modifiers.push_back(AttributeModifier::read(in));
    }

    return UpdateAttribute(std::move(id), min, max, current, defaultMin, defaultMax, defaultValue,
                           std::move(modifiers));
}

void UpdateAttribute::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, min);
    LE::writeFloat(out, max);
    LE::writeFloat(out, current);
    LE::writeFloat(out, defaultMin);
    LE::writeFloat(out, defaultMax);
    LE::writeFloat(out, defaultValue);
    CommonTypes::putString(out, id);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(modifiers.size()));
    for (const auto &modifier : modifiers) {
        modifier.write(out);
    }
}

}  // namespace bedrock_protocol::types::entity
