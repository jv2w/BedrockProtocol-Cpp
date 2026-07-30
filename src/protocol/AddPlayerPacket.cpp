/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AddPlayerPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddPlayerPacket.h"

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

AddPlayerPacket AddPlayerPacket::create(uuid::Uuid uuid, std::string username, std::uint64_t actorRuntimeId, std::string platformChatId, math::Vector3 position, std::optional<math::Vector3> motion, float pitch, float yaw, float headYaw, types::inventory::ItemStackWrapper item, std::int32_t gameMode, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, UpdateAbilitiesPacket abilitiesPacket, std::vector<types::entity::EntityLink> links, std::string deviceId, std::int32_t buildPlatform)
{
    AddPlayerPacket result;
    result.uuid = std::move(uuid);
    result.username = std::move(username);
    result.actorRuntimeId = actorRuntimeId;
    result.platformChatId = std::move(platformChatId);
    result.position = std::move(position);
    result.motion = std::move(motion);
    result.pitch = pitch;
    result.yaw = yaw;
    result.headYaw = headYaw;
    result.item = std::move(item);
    result.gameMode = gameMode;
    result.metadata = std::move(metadata);
    result.syncedProperties = std::move(syncedProperties);
    result.abilitiesPacket = std::move(abilitiesPacket);
    result.links = std::move(links);
    result.deviceId = std::move(deviceId);
    result.buildPlatform = buildPlatform;
    return result;
}

void AddPlayerPacket::decodePayload(encoding::ByteBufferReader &in)
{
    uuid = serializer::CommonTypes::getUUID(in);
    username = serializer::CommonTypes::getString(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    platformChatId = serializer::CommonTypes::getString(in);
    position = serializer::CommonTypes::getVector3(in);
    motion = serializer::CommonTypes::getVector3(in);
    pitch = encoding::LE::readFloat(in);
    yaw = encoding::LE::readFloat(in);
    headYaw = encoding::LE::readFloat(in);
    item = serializer::CommonTypes::getItemStackWrapper(in);
    gameMode = encoding::VarInt::readSignedInt(in);
    metadata = serializer::CommonTypes::getEntityMetadata(in);
    syncedProperties = types::entity::PropertySyncData::read(in);

    abilitiesPacket = UpdateAbilitiesPacket();
    abilitiesPacket.decodeBody(in);

    const auto linkCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < linkCount; ++i) {
        links.push_back(serializer::CommonTypes::getEntityLink(in));
    }

    deviceId = serializer::CommonTypes::getString(in);
    buildPlatform = encoding::LE::readSignedInt(in);

}

void AddPlayerPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putUUID(out, uuid);
    serializer::CommonTypes::putString(out, username);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putString(out, platformChatId);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putVector3Nullable(out, motion);
    encoding::LE::writeFloat(out, pitch);
    encoding::LE::writeFloat(out, yaw);
    encoding::LE::writeFloat(out, headYaw);
    serializer::CommonTypes::putItemStackWrapper(out, item);
    encoding::VarInt::writeSignedInt(out, gameMode);
    serializer::CommonTypes::putEntityMetadata(out, metadata);
    syncedProperties.write(out);

    abilitiesPacket.encodeBody(out);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(links.size()));
    for (const auto &link : links) {
        serializer::CommonTypes::putEntityLink(out, link);
    }

    serializer::CommonTypes::putString(out, deviceId);
    encoding::LE::writeSignedInt(out, buildPlatform);

}

bool AddPlayerPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddPlayer(*this);
}

}  // namespace bedrock_protocol
