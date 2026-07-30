/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CraftRecipeStackRequestAction.php
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

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Tells that the current transaction crafted the specified recipe.
 */
class CraftRecipeStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_RECIPE;

    CraftRecipeStackRequestAction(std::uint32_t recipeId, std::uint8_t repetitions) :
        recipeId(recipeId), repetitions(repetitions) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint32_t getRecipeId() const { return recipeId; }

    [[nodiscard]] std::uint8_t getRepetitions() const { return repetitions; }

    static CraftRecipeStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto recipeId = serializer::CommonTypes::readRecipeNetId(in);
        const auto repetitions = encoding::Byte::readUnsigned(in);
        return CraftRecipeStackRequestAction(recipeId, repetitions);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        serializer::CommonTypes::writeRecipeNetId(out, recipeId);
        encoding::Byte::writeUnsigned(out, repetitions);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<CraftRecipeStackRequestAction>(*this);
    }

private:
    std::uint32_t recipeId;
    std::uint8_t repetitions;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
