/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientMovementPredictionSyncPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientMovementPredictionSyncPacket.h"

#include <stdexcept>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/BitSet.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

ClientMovementPredictionSyncPacket ClientMovementPredictionSyncPacket::internalCreate(serializer::BitSet flags, float scale, float width, float height, float movementSpeed, float underwaterMovementSpeed, float lavaMovementSpeed, float jumpStrength, float health, float hunger, float frictionModifier, float bounciness, float airDragModifier, std::int64_t actorUniqueId, bool actorFlyingState)
{
    ClientMovementPredictionSyncPacket result;
    result.flags = std::move(flags);
    result.scale = scale;
    result.width = width;
    result.height = height;
    result.movementSpeed = movementSpeed;
    result.underwaterMovementSpeed = underwaterMovementSpeed;
    result.lavaMovementSpeed = lavaMovementSpeed;
    result.jumpStrength = jumpStrength;
    result.health = health;
    result.hunger = hunger;
    result.frictionModifier = frictionModifier;
    result.bounciness = bounciness;
    result.airDragModifier = airDragModifier;
    result.actorUniqueId = actorUniqueId;
    result.actorFlyingState = actorFlyingState;
    return result;
}

ClientMovementPredictionSyncPacket ClientMovementPredictionSyncPacket::create(serializer::BitSet flags, float scale, float width, float height, float movementSpeed, float underwaterMovementSpeed, float lavaMovementSpeed, float jumpStrength, float health, float hunger, float frictionModifier, float bounciness, float airDragModifier, std::int64_t actorUniqueId, bool actorFlyingState)
{
    if (flags.getLength() != FLAG_LENGTH) {
        throw std::invalid_argument("Input flags must be " + std::to_string(FLAG_LENGTH) + " bits long");
    }

    return internalCreate(std::move(flags), scale, width, height, movementSpeed, underwaterMovementSpeed, lavaMovementSpeed, jumpStrength, health, hunger, frictionModifier, bounciness, airDragModifier, actorUniqueId, actorFlyingState);
}

void ClientMovementPredictionSyncPacket::decodePayload(encoding::ByteBufferReader &in)
{
    flags = serializer::BitSet::read(in, FLAG_LENGTH);
    scale = encoding::LE::readFloat(in);
    width = encoding::LE::readFloat(in);
    height = encoding::LE::readFloat(in);
    movementSpeed = encoding::LE::readFloat(in);
    underwaterMovementSpeed = encoding::LE::readFloat(in);
    lavaMovementSpeed = encoding::LE::readFloat(in);
    jumpStrength = encoding::LE::readFloat(in);
    health = encoding::LE::readFloat(in);
    hunger = encoding::LE::readFloat(in);
    frictionModifier = encoding::LE::readFloat(in);
    bounciness = encoding::LE::readFloat(in);
    airDragModifier = encoding::LE::readFloat(in);
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    actorFlyingState = serializer::CommonTypes::getBool(in);

}

void ClientMovementPredictionSyncPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    flags.write(out);
    encoding::LE::writeFloat(out, scale);
    encoding::LE::writeFloat(out, width);
    encoding::LE::writeFloat(out, height);
    encoding::LE::writeFloat(out, movementSpeed);
    encoding::LE::writeFloat(out, underwaterMovementSpeed);
    encoding::LE::writeFloat(out, lavaMovementSpeed);
    encoding::LE::writeFloat(out, jumpStrength);
    encoding::LE::writeFloat(out, health);
    encoding::LE::writeFloat(out, hunger);
    encoding::LE::writeFloat(out, frictionModifier);
    encoding::LE::writeFloat(out, bounciness);
    encoding::LE::writeFloat(out, airDragModifier);
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    serializer::CommonTypes::putBool(out, actorFlyingState);

}

bool ClientMovementPredictionSyncPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientMovementPredictionSync(*this);
}

}  // namespace bedrock_protocol
