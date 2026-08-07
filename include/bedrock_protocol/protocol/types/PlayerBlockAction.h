/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerBlockAction.php
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

/**
 * This is used for the PlayerAuthInput packet's block action list.
 *
 * There is one element shape and no per-action branch: every action carries a block position and a
 * face (minecraft/protocol/player.go:158-163).
 */
class PlayerBlockAction final {
public:
    PlayerBlockAction() = default;

    PlayerBlockAction(std::int32_t actionType, const BlockPosition &blockPosition, std::int32_t face)
        : actionType(actionType), blockPosition(blockPosition), face(face)
    {
    }

    [[nodiscard]] std::int32_t getActionType() const { return actionType; }

    [[nodiscard]] const BlockPosition &getBlockPosition() const { return blockPosition; }

    [[nodiscard]] std::int32_t getFace() const { return face; }

    /**
     * @throws encoding::DataDecodeException
     */
    static PlayerBlockAction read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t actionType = 0;
    BlockPosition blockPosition;
    std::int32_t face = 0;
};

}  // namespace bedrock_protocol::types
