/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/LevelSoundEventPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/LevelSoundEventPacket.h"

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

LevelSoundEventPacket LevelSoundEventPacket::create(std::string sound, math::Vector3 position, std::int32_t extraData, std::string entityType, bool isBabyMob, bool disableRelativeVolume, std::int64_t actorUniqueId, std::optional<math::Vector3> firePosition)
{
    LevelSoundEventPacket result;
    result.sound = std::move(sound);
    result.position = std::move(position);
    result.extraData = extraData;
    result.entityType = std::move(entityType);
    result.isBabyMob = isBabyMob;
    result.disableRelativeVolume = disableRelativeVolume;
    result.actorUniqueId = actorUniqueId;
    result.firePosition = std::move(firePosition);
    return result;
}

LevelSoundEventPacket LevelSoundEventPacket::nonActorSound(std::string sound, math::Vector3 position, bool disableRelativeVolume, std::int32_t extraData)
{
    return create(sound, position, extraData, ":", false, disableRelativeVolume, -1, std::nullopt);
}

void LevelSoundEventPacket::decodePayload(encoding::ByteBufferReader &in)
{
    sound = serializer::CommonTypes::getString(in);
    position = serializer::CommonTypes::getVector3(in);
    extraData = encoding::VarInt::readSignedInt(in);
    entityType = serializer::CommonTypes::getString(in);
    isBabyMob = serializer::CommonTypes::getBool(in);
    disableRelativeVolume = serializer::CommonTypes::getBool(in);
    actorUniqueId = encoding::LE::readSignedLong(in); //WHY IS THIS NON-STANDARD?
    firePosition = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getVector3(reader); });

}

void LevelSoundEventPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putString(out, sound);
    serializer::CommonTypes::putVector3(out, position);
    encoding::VarInt::writeSignedInt(out, extraData);
    serializer::CommonTypes::putString(out, entityType);
    serializer::CommonTypes::putBool(out, isBabyMob);
    serializer::CommonTypes::putBool(out, disableRelativeVolume);
    encoding::LE::writeSignedLong(out, actorUniqueId);
    serializer::CommonTypes::writeOptional(out, firePosition, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putVector3(writer, value); });

}

bool LevelSoundEventPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleLevelSoundEvent(*this);
}

}  // namespace bedrock_protocol
