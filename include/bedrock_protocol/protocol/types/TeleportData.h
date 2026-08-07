/*
 * This file is part of BedrockProtocol-Cpp.
 * Not present in the PHP original; models gophertunnel's protocol.TeleportData
 * (minecraft/protocol/player.go:179-190).
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

namespace bedrock_protocol::types {

/**
 * The additional information accompanying a MovePlayer teleport.
 *
 * It is an optional of its own as of 1.26.40: its presence bool is always on the wire, independent
 * of the movement mode (packet/move_player.go:68).
 */
class TeleportData final {
public:
    TeleportData() = default;

    TeleportData(std::int32_t teleportCause, std::int32_t teleportItem)
        : teleportCause(teleportCause), teleportItem(teleportItem)
    {
    }

    [[nodiscard]] std::int32_t getTeleportCause() const { return teleportCause; }

    /** The entity type that caused the teleportation, for example an ender pearl. */
    [[nodiscard]] std::int32_t getTeleportItem() const { return teleportItem; }

    /**
     * @throws encoding::DataDecodeException
     */
    static TeleportData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t teleportCause = 0;
    std::int32_t teleportItem = 0;
};

}  // namespace bedrock_protocol::types
