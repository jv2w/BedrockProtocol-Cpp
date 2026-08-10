/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerLocationPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerLocationPacket.h"

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

PlayerLocationPacket PlayerLocationPacket::create(types::PlayerLocationType type, std::int64_t actorUniqueId, std::optional<math::Vector3> position)
{
    PlayerLocationPacket result;
    result.type = type;
    result.actorUniqueId = actorUniqueId;
    result.position = std::move(position);
    return result;
}

PlayerLocationPacket PlayerLocationPacket::createCoordinates(std::int64_t actorUniqueId, math::Vector3 position)
{
    return create(types::PlayerLocationType::PLAYER_LOCATION_COORDINATES, actorUniqueId, position);
}

PlayerLocationPacket PlayerLocationPacket::createHide(std::int64_t actorUniqueId)
{
    return create(types::PlayerLocationType::PLAYER_LOCATION_HIDE, actorUniqueId, std::nullopt);
}

void PlayerLocationPacket::decodePayload(encoding::ByteBufferReader &in)
{
    // player_location.go:33-35 - the unique ID comes first, then the type as a varuint32, then the
    // same type again as a varint32. The repeat carries no information, so it is read and discarded
    // rather than checked, which keeps a client that disagrees with itself readable.
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    type = types::PlayerLocationTypeFromPacket(static_cast<std::int32_t>(encoding::VarInt::readUnsignedInt(in)));
    encoding::VarInt::readSignedInt(in);

    if (type == types::PlayerLocationType::PLAYER_LOCATION_COORDINATES) {
        position = serializer::CommonTypes::getVector3(in);
    }

}

void PlayerLocationPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(type));
    //The second varint is the type again, not a reserved zero: gophertunnel v1.58.0
    //minecraft/protocol/packet/player_location.go:34-35 writes pk.Type through both, and Endstone
    //r26_u4 gives each case of the switch its own "Packet Type" field constrained to that case.
    encoding::VarInt::writeSignedInt(out, static_cast<std::int32_t>(type));

    if (type == types::PlayerLocationType::PLAYER_LOCATION_COORDINATES) {
        if (!position.has_value()) { // this should never be the case
            throw std::logic_error("PlayerLocationPacket with type PLAYER_LOCATION_COORDINATES require a position to be provided");
        }
        serializer::CommonTypes::putVector3(out, position.value());
    }

}

bool PlayerLocationPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerLocation(*this);
}

}  // namespace bedrock_protocol
