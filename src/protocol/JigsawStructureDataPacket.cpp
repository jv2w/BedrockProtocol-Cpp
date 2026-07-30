/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/JigsawStructureDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/JigsawStructureDataPacket.h"

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

JigsawStructureDataPacket JigsawStructureDataPacket::create(types::CacheableNbt<nbt::tag::CompoundTag> nbt)
{
    JigsawStructureDataPacket result;
    result.nbt = std::move(nbt);
    return result;
}

void JigsawStructureDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    nbt = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void JigsawStructureDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    out.writeByteArray(nbt.getEncodedNbt());

}

bool JigsawStructureDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleJigsawStructureData(*this);
}

}  // namespace bedrock_protocol
