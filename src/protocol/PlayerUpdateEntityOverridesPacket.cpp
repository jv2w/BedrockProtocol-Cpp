/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/PlayerUpdateEntityOverridesPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/PlayerUpdateEntityOverridesPacket.h"

#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {

/**
 * gophertunnel v1.58.0 minecraft/protocol/packet/player_update_entity_overrides.go:41-47 writes this
 * name after the numeric type, indexed by the type itself. Endstone r26_u4 shows the same string as
 * the first field of every case of the update switch.
 */
constexpr std::string_view UPDATE_TYPE_NAMES[] = {"clearoverrides", "removeoverride", "setintoverride",
                                                  "setfloatoverride"};

}  // namespace

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::create(std::int64_t actorUniqueId, std::uint32_t propertyIndex, types::OverrideUpdateType updateType, std::optional<std::int32_t> intOverrideValue, std::optional<float> floatOverrideValue)
{
    PlayerUpdateEntityOverridesPacket result;
    result.actorUniqueId = actorUniqueId;
    result.propertyIndex = propertyIndex;
    result.updateType = std::move(updateType);
    result.intOverrideValue = std::move(intOverrideValue);
    result.floatOverrideValue = std::move(floatOverrideValue);
    return result;
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createIntOverride(std::int64_t actorUniqueId, std::uint32_t propertyIndex, std::int32_t value)
{
    return create(actorUniqueId, propertyIndex, types::OverrideUpdateType::SET_INT_OVERRIDE, value, std::nullopt);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createFloatOverride(std::int64_t actorUniqueId, std::uint32_t propertyIndex, float value)
{
    return create(actorUniqueId, propertyIndex, types::OverrideUpdateType::SET_FLOAT_OVERRIDE, std::nullopt, value);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createClearOverrides(std::int64_t actorUniqueId, std::uint32_t propertyIndex)
{
    return create(actorUniqueId, propertyIndex, types::OverrideUpdateType::CLEAR_OVERRIDES, std::nullopt, std::nullopt);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createRemoveOverride(std::int64_t actorUniqueId, std::uint32_t propertyIndex)
{
    return create(actorUniqueId, propertyIndex, types::OverrideUpdateType::REMOVE_OVERRIDE, std::nullopt, std::nullopt);
}

void PlayerUpdateEntityOverridesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    // player_update_entity_overrides.go:38-47 - the varuint32 type is followed by its name as a
    // string, not by the type repeated as a byte. Reading a byte there left the rest of the name on
    // the stream and desynchronised every field after it.
    actorUniqueId = serializer::CommonTypes::getActorUniqueId(in);
    propertyIndex = encoding::VarInt::readUnsignedInt(in);
    const auto rawUpdateType = encoding::VarInt::readUnsignedInt(in);
    if (rawUpdateType >= std::size(UPDATE_TYPE_NAMES)) {
        throw PacketDecodeException("Unknown entity override type " + std::to_string(rawUpdateType));
    }
    // The name is redundant with the numeric type, so it is read and discarded.
    serializer::CommonTypes::getString(in);
    updateType = types::OverrideUpdateTypeFromPacket(static_cast<std::int32_t>(rawUpdateType));
    if (updateType == types::OverrideUpdateType::SET_INT_OVERRIDE) {
        intOverrideValue = encoding::LE::readSignedInt(in);
    }
    else if (updateType == types::OverrideUpdateType::SET_FLOAT_OVERRIDE) {
        floatOverrideValue = encoding::LE::readFloat(in);
    }

}

void PlayerUpdateEntityOverridesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorUniqueId(out, actorUniqueId);
    encoding::VarInt::writeUnsignedInt(out, propertyIndex);
    const auto rawUpdateType = static_cast<std::uint32_t>(updateType);
    if (rawUpdateType >= std::size(UPDATE_TYPE_NAMES)) {
        throw std::invalid_argument("Unknown entity override type " + std::to_string(rawUpdateType));
    }
    encoding::VarInt::writeUnsignedInt(out, rawUpdateType);
    serializer::CommonTypes::putString(out, std::string(UPDATE_TYPE_NAMES[rawUpdateType]));
    if (updateType == types::OverrideUpdateType::SET_INT_OVERRIDE) {
        if (!intOverrideValue.has_value()) { // this should never be the case
            throw std::logic_error("PlayerUpdateEntityOverridesPacket with type SET_INT_OVERRIDE requires intOverrideValue to be provided");
        }
        encoding::LE::writeSignedInt(out, *intOverrideValue);
    }
    else if (updateType == types::OverrideUpdateType::SET_FLOAT_OVERRIDE) {
        if (!floatOverrideValue.has_value()) { // this should never be the case
            throw std::logic_error("PlayerUpdateEntityOverridesPacket with type SET_FLOAT_OVERRIDE requires floatOverrideValue to be provided");
        }
        encoding::LE::writeFloat(out, *floatOverrideValue);
    }

}

bool PlayerUpdateEntityOverridesPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handlePlayerUpdateEntityOverrides(*this);
}

}  // namespace bedrock_protocol
