/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SetHudPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SetHudPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

SetHudPacket SetHudPacket::create(std::vector<types::hud::HudElement> hudElements, types::hud::HudVisibility visibility)
{
    SetHudPacket result;
    result.hudElements = std::move(hudElements);
    result.visibility = visibility;
    return result;
}

void SetHudPacket::decodePayload(encoding::ByteBufferReader &in)
{
    hudElements.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        hudElements.push_back(types::hud::HudElementFromPacket(encoding::VarInt::readSignedInt(in)));
    }
    visibility = types::hud::HudVisibilityFromPacket(encoding::VarInt::readSignedInt(in));

}

void SetHudPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(hudElements.size()));
    for (const auto &element : hudElements) {
        encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(element));
    }
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(visibility));

}

bool SetHudPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSetHud(*this);
}

}  // namespace bedrock_protocol
