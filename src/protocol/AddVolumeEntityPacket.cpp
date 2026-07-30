/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AddVolumeEntityPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AddVolumeEntityPacket.h"

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

AddVolumeEntityPacket AddVolumeEntityPacket::create(std::uint32_t entityNetId, types::CacheableNbt<nbt::tag::CompoundTag> data, std::string jsonIdentifier, std::string instanceName, types::BlockPosition minBound, types::BlockPosition maxBound, std::int32_t dimension, std::string engineVersion)
{
    AddVolumeEntityPacket result;
    result.entityNetId = entityNetId;
    result.data = std::move(data);
    result.jsonIdentifier = std::move(jsonIdentifier);
    result.instanceName = std::move(instanceName);
    result.minBound = std::move(minBound);
    result.maxBound = std::move(maxBound);
    result.dimension = dimension;
    result.engineVersion = std::move(engineVersion);
    return result;
}

void AddVolumeEntityPacket::decodePayload(encoding::ByteBufferReader &in)
{
    entityNetId = encoding::VarInt::readUnsignedInt(in);
    data = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));
    jsonIdentifier = serializer::CommonTypes::getString(in);
    instanceName = serializer::CommonTypes::getString(in);
    minBound = serializer::CommonTypes::getBlockPosition(in);
    maxBound = serializer::CommonTypes::getBlockPosition(in);
    dimension = encoding::VarInt::readSignedInt(in);
    engineVersion = serializer::CommonTypes::getString(in);

}

void AddVolumeEntityPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, entityNetId);
    out.writeByteArray(data.getEncodedNbt());
    serializer::CommonTypes::putString(out, jsonIdentifier);
    serializer::CommonTypes::putString(out, instanceName);
    serializer::CommonTypes::putBlockPosition(out, minBound);
    serializer::CommonTypes::putBlockPosition(out, maxBound);
    encoding::VarInt::writeSignedInt(out, dimension);
    serializer::CommonTypes::putString(out, engineVersion);

}

bool AddVolumeEntityPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAddVolumeEntity(*this);
}

}  // namespace bedrock_protocol
