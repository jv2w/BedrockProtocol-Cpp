/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/CraftingDataPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/recipe/MaterialReducerRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/MaterialReducerRecipeOutput.h"
#include "bedrock_protocol/protocol/types/recipe/MultiRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/PotionContainerChangeRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/PotionTypeRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/ShapedRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/ShapelessRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/SmithingTransformRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/SmithingTrimRecipe.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CraftingDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CRAFTING_DATA_PACKET;

    /**
     * The single tagged recipe list was replaced by ten separately-typed vectors in 1.26.40; there is no
     * per-entry recipe type tag any more.
     * @see gophertunnel minecraft/protocol/packet/crafting_data.go:38-51
     */
    /** @var ShapedRecipe[] */
    std::vector<types::recipe::ShapedRecipe> shapedRecipes;
    /** @var ShapelessRecipe[] */
    std::vector<types::recipe::ShapelessRecipe> shapelessRecipes;
    /** @var MultiRecipe[] */
    std::vector<types::recipe::MultiRecipe> multiRecipes;
    /** @var ShapelessRecipe[] - a shulker box recipe is a shapeless recipe (recipe.go:118-122) */
    std::vector<types::recipe::ShapelessRecipe> shulkerBoxRecipes;
    /** @var ShapelessRecipe[] */
    std::vector<types::recipe::ShapelessRecipe> shapelessChemistryRecipes;
    /** @var ShapedRecipe[] */
    std::vector<types::recipe::ShapedRecipe> shapedChemistryRecipes;
    /** @var SmithingTransformRecipe[] */
    std::vector<types::recipe::SmithingTransformRecipe> smithingTransformRecipes;
    /** @var SmithingTrimRecipe[] */
    std::vector<types::recipe::SmithingTrimRecipe> smithingTrimRecipes;
    /** @var PotionTypeRecipe[] */
    std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes;
    /** @var PotionContainerChangeRecipe[] */
    std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes;
    /** @var MaterialReducerRecipe[] */
    std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes;
    bool cleanRecipes = false;

    /**
     * @generate-create-func
     */
    static CraftingDataPacket create(std::vector<types::recipe::ShapedRecipe> shapedRecipes, std::vector<types::recipe::ShapelessRecipe> shapelessRecipes, std::vector<types::recipe::MultiRecipe> multiRecipes, std::vector<types::recipe::ShapelessRecipe> shulkerBoxRecipes, std::vector<types::recipe::ShapelessRecipe> shapelessChemistryRecipes, std::vector<types::recipe::ShapedRecipe> shapedChemistryRecipes, std::vector<types::recipe::SmithingTransformRecipe> smithingTransformRecipes, std::vector<types::recipe::SmithingTrimRecipe> smithingTrimRecipes, std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes, std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes, std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes, bool cleanRecipes);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CraftingDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
