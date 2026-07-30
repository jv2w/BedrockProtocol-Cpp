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

#include "bedrock_protocol/protocol/CraftingDataPacket.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

CraftingDataPacket CraftingDataPacket::create(std::vector<std::unique_ptr<types::recipe::RecipeWithTypeId>> recipesWithTypeIds, std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes, std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes, std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes, bool cleanRecipes)
{
    CraftingDataPacket result;
    result.recipesWithTypeIds = std::move(recipesWithTypeIds);
    result.potionTypeRecipes = std::move(potionTypeRecipes);
    result.potionContainerRecipes = std::move(potionContainerRecipes);
    result.materialReducerRecipes = std::move(materialReducerRecipes);
    result.cleanRecipes = cleanRecipes;
    return result;
}

void CraftingDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    const auto recipeCount = encoding::VarInt::readUnsignedInt(in);
    std::string previousType = "none";
    for (std::uint32_t i = 0; i < recipeCount; ++i) {
        const auto recipeType = encoding::VarInt::readSignedInt(in);

        switch (recipeType) {
        case ENTRY_SHAPELESS:
        case ENTRY_USER_DATA_SHAPELESS:
        case ENTRY_SHAPELESS_CHEMISTRY:
            recipesWithTypeIds.push_back(
                std::make_unique<types::recipe::ShapelessRecipe>(types::recipe::ShapelessRecipe::decode(recipeType, in)));
            break;
        case ENTRY_SHAPED:
        case ENTRY_SHAPED_CHEMISTRY:
            recipesWithTypeIds.push_back(
                std::make_unique<types::recipe::ShapedRecipe>(types::recipe::ShapedRecipe::decode(recipeType, in)));
            break;
        case ENTRY_MULTI:
            recipesWithTypeIds.push_back(
                std::make_unique<types::recipe::MultiRecipe>(types::recipe::MultiRecipe::decode(recipeType, in)));
            break;
        case ENTRY_SMITHING_TRANSFORM:
            recipesWithTypeIds.push_back(std::make_unique<types::recipe::SmithingTransformRecipe>(
                types::recipe::SmithingTransformRecipe::decode(recipeType, in)));
            break;
        case ENTRY_SMITHING_TRIM:
            recipesWithTypeIds.push_back(std::make_unique<types::recipe::SmithingTrimRecipe>(
                types::recipe::SmithingTrimRecipe::decode(recipeType, in)));
            break;
        default:
            throw PacketDecodeException("Unhandled recipe type " + std::to_string(recipeType) + " (previous was " +
                                        previousType + ")");
        }
        previousType = std::to_string(recipeType);
    }
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto inputId = encoding::VarInt::readSignedInt(in);
        const auto inputMeta = encoding::VarInt::readSignedInt(in);
        const auto ingredientId = encoding::VarInt::readSignedInt(in);
        const auto ingredientMeta = encoding::VarInt::readSignedInt(in);
        const auto outputId = encoding::VarInt::readSignedInt(in);
        const auto outputMeta = encoding::VarInt::readSignedInt(in);
        potionTypeRecipes.push_back(types::recipe::PotionTypeRecipe(inputId, inputMeta, ingredientId, ingredientMeta, outputId, outputMeta));
    }
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto input = encoding::VarInt::readSignedInt(in);
        const auto ingredient = encoding::VarInt::readSignedInt(in);
        const auto output = encoding::VarInt::readSignedInt(in);
        potionContainerRecipes.push_back(types::recipe::PotionContainerChangeRecipe(input, ingredient, output));
    }
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        const auto inputIdAndData = encoding::VarInt::readSignedInt(in);
        const auto inputId = inputIdAndData >> 16;
        const auto inputMeta = inputIdAndData & 0x7fff;
        std::vector<types::recipe::MaterialReducerRecipeOutput> outputs;
        for (std::uint32_t j = 0, outputCount = encoding::VarInt::readUnsignedInt(in); j < outputCount; ++j) {
            const auto outputItemId = encoding::VarInt::readSignedInt(in);
            const auto outputItemCount = encoding::VarInt::readSignedInt(in);
            outputs.push_back(types::recipe::MaterialReducerRecipeOutput(outputItemId, outputItemCount));
        }
        materialReducerRecipes.push_back(types::recipe::MaterialReducerRecipe(inputId, inputMeta, std::move(outputs)));
    }
    cleanRecipes = serializer::CommonTypes::getBool(in);

}

void CraftingDataPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(recipesWithTypeIds.size()));
    for (const auto &d : recipesWithTypeIds) {
        encoding::VarInt::writeSignedInt(out, d->getTypeId());
        d->encode(out);
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(potionTypeRecipes.size()));
    for (const auto &recipe : potionTypeRecipes) {
        encoding::VarInt::writeSignedInt(out, recipe.getInputItemId());
        encoding::VarInt::writeSignedInt(out, recipe.getInputItemMeta());
        encoding::VarInt::writeSignedInt(out, recipe.getIngredientItemId());
        encoding::VarInt::writeSignedInt(out, recipe.getIngredientItemMeta());
        encoding::VarInt::writeSignedInt(out, recipe.getOutputItemId());
        encoding::VarInt::writeSignedInt(out, recipe.getOutputItemMeta());
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(potionContainerRecipes.size()));
    for (const auto &recipe : potionContainerRecipes) {
        encoding::VarInt::writeSignedInt(out, recipe.getInputItemId());
        encoding::VarInt::writeSignedInt(out, recipe.getIngredientItemId());
        encoding::VarInt::writeSignedInt(out, recipe.getOutputItemId());
    }
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(materialReducerRecipes.size()));
    for (const auto &recipe : materialReducerRecipes) {
        //PHP evaluates ($id << 16) in 64-bit and never overflows; shifting a signed 32-bit value by 16 here is
        //undefined behaviour once the id reaches 0x8000, so do the shift in unsigned and convert back.
        encoding::VarInt::writeSignedInt(
            out, static_cast<std::int32_t>((static_cast<std::uint32_t>(recipe.getInputItemId()) << 16) |
                                           static_cast<std::uint32_t>(recipe.getInputItemMeta())));
        encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(recipe.getOutputs().size()));
        for (const auto &output : recipe.getOutputs()) {
            encoding::VarInt::writeSignedInt(out, output.getItemId());
            encoding::VarInt::writeSignedInt(out, output.getCount());
        }
    }
    serializer::CommonTypes::putBool(out, cleanRecipes);

}

bool CraftingDataPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCraftingData(*this);
}

}  // namespace bedrock_protocol
