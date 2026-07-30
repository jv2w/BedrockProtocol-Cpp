/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BossEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BossEventPacket.h"

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

BossEventPacket BossEventPacket::base(std::int64_t bossActorUniqueId, std::int32_t eventId)
{
    BossEventPacket result;
    result.bossActorUniqueId = bossActorUniqueId;
    result.eventType = eventId;
    return result;
}

BossEventPacket BossEventPacket::show(std::int64_t bossActorUniqueId, std::string title, float healthPercent, std::uint8_t color, std::uint8_t overlay)
{
    auto result = base(bossActorUniqueId, TYPE_SHOW);
    result.title = std::move(title);
    result.filteredTitle = std::move(title);
    result.healthPercent = healthPercent;
    result.color = color;
    result.overlay = overlay;
    return result;
}

BossEventPacket BossEventPacket::hide(std::int64_t bossActorUniqueId)
{
    return base(bossActorUniqueId, TYPE_HIDE);
}

BossEventPacket BossEventPacket::registerPlayer(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId)
{
    auto result = base(bossActorUniqueId, TYPE_REGISTER_PLAYER);
    result.playerActorUniqueId = playerActorUniqueId;
    return result;
}

BossEventPacket BossEventPacket::unregisterPlayer(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId)
{
    auto result = base(bossActorUniqueId, TYPE_UNREGISTER_PLAYER);
    result.playerActorUniqueId = playerActorUniqueId;
    return result;
}

BossEventPacket BossEventPacket::makeHealthPercent(std::int64_t bossActorUniqueId, float healthPercent)
{
    auto result = base(bossActorUniqueId, TYPE_HEALTH_PERCENT);
    result.healthPercent = healthPercent;
    return result;
}

BossEventPacket BossEventPacket::makeTitle(std::int64_t bossActorUniqueId, std::string title)
{
    auto result = base(bossActorUniqueId, TYPE_TITLE);
    result.title = std::move(title);
    result.filteredTitle = std::move(title);
    return result;
}

BossEventPacket BossEventPacket::properties(std::int64_t bossActorUniqueId, std::uint8_t color, std::uint8_t overlay)
{
    auto result = base(bossActorUniqueId, TYPE_PROPERTIES);
    result.color = color;
    result.overlay = overlay;
    return result;
}

BossEventPacket BossEventPacket::query(std::int64_t bossActorUniqueId, std::int64_t playerActorUniqueId)
{
    auto result = base(bossActorUniqueId, TYPE_QUERY);
    result.playerActorUniqueId = playerActorUniqueId;
    return result;
}

void BossEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    bossActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    playerActorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    eventType = encoding::Byte::readUnsigned(in);
    title = serializer::CommonTypes::getString(in);
    filteredTitle = serializer::CommonTypes::getString(in);
    healthPercent = encoding::LE::readFloat(in);
    color = encoding::Byte::readUnsigned(in);
    overlay = encoding::Byte::readUnsigned(in);

}

void BossEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, bossActorUniqueId);
    serializer::CommonTypes::putActorUniqueId(out, playerActorUniqueId);
    encoding::Byte::writeUnsigned(out, eventType);
    serializer::CommonTypes::putString(out, title);
    serializer::CommonTypes::putString(out, filteredTitle);
    encoding::LE::writeFloat(out, healthPercent);
    encoding::Byte::writeUnsigned(out, color);
    encoding::Byte::writeUnsigned(out, overlay);

}

bool BossEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBossEvent(*this);
}

}  // namespace bedrock_protocol
