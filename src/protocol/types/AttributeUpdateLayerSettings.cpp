/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeUpdateLayerSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeUpdateLayerSettings.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

AttributeUpdateLayerSettings AttributeUpdateLayerSettings::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    const auto dimension = VarInt::readUnsignedInt(in);
    auto settings = AttributeLayerSettings::read(in);

    return AttributeUpdateLayerSettings(std::move(name), dimension, std::move(settings));
}

void AttributeUpdateLayerSettings::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    VarInt::writeUnsignedInt(out, dimension);
    settings.write(out);
}

}  // namespace bedrock_protocol::types
