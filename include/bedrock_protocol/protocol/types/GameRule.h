/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/GameRule.php
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

class GameRule {
public:
    explicit GameRule(bool isPlayerModifiable) : isPlayerModifiable_(isPlayerModifiable) {}

    virtual ~GameRule() = default;

    [[nodiscard]] bool isPlayerModifiable() const { return isPlayerModifiable_; }

    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;

    virtual void encode(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for polymorphic members. */
    [[nodiscard]] virtual std::unique_ptr<GameRule> clone() const = 0;

protected:
    GameRule(const GameRule &) = default;
    GameRule &operator=(const GameRule &) = default;

private:
    /** Named with a trailing underscore only because isPlayerModifiable() is the getter's name. */
    bool isPlayerModifiable_;
};

}  // namespace bedrock_protocol::types
