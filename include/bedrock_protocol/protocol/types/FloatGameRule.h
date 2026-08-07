/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/FloatGameRule.php
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
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/types/GameRule.h"
#include "bedrock_protocol/protocol/types/GameRuleType.h"

namespace bedrock_protocol::types {

class FloatGameRule final : public GameRule {
public:
    static constexpr std::int32_t ID = GameRuleType::FLOAT;

    FloatGameRule(float value, bool isPlayerModifiable) : GameRule(isPlayerModifiable), value(value) {}

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    void encode(encoding::ByteBufferWriter &out) const override
    {
        encoding::LE::writeFloat(out, value);
    }

    /** @throws DataDecodeException */
    static FloatGameRule decode(encoding::ByteBufferReader &in, bool isPlayerModifiable)
    {
        return FloatGameRule(encoding::LE::readFloat(in), isPlayerModifiable);
    }

    [[nodiscard]] std::unique_ptr<GameRule> clone() const override
    {
        return std::make_unique<FloatGameRule>(*this);
    }

private:
    float value;
};

}  // namespace bedrock_protocol::types
