/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ChangeDimensionPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ChangeDimensionPacket.h"

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

ChangeDimensionPacket ChangeDimensionPacket::create(std::int32_t dimension, math::Vector3 position, bool respawn, std::optional<std::uint32_t> loadingScreenId)
{
    ChangeDimensionPacket result;
    result.dimension = dimension;
    result.position = std::move(position);
    result.respawn = respawn;
    result.loadingScreenId = std::move(loadingScreenId);
    return result;
}

void ChangeDimensionPacket::decodePayload(encoding::ByteBufferReader &in)
{
    dimension = encoding::VarInt::readSignedInt(in);
    position = serializer::CommonTypes::getVector3(in);
    respawn = serializer::CommonTypes::getBool(in);
    loadingScreenId = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::LE::readUnsignedInt(reader); });

}

void ChangeDimensionPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, dimension);
    serializer::CommonTypes::putVector3(out, position);
    serializer::CommonTypes::putBool(out, respawn);
    serializer::CommonTypes::writeOptional(out, loadingScreenId, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::LE::writeUnsignedInt(writer, value); });

}

bool ChangeDimensionPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleChangeDimension(*this);
}

}  // namespace bedrock_protocol
