/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayerSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeLayerSettings.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeightFloat.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeightString.h"

namespace bedrock_protocol::types {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

AttributeLayerSettings AttributeLayerSettings::read(encoding::ByteBufferReader &in)
{
    const auto priority = LE::readSignedInt(in);
    std::unique_ptr<AttributeLayerSettingsWeight> weight;
    switch (static_cast<std::int32_t>(VarInt::readUnsignedInt(in))) {
    case AttributeLayerSettingsWeightFloat::ID:
        weight = std::make_unique<AttributeLayerSettingsWeightFloat>(AttributeLayerSettingsWeightFloat::read(in));
        break;
    case AttributeLayerSettingsWeightString::ID:
        weight = std::make_unique<AttributeLayerSettingsWeightString>(AttributeLayerSettingsWeightString::read(in));
        break;
    default:
        throw PacketDecodeException("Unknown AttributeLayerSettingsWeight type");
    }
    const auto enabled = CommonTypes::getBool(in);
    const auto transitionsPaused = CommonTypes::getBool(in);

    return AttributeLayerSettings(priority, std::move(weight), enabled, transitionsPaused);
}

void AttributeLayerSettings::write(encoding::ByteBufferWriter &out) const
{
    LE::writeSignedInt(out, priority);
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(weight->getTypeId()));
    weight->write(out);
    CommonTypes::putBool(out, enabled);
    CommonTypes::putBool(out, transitionsPaused);
}

}  // namespace bedrock_protocol::types
