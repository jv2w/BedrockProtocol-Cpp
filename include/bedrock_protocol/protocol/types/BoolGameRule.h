/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/BoolGameRule.php
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
#include "bedrock_protocol/protocol/types/GameRule.h"
#include "bedrock_protocol/protocol/types/GameRuleType.h"

namespace bedrock_protocol::types {

class BoolGameRule final : public GameRule {
public:
    static constexpr std::int32_t ID = GameRuleType::BOOL;

    BoolGameRule(bool value, bool isPlayerModifiable) : GameRule(isPlayerModifiable), value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] bool getValue() const { return value; }

    void encode(encoding::ByteBufferWriter &out, bool isStartGame) const override;

    static BoolGameRule decode(encoding::ByteBufferReader &in, bool isPlayerModifiable);

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] std::unique_ptr<GameRule> clone() const override { return std::make_unique<BoolGameRule>(*this); }

private:
    bool value;
};

}  // namespace bedrock_protocol::types
