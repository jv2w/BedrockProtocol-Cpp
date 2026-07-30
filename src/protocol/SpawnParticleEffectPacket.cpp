/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/SpawnParticleEffectPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/SpawnParticleEffectPacket.h"

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

SpawnParticleEffectPacket SpawnParticleEffectPacket::create(std::uint8_t dimensionId, std::int64_t actorUniqueId, math::Vector3 position, std::string particleName, std::optional<std::string> molangVariablesJson)
{
    SpawnParticleEffectPacket result;
    result.dimensionId = dimensionId;
    result.actorUniqueId = actorUniqueId;
    result.position = std::move(position);
    result.particleName = std::move(particleName);
    result.molangVariablesJson = std::move(molangVariablesJson);
    return result;
}

void SpawnParticleEffectPacket::decodePayload(encoding::ByteBufferReader &in)
{
    dimensionId = encoding::Byte::readUnsigned(in);
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    position = serializer::CommonTypes::getVector3(in);
    particleName = serializer::CommonTypes::getString(in);
    molangVariablesJson = serializer::CommonTypes::getBool(in) ? std::optional<std::string>(serializer::CommonTypes::getString(in)) : std::nullopt;

}

void SpawnParticleEffectPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, dimensionId);
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putString(out, particleName);
    serializer::CommonTypes::putBool(out, molangVariablesJson.has_value());
    if (molangVariablesJson.has_value()) {
        serializer::CommonTypes::putString(out, *molangVariablesJson);
    }

}

bool SpawnParticleEffectPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleSpawnParticleEffect(*this);
}

}  // namespace bedrock_protocol
