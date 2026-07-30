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

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::create(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, types::OverrideUpdateType updateType, std::optional<std::int32_t> intOverrideValue, std::optional<float> floatOverrideValue)
{
    PlayerUpdateEntityOverridesPacket result;
    result.actorRuntimeId = actorRuntimeId;
    result.propertyIndex = propertyIndex;
    result.updateType = std::move(updateType);
    result.intOverrideValue = std::move(intOverrideValue);
    result.floatOverrideValue = std::move(floatOverrideValue);
    return result;
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createIntOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, std::int32_t value)
{
    return create(actorRuntimeId, propertyIndex, types::OverrideUpdateType::SET_INT_OVERRIDE, value, std::nullopt);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createFloatOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex, float value)
{
    return create(actorRuntimeId, propertyIndex, types::OverrideUpdateType::SET_FLOAT_OVERRIDE, std::nullopt, value);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createClearOverrides(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex)
{
    return create(actorRuntimeId, propertyIndex, types::OverrideUpdateType::CLEAR_OVERRIDES, std::nullopt, std::nullopt);
}

PlayerUpdateEntityOverridesPacket PlayerUpdateEntityOverridesPacket::createRemoveOverride(std::uint64_t actorRuntimeId, std::uint32_t propertyIndex)
{
    return create(actorRuntimeId, propertyIndex, types::OverrideUpdateType::REMOVE_OVERRIDE, std::nullopt, std::nullopt);
}

void PlayerUpdateEntityOverridesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    actorRuntimeId = serializer::CommonTypes::getActorRuntimeId(in);
    propertyIndex = encoding::VarInt::readUnsignedInt(in);
    updateType = types::OverrideUpdateTypeFromPacket(encoding::Byte::readUnsigned(in));
    if (updateType == types::OverrideUpdateType::SET_INT_OVERRIDE) {
        intOverrideValue = encoding::LE::readSignedInt(in);
    }
    else if (updateType == types::OverrideUpdateType::SET_FLOAT_OVERRIDE) {
        floatOverrideValue = encoding::LE::readFloat(in);
    }

}

void PlayerUpdateEntityOverridesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putActorRuntimeId(out, actorRuntimeId);
    encoding::VarInt::writeUnsignedInt(out, propertyIndex);
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(updateType));
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
