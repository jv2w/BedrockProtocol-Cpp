/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayerBlockActionWithBlockInfo.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"

namespace bedrock_protocol::types {

/**
 * This is used for PlayerAuthInput packet when the flags include PERFORM_BLOCK_ACTIONS
 */
class PlayerBlockActionWithBlockInfo final : public PlayerBlockAction {
public:
    /**
     * @throws std::invalid_argument if the action type is not valid for this action
     */
    PlayerBlockActionWithBlockInfo(std::int32_t actionType, const BlockPosition &blockPosition, std::int32_t face);

    [[nodiscard]] std::int32_t getActionType() const override { return actionType; }

    [[nodiscard]] const BlockPosition &getBlockPosition() const { return blockPosition; }

    [[nodiscard]] std::int32_t getFace() const { return face; }

    static PlayerBlockActionWithBlockInfo read(encoding::ByteBufferReader &in, std::int32_t actionType);

    void write(encoding::ByteBufferWriter &out) const override;

    static bool isValidActionType(std::int32_t actionType);

    [[nodiscard]] std::unique_ptr<PlayerBlockAction> clone() const override
    {
        return std::make_unique<PlayerBlockActionWithBlockInfo>(*this);
    }

private:
    std::int32_t actionType;
    BlockPosition blockPosition;
    std::int32_t face;
};

}  // namespace bedrock_protocol::types
