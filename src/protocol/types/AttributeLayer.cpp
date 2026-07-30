/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeLayer.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

AttributeLayer AttributeLayer::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    auto noiseName = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getString(in); });
    const auto dimension = VarInt::readUnsignedInt(in);
    auto settings = AttributeLayerSettings::read(in);

    std::vector<AttributeEnvironment> attributes;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        attributes.push_back(AttributeEnvironment::read(in));
    }

    return AttributeLayer(std::move(name), std::move(noiseName), dimension, std::move(settings),
                          std::move(attributes));
}

void AttributeLayer::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    // Upstream bug, reproduced verbatim to stay wire-compatible with PocketMine: read() fills
    // noiseName from this slot, but write() emits `name` again, so the optional is always present and
    // noiseName never reaches the wire. See AttributeLayer.php::write().
    CommonTypes::writeOptional(out, std::optional<std::string>(name),
                               [](encoding::ByteBufferWriter &out, const std::string &value) {
                                   CommonTypes::putString(out, value);
                               });
    VarInt::writeUnsignedInt(out, dimension);
    settings.write(out);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(attributes.size()));
    for (const auto &attribute : attributes) {
        attribute.write(out);
    }
}

}  // namespace bedrock_protocol::types
