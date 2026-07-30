/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/SubChunkPacketEntryWithCache.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/SubChunkPacketEntryCommon.h"

namespace bedrock_protocol::types {

class SubChunkPacketEntryWithCache final {
public:
    SubChunkPacketEntryWithCache(SubChunkPacketEntryCommon base, std::uint64_t usedBlobHash)
        : base(std::move(base)), usedBlobHash(usedBlobHash)
    {
    }

    [[nodiscard]] const SubChunkPacketEntryCommon &getBase() const { return base; }

    [[nodiscard]] std::uint64_t getUsedBlobHash() const { return usedBlobHash; }

    static SubChunkPacketEntryWithCache read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    SubChunkPacketEntryCommon base;
    std::uint64_t usedBlobHash;
};

}  // namespace bedrock_protocol::types
