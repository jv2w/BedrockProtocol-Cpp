/*
 * This file is part of BedrockProtocol-Cpp.
 * Not present in the PHP original: added for Minecraft 1.26.40 (protocol 2168).
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

/**
 * A game rule that carries no value at all.
 *
 * New in 1.26.40: gophertunnel v1.58.0 minecraft/protocol/reader.go:272-273 reads type 0 as a nil value,
 * and writer.go:208-210 writes the type with no payload following it.
 */
class NullGameRule final : public GameRule {
public:
    static constexpr std::int32_t ID = GameRuleType::NULL_;

    explicit NullGameRule(bool isPlayerModifiable) : GameRule(isPlayerModifiable) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    void encode(encoding::ByteBufferWriter &out) const override
    {
        // The type ID is the whole encoding; there is no payload.
        (void)out;
    }

    static NullGameRule decode(encoding::ByteBufferReader &in, bool isPlayerModifiable)
    {
        (void)in;
        return NullGameRule(isPlayerModifiable);
    }

    [[nodiscard]] std::unique_ptr<GameRule> clone() const override
    {
        return std::make_unique<NullGameRule>(*this);
    }
};

}  // namespace bedrock_protocol::types
