/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/AttributeModifier.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/entity/AttributeModifier.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types::entity {

using encoding::LE;
using serializer::CommonTypes;

AttributeModifier AttributeModifier::read(encoding::ByteBufferReader &in)
{
    std::string id{CommonTypes::getString(in)};
    std::string name{CommonTypes::getString(in)};
    const auto amount = LE::readFloat(in);
    const auto operation = LE::readSignedInt(in);
    const auto operand = LE::readSignedInt(in);
    const auto serializable = CommonTypes::getBool(in);

    return AttributeModifier(std::move(id), std::move(name), amount, operation, operand, serializable);
}

void AttributeModifier::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, id);
    CommonTypes::putString(out, name);
    LE::writeFloat(out, amount);
    LE::writeSignedInt(out, operation);
    LE::writeSignedInt(out, operand);
    CommonTypes::putBool(out, serializable);
}

}  // namespace bedrock_protocol::types::entity
