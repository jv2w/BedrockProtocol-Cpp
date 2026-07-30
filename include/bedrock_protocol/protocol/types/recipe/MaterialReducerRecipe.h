/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/MaterialReducerRecipe.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/types/recipe/MaterialReducerRecipeOutput.h"

namespace bedrock_protocol::types::recipe {

class MaterialReducerRecipe final {
public:
    MaterialReducerRecipe(std::int32_t inputItemId, std::int32_t inputItemMeta,
                          std::vector<MaterialReducerRecipeOutput> outputs)
        : inputItemId(inputItemId), inputItemMeta(inputItemMeta), outputs(std::move(outputs))
    {
    }

    [[nodiscard]] std::int32_t getInputItemId() const { return inputItemId; }

    [[nodiscard]] std::int32_t getInputItemMeta() const { return inputItemMeta; }

    [[nodiscard]] const std::vector<MaterialReducerRecipeOutput> &getOutputs() const { return outputs; }

private:
    std::int32_t inputItemId;
    std::int32_t inputItemMeta;
    std::vector<MaterialReducerRecipeOutput> outputs;
};

}  // namespace bedrock_protocol::types::recipe
