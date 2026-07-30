/*
 * This file is part of BedrockProtocol for Endstone.
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
#include <memory>
#include <optional>
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
#include "bedrock_protocol/protocol/types/recipe/RecipeWithTypeId.h"
#include "bedrock_protocol/protocol/types/recipe/ShapedRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/ShapelessRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/SmithingTransformRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/SmithingTrimRecipe.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

class CraftingDataPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::CRAFTING_DATA_PACKET;

    static constexpr std::int32_t ENTRY_SHAPELESS = 0;
    static constexpr std::int32_t ENTRY_SHAPED = 1;
    static constexpr std::int32_t ENTRY_MULTI = 4;
    static constexpr std::int32_t ENTRY_USER_DATA_SHAPELESS = 5;
    static constexpr std::int32_t ENTRY_SHAPELESS_CHEMISTRY = 6;
    static constexpr std::int32_t ENTRY_SHAPED_CHEMISTRY = 7;
    static constexpr std::int32_t ENTRY_SMITHING_TRANSFORM = 8;
    static constexpr std::int32_t ENTRY_SMITHING_TRIM = 9;

    /** @var RecipeWithTypeId[] */
    std::vector<std::unique_ptr<types::recipe::RecipeWithTypeId>> recipesWithTypeIds;
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
    static CraftingDataPacket create(std::vector<std::unique_ptr<types::recipe::RecipeWithTypeId>> recipesWithTypeIds, std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes, std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes, std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes, bool cleanRecipes);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "CraftingDataPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
