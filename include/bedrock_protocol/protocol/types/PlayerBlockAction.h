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
#include <memory>

#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

/**
 * This is used for PlayerAuthInput packet when the flags include PERFORM_BLOCK_ACTIONS
 */
class PlayerBlockAction {
public:
    virtual ~PlayerBlockAction() = default;

    [[nodiscard]] virtual std::int32_t getActionType() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<PlayerBlockAction> clone() const = 0;

protected:
    PlayerBlockAction() = default;
    PlayerBlockAction(const PlayerBlockAction &) = default;
    PlayerBlockAction &operator=(const PlayerBlockAction &) = default;
};

}  // namespace bedrock_protocol::types
