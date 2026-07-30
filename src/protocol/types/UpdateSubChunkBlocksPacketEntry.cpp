/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/UpdateSubChunkBlocksPacketEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/UpdateSubChunkBlocksPacketEntry.h"

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/UpdateBlockPacket.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::VarInt;
using serializer::CommonTypes;

UpdateSubChunkBlocksPacketEntry UpdateSubChunkBlocksPacketEntry::simple(BlockPosition blockPosition,
                                                                       std::uint32_t blockRuntimeId)
{
    return UpdateSubChunkBlocksPacketEntry(blockPosition, blockRuntimeId,
                                           static_cast<std::uint32_t>(UpdateBlockPacket::FLAG_NETWORK), 0, 0);
}

UpdateSubChunkBlocksPacketEntry UpdateSubChunkBlocksPacketEntry::read(encoding::ByteBufferReader &in)
{
    const auto blockPosition = CommonTypes::getBlockPosition(in);
    const auto blockRuntimeId = VarInt::readUnsignedInt(in);
    const auto updateFlags = VarInt::readUnsignedInt(in);
    const auto syncedUpdateActorUniqueId = VarInt::readUnsignedLong(in); //this can't use the standard method because it's unsigned as opposed to the usual signed... !!!!!!
    const auto syncedUpdateType = VarInt::readUnsignedInt(in); //this isn't even consistent with UpdateBlockSyncedPacket?!

    return UpdateSubChunkBlocksPacketEntry(blockPosition, blockRuntimeId, updateFlags, syncedUpdateActorUniqueId,
                                           syncedUpdateType);
}

void UpdateSubChunkBlocksPacketEntry::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putBlockPosition(out, blockPosition);
    VarInt::writeUnsignedInt(out, blockRuntimeId);
    VarInt::writeUnsignedInt(out, flags);
    VarInt::writeUnsignedLong(out, syncedUpdateActorUniqueId);
    VarInt::writeUnsignedInt(out, syncedUpdateType);
}

}  // namespace bedrock_protocol::types
