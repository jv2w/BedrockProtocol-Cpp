/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PlayerBlockActionStopBreak.php
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

#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/PlayerAction.h"
#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"

namespace bedrock_protocol::types {

class PlayerBlockActionStopBreak final : public PlayerBlockAction {
public:
    [[nodiscard]] std::int32_t getActionType() const override
    {
        return PlayerAction::STOP_BREAK;
    }

    void write(encoding::ByteBufferWriter &) const override
    {
        //NOOP
    }

    [[nodiscard]] std::unique_ptr<PlayerBlockAction> clone() const override
    {
        return std::make_unique<PlayerBlockActionStopBreak>(*this);
    }
};

}  // namespace bedrock_protocol::types
