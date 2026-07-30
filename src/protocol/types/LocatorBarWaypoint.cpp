/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/LocatorBarWaypoint.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/LocatorBarWaypoint.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

LocatorBarWaypoint LocatorBarWaypoint::read(encoding::ByteBufferReader &in)
{
    const auto updateFlag = LE::readUnsignedInt(in);
    auto visible = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    auto worldPosition =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return WorldPosition::read(in); });
    auto texturePath =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getString(in); });
    auto iconSize =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getVector2(in); });
    auto color = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return ::bedrock_protocol::color::Color::fromARGB(LE::readUnsignedInt(in));
    });
    auto clientPositionAuthority =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    auto actorUniqueId =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getActorUniqueId(in); });

    return LocatorBarWaypoint(updateFlag, visible, std::move(worldPosition), std::move(texturePath), iconSize, color,
                              clientPositionAuthority, actorUniqueId);
}

void LocatorBarWaypoint::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, updateFlag);
    CommonTypes::writeOptional(out, visible,
                               [](encoding::ByteBufferWriter &out, const bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::writeOptional(
        out, worldPosition, [](encoding::ByteBufferWriter &out, const WorldPosition &v) { v.write(out); });
    CommonTypes::writeOptional(
        out, texturePath, [](encoding::ByteBufferWriter &out, const std::string &v) { CommonTypes::putString(out, v); });
    CommonTypes::writeOptional(out, iconSize, [](encoding::ByteBufferWriter &out, const math::Vector2 &v) {
        CommonTypes::putVector2(out, v);
    });
    CommonTypes::writeOptional(
        out, color, [](encoding::ByteBufferWriter &out, const ::bedrock_protocol::color::Color &v) {
            LE::writeUnsignedInt(out, v.toARGB());
        });
    CommonTypes::writeOptional(out, clientPositionAuthority,
                               [](encoding::ByteBufferWriter &out, const bool v) { CommonTypes::putBool(out, v); });
    CommonTypes::writeOptional(out, actorUniqueId, [](encoding::ByteBufferWriter &out, const std::int64_t v) {
        CommonTypes::putActorUniqueId(out, v);
    });
}

}  // namespace bedrock_protocol::types
