/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/CraftRecipeOptionalStackRequestAction.php
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
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Renames an item in an anvil, or map on a cartography table.
 */
class CraftRecipeOptionalStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_RECIPE_OPTIONAL;

    //TODO: promote this when we can rename parameters (BC break)
    CraftRecipeOptionalStackRequestAction(std::uint32_t type, std::int32_t filterStringIndex) {
        recipeId = type;
        this->filterStringIndex = filterStringIndex;
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint32_t getRecipeId() const { return recipeId; }

    [[nodiscard]] std::int32_t getFilterStringIndex() const { return filterStringIndex; }

    static CraftRecipeOptionalStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto recipeId = serializer::CommonTypes::readRecipeNetId(in);
        const auto filterStringIndex = encoding::LE::readSignedInt(in);
        return CraftRecipeOptionalStackRequestAction(recipeId, filterStringIndex);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        serializer::CommonTypes::writeRecipeNetId(out, recipeId);
        encoding::LE::writeSignedInt(out, filterStringIndex);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<CraftRecipeOptionalStackRequestAction>(*this);
    }

private:
    std::uint32_t recipeId;
    std::int32_t filterStringIndex;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
