/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/DeprecatedCraftingResultsStackRequestAction.php
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
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Not clear what this is needed for, but it is very clearly marked as deprecated, so hopefully it'll go away before I
 * have to write a proper description for it.
 */
class DeprecatedCraftingResultsStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::CRAFTING_RESULTS_DEPRECATED_ASK_TY_LAING;

    DeprecatedCraftingResultsStackRequestAction(std::vector<inventory::ItemStack> results, std::uint8_t iterations) :
        results(std::move(results)), iterations(iterations) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::vector<inventory::ItemStack> &getResults() const { return results; }

    [[nodiscard]] std::uint8_t getIterations() const { return iterations; }

    static DeprecatedCraftingResultsStackRequestAction read(encoding::ByteBufferReader &in) {
        std::vector<inventory::ItemStack> results;
        for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
            results.push_back(serializer::CommonTypes::getItemStackWithoutStackId(in));
        }
        const auto iterations = encoding::Byte::readUnsigned(in);
        return DeprecatedCraftingResultsStackRequestAction(std::move(results), iterations);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(results.size()));
        for (const auto &result : results) {
            serializer::CommonTypes::putItemStackWithoutStackId(out, result);
        }
        encoding::Byte::writeUnsigned(out, iterations);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<DeprecatedCraftingResultsStackRequestAction>(*this);
    }

private:
    std::vector<inventory::ItemStack> results;
    std::uint8_t iterations;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
