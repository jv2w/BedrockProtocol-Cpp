/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeLayerSettingsWeightFloat.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeightFloat.h"

#include "bedrock_protocol/encoding/LE.h"

namespace bedrock_protocol::types {

using encoding::LE;

AttributeLayerSettingsWeightFloat AttributeLayerSettingsWeightFloat::read(encoding::ByteBufferReader &in)
{
    const auto value = LE::readFloat(in);

    return AttributeLayerSettingsWeightFloat(value);
}

void AttributeLayerSettingsWeightFloat::write(encoding::ByteBufferWriter &out) const
{
    LE::writeFloat(out, value);
}

}  // namespace bedrock_protocol::types
