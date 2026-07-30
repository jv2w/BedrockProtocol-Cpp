/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/RequestAbilityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/RequestAbilityPacket.h"

#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

RequestAbilityPacket RequestAbilityPacket::create(std::int32_t abilityId, std::variant<bool, float> abilityValue)
{
    RequestAbilityPacket result;
    result.abilityId = abilityId;
    result.abilityValue = std::move(abilityValue);
    return result;
}

void RequestAbilityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    abilityId = encoding::VarInt::readSignedInt(in);

    const auto valueType = encoding::Byte::readUnsigned(in);

    //what is the point of having a type ID if you just write all the types anyway ??? mojang ...
    //only one of these values is ever used; the other(s) are discarded
    const auto boolValue = serializer::CommonTypes::getBool(in);
    const auto floatValue = encoding::LE::readFloat(in);

    switch (valueType) {
    case VALUE_TYPE_BOOL:
        abilityValue = boolValue;
        break;
    case VALUE_TYPE_FLOAT:
        abilityValue = floatValue;
        break;
    default:
        throw PacketDecodeException("Unknown ability value type " + std::to_string(valueType));
    }

}

void RequestAbilityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, abilityId);

    std::uint8_t valueType;
    bool boolValue;
    float floatValue;
    if (std::holds_alternative<bool>(abilityValue)) {
        valueType = VALUE_TYPE_BOOL;
        boolValue = std::get<bool>(abilityValue);
        floatValue = 0.0f;
    }
    else if (std::holds_alternative<float>(abilityValue)) {
        valueType = VALUE_TYPE_FLOAT;
        boolValue = false;
        floatValue = std::get<float>(abilityValue);
    }
    else {
        throw std::logic_error("Unreachable");
    }
    encoding::Byte::writeUnsigned(out, valueType);
    serializer::CommonTypes::putBool(out, boolValue);
    encoding::LE::writeFloat(out, floatValue);

}

bool RequestAbilityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleRequestAbility(*this);
}

}  // namespace bedrock_protocol
