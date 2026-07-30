/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/NetworkSettingsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/NetworkSettingsPacket.h"

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

NetworkSettingsPacket NetworkSettingsPacket::create(std::uint16_t compressionThreshold, std::uint16_t compressionAlgorithm, bool enableClientThrottling, std::uint8_t clientThrottleThreshold, float clientThrottleScalar)
{
    NetworkSettingsPacket result;
    result.compressionThreshold = compressionThreshold;
    result.compressionAlgorithm = compressionAlgorithm;
    result.enableClientThrottling = enableClientThrottling;
    result.clientThrottleThreshold = clientThrottleThreshold;
    result.clientThrottleScalar = clientThrottleScalar;
    return result;
}

void NetworkSettingsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    compressionThreshold = encoding::LE::readUnsignedShort(in);
    compressionAlgorithm = encoding::LE::readUnsignedShort(in);
    enableClientThrottling = serializer::CommonTypes::getBool(in);
    clientThrottleThreshold = encoding::Byte::readUnsigned(in);
    clientThrottleScalar = encoding::LE::readFloat(in);

}

void NetworkSettingsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedShort(out, compressionThreshold);
    encoding::LE::writeUnsignedShort(out, compressionAlgorithm);
    serializer::CommonTypes::putBool(out, enableClientThrottling);
    encoding::Byte::writeUnsigned(out, clientThrottleThreshold);
    encoding::LE::writeFloat(out, clientThrottleScalar);

}

bool NetworkSettingsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleNetworkSettings(*this);
}

}  // namespace bedrock_protocol
