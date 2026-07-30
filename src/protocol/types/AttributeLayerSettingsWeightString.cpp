/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayerSettingsWeightString.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeightString.h"

#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using serializer::CommonTypes;

AttributeLayerSettingsWeightString AttributeLayerSettingsWeightString::read(encoding::ByteBufferReader &in)
{
    auto value = CommonTypes::getString(in);

    return AttributeLayerSettingsWeightString(std::move(value));
}

void AttributeLayerSettingsWeightString::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, value);
}

}  // namespace bedrock_protocol::types
