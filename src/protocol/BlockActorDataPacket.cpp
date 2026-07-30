/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/BlockActorDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BlockActorDataPacket.h"

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

BlockActorDataPacket BlockActorDataPacket::create(types::BlockPosition blockPosition, types::CacheableNbt<nbt::tag::CompoundTag> nbt)
{
    BlockActorDataPacket result;
    result.blockPosition = std::move(blockPosition);
    result.nbt = std::move(nbt);
    return result;
}

void BlockActorDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    blockPosition = serializer::CommonTypes::getBlockPosition(in);
    nbt = types::CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in));

}

void BlockActorDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    serializer::CommonTypes::putBlockPosition(out, blockPosition);
    out.writeByteArray(nbt.getEncodedNbt());

}

bool BlockActorDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBlockActorData(*this);
}

}  // namespace bedrock_protocol
