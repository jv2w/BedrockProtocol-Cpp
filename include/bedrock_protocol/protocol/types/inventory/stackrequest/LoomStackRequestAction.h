/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/LoomStackRequestAction.php
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
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Apply a pattern to a banner using a loom.
 */
class LoomStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_LOOM;

    explicit LoomStackRequestAction(std::string patternId, std::uint8_t repetitions = 1) :
        patternId(std::move(patternId)), repetitions(repetitions) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getPatternId() const { return patternId; }

    [[nodiscard]] std::uint8_t getRepetitions() const { return repetitions; }

    static LoomStackRequestAction read(encoding::ByteBufferReader &in) {
        auto patternId = std::string(serializer::CommonTypes::getString(in));
        const auto repetitions = encoding::Byte::readUnsigned(in);
        return LoomStackRequestAction(std::move(patternId), repetitions);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        serializer::CommonTypes::putString(out, patternId);
        encoding::Byte::writeUnsigned(out, repetitions);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<LoomStackRequestAction>(*this);
    }

private:
    std::string patternId;
    std::uint8_t repetitions;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
