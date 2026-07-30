/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/UpdateSubChunkBlocksPacketEntry.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"

namespace bedrock_protocol::types {

class UpdateSubChunkBlocksPacketEntry final {
public:
    UpdateSubChunkBlocksPacketEntry(BlockPosition blockPosition, std::uint32_t blockRuntimeId, std::uint32_t flags,
                                    std::uint64_t syncedUpdateActorUniqueId, std::uint32_t syncedUpdateType)
        : blockPosition(blockPosition), blockRuntimeId(blockRuntimeId), flags(flags),
          syncedUpdateActorUniqueId(syncedUpdateActorUniqueId), syncedUpdateType(syncedUpdateType)
    {
    }

    static UpdateSubChunkBlocksPacketEntry simple(BlockPosition blockPosition, std::uint32_t blockRuntimeId);

    [[nodiscard]] const BlockPosition &getBlockPosition() const { return blockPosition; }

    [[nodiscard]] std::uint32_t getBlockRuntimeId() const { return blockRuntimeId; }

    [[nodiscard]] std::uint32_t getFlags() const { return flags; }

    [[nodiscard]] std::uint64_t getSyncedUpdateActorUniqueId() const { return syncedUpdateActorUniqueId; }

    [[nodiscard]] std::uint32_t getSyncedUpdateType() const { return syncedUpdateType; }

    static UpdateSubChunkBlocksPacketEntry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    BlockPosition blockPosition;
    std::uint32_t blockRuntimeId;
    std::uint32_t flags;
    //These two fields are useless 99.9% of the time; they are here to allow this packet to provide UpdateBlockSyncedPacket functionality.
    std::uint64_t syncedUpdateActorUniqueId;
    std::uint32_t syncedUpdateType;
};

}  // namespace bedrock_protocol::types
