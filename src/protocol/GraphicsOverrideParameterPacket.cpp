/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/GraphicsOverrideParameterPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/GraphicsOverrideParameterPacket.h"

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

GraphicsOverrideParameterPacket GraphicsOverrideParameterPacket::create(std::vector<types::ParameterKeyframeValue> values, std::optional<float> unknownFloat, std::optional<math::Vector3> unknownVector3, std::string biomeIdentifier, std::optional<std::string> playerIdentifier, types::GraphicsOverrideParameterType parameterType, bool reset)
{
    GraphicsOverrideParameterPacket result;
    result.values = std::move(values);
    result.unknownFloat = std::move(unknownFloat);
    result.unknownVector3 = std::move(unknownVector3);
    result.biomeIdentifier = std::move(biomeIdentifier);
    result.playerIdentifier = std::move(playerIdentifier);
    result.parameterType = parameterType;
    result.reset = reset;
    return result;
}

void GraphicsOverrideParameterPacket::decodePayload(encoding::ByteBufferReader &in)
{
    values.clear();  // PHP appends to $this->values with []=; the property starts empty on every decoded instance, so the C++ vector must be reset or a reused packet accumulates stale entries
    const auto count = encoding::VarInt::readUnsignedInt(in);
    for (std::uint32_t i = 0; i < count; ++i) {
        values.push_back(types::ParameterKeyframeValue::read(in));
    }
    unknownFloat = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readFloat(reader); });
    unknownVector3 = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getVector3(reader); });
    biomeIdentifier = serializer::CommonTypes::getString(in);
    playerIdentifier = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });
    parameterType = types::GraphicsOverrideParameterTypeFromPacket(encoding::Byte::readUnsigned(in));
    reset = serializer::CommonTypes::getBool(in);

}

void GraphicsOverrideParameterPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(values.size()));
    for (const auto &value : values) {
        value.write(out);
    }
    serializer::CommonTypes::writeOptional(out, unknownFloat, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeFloat(writer, value); });
    serializer::CommonTypes::writeOptional(out, unknownVector3, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putVector3(writer, value); });
    serializer::CommonTypes::putString(out, biomeIdentifier);
    serializer::CommonTypes::writeOptional(out, playerIdentifier, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putString(writer, value); });
    encoding::Byte::writeUnsigned(out, static_cast<std::uint8_t>(parameterType));
    serializer::CommonTypes::putBool(out, reset);

}

bool GraphicsOverrideParameterPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleGraphicsOverrideParameter(*this);
}

}  // namespace bedrock_protocol
