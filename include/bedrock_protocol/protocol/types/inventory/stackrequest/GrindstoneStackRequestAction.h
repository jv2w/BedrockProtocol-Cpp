/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/GrindstoneStackRequestAction.php
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
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Repair and/or remove enchantments from an item in a grindstone.
 */
class GrindstoneStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_GRINDSTONE;

    GrindstoneStackRequestAction(std::uint32_t recipeId, std::int32_t repairCost, std::uint8_t repetitions) :
        recipeId(recipeId), repairCost(repairCost), repetitions(repetitions) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint32_t getRecipeId() const { return recipeId; }

    /** WARNING: This may be negative */
    [[nodiscard]] std::int32_t getRepairCost() const { return repairCost; }

    [[nodiscard]] std::uint8_t getRepetitions() const { return repetitions; }

    static GrindstoneStackRequestAction read(encoding::ByteBufferReader &in) {
        //gophertunnel minecraft/protocol/item_stack.go:566-570 - a fixed int32 recipe ID, then the repetition
        //count, then the cost. The recipe ID is no longer a varuint32 and the last two fields are the other
        //way round from the previous version.
        const auto recipeId = static_cast<std::uint32_t>(encoding::LE::readSignedInt(in));
        const auto repetitions = encoding::Byte::readUnsigned(in);
        const auto repairCost = encoding::VarInt::readSignedInt(in); //WHY!!!!

        return GrindstoneStackRequestAction(recipeId, repairCost, repetitions);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::LE::writeSignedInt(out, static_cast<std::int32_t>(recipeId));
        encoding::Byte::writeUnsigned(out, repetitions);
        encoding::VarInt::writeSignedInt(out, repairCost);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<GrindstoneStackRequestAction>(*this);
    }

private:
    std::uint32_t recipeId;
    std::int32_t repairCost;
    std::uint8_t repetitions;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
