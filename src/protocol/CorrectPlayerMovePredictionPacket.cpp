/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CorrectPlayerMovePredictionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CorrectPlayerMovePredictionPacket.h"

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

CorrectPlayerMovePredictionPacket CorrectPlayerMovePredictionPacket::create(math::Vector3 position, math::Vector3 delta, bool onGround, std::uint64_t tick, std::uint8_t predictionType, math::Vector2 vehicleRotation, std::optional<float> vehicleAngularVelocity)
{
    CorrectPlayerMovePredictionPacket result;
    result.position = std::move(position);
    result.delta = std::move(delta);
    result.onGround = onGround;
    result.tick = tick;
    result.predictionType = predictionType;
    result.vehicleRotation = std::move(vehicleRotation);
    result.vehicleAngularVelocity = std::move(vehicleAngularVelocity);
    return result;
}

void CorrectPlayerMovePredictionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    predictionType = encoding::Byte::readUnsigned(in);
    position = serializer::CommonTypes::getVector3(in);
    delta = serializer::CommonTypes::getVector3(in);
    // PHP evaluates `new Vector2(LE::readFloat($in), LE::readFloat($in))` left to right, but C++ leaves
    // the evaluation order of constructor arguments unspecified - MSVC reads them right to left, which
    // silently swapped X and Y. The two reads have to be sequenced explicitly.
    const auto vehicleRotationX = encoding::LE::readFloat(in);
    const auto vehicleRotationY = encoding::LE::readFloat(in);
    vehicleRotation = math::Vector2(vehicleRotationX, vehicleRotationY);
    vehicleAngularVelocity = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readFloat(reader); });
    onGround = serializer::CommonTypes::getBool(in);
    tick = encoding::VarInt::readUnsignedLong(in);

}

void CorrectPlayerMovePredictionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::Byte::writeUnsigned(out, predictionType);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putVector3(out, delta);
    encoding::LE::writeFloat(out, vehicleRotation.getX());
    encoding::LE::writeFloat(out, vehicleRotation.getY());
    serializer::CommonTypes::writeOptional(out, vehicleAngularVelocity, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeFloat(writer, value); });
    serializer::CommonTypes::putBool(out, onGround);
    encoding::VarInt::writeUnsignedLong(out, tick);

}

bool CorrectPlayerMovePredictionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCorrectPlayerMovePrediction(*this);
}

}  // namespace bedrock_protocol
