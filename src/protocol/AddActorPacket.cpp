/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AddActorPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddActorPacket.h"

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

AddActorPacket AddActorPacket::create(std::int64_t actorUniqueId, std::uint64_t actorRuntimeId, std::string type, math::Vector3 position, std::optional<math::Vector3> motion, float pitch, float yaw, float headYaw, float bodyYaw, std::vector<types::entity::Attribute> attributes, serializer::CommonTypes::EntityMetadata metadata, types::entity::PropertySyncData syncedProperties, std::vector<types::entity::EntityLink> links)
{
    AddActorPacket result;
    result.actorUniqueId = actorUniqueId;
    result.actorRuntimeId = actorRuntimeId;
    result.type = std::move(type);
    result.position = std::move(position);
    result.motion = std::move(motion);
    result.pitch = pitch;
    result.yaw = yaw;
    result.headYaw = headYaw;
    result.bodyYaw = bodyYaw;
    result.attributes = std::move(attributes);
    result.metadata = std::move(metadata);
    result.syncedProperties = std::move(syncedProperties);
    result.links = std::move(links);
    return result;
}

void AddActorPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    type = serializer::CommonTypes::getString(in);
    position = serializer::CommonTypes::getVector3(in);
    motion = serializer::CommonTypes::getVector3(in);
    pitch = encoding::LE::readFloat(in);
    yaw = encoding::LE::readFloat(in);
    headYaw = encoding::LE::readFloat(in);
    bodyYaw = encoding::LE::readFloat(in);

    const auto attrCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < attrCount; ++i) {
        const auto id = serializer::CommonTypes::getString(in);
        const auto min = encoding::LE::readFloat(in);
        const auto current = encoding::LE::readFloat(in);
        const auto max = encoding::LE::readFloat(in);
        attributes.push_back(types::entity::Attribute(id, min, max, current, current, {}));
    }

    metadata = serializer::CommonTypes::getEntityMetadata(in);
    syncedProperties = types::entity::PropertySyncData::read(in);

    const auto linkCount = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < linkCount; ++i) {
        links.push_back(serializer::CommonTypes::getEntityLink(in));
    }

}

void AddActorPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    serializer::CommonTypes::putString(out, type);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putVector3Nullable(out, motion);
    encoding::LE::writeFloat(out, pitch);
    encoding::LE::writeFloat(out, yaw);
    encoding::LE::writeFloat(out, headYaw);
    encoding::LE::writeFloat(out, bodyYaw);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(attributes.size()));
    for (const auto &attribute : attributes) {
        serializer::CommonTypes::putString(out, attribute.getId());
        encoding::LE::writeFloat(out, attribute.getMin());
        encoding::LE::writeFloat(out, attribute.getCurrent());
        encoding::LE::writeFloat(out, attribute.getMax());
    }

    serializer::CommonTypes::putEntityMetadata(out, metadata);
    syncedProperties.write(out);

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(links.size()));
    for (const auto &link : links) {
        serializer::CommonTypes::putEntityLink(out, link);
    }

}

bool AddActorPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddActor(*this);
}

}  // namespace bedrock_protocol
