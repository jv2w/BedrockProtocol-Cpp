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

#include "bedrock_protocol/protocol/CraftingDataPacket.h"

#include <vector>
#include <utility>

#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

namespace {

/** Reads a varuint32-counted vector of recipes, each of which decodes itself. */
template <typename Recipe>
std::vector<Recipe> decodeRecipeList(encoding::ByteBufferReader &in)
{
    std::vector<Recipe> recipes;
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        recipes.push_back(Recipe::decode(in));
    }
    return recipes;
}

template <typename Recipe>
void encodeRecipeList(encoding::ByteBufferWriter &out, const std::vector<Recipe> &recipes)
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(recipes.size()));
    for (const auto &recipe : recipes) {
        recipe.encode(out);
    }
}

}  // namespace

CraftingDataPacket CraftingDataPacket::create(std::vector<types::recipe::ShapedRecipe> shapedRecipes, std::vector<types::recipe::ShapelessRecipe> shapelessRecipes, std::vector<types::recipe::MultiRecipe> multiRecipes, std::vector<types::recipe::ShapelessRecipe> shulkerBoxRecipes, std::vector<types::recipe::ShapelessRecipe> shapelessChemistryRecipes, std::vector<types::recipe::ShapedRecipe> shapedChemistryRecipes, std::vector<types::recipe::SmithingTransformRecipe> smithingTransformRecipes, std::vector<types::recipe::SmithingTrimRecipe> smithingTrimRecipes, std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes, std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes, std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes, bool cleanRecipes)
{
    CraftingDataPacket result;
    result.shapedRecipes = std::move(shapedRecipes);
    result.shapelessRecipes = std::move(shapelessRecipes);
    result.multiRecipes = std::move(multiRecipes);
    result.shulkerBoxRecipes = std::move(shulkerBoxRecipes);
    result.shapelessChemistryRecipes = std::move(shapelessChemistryRecipes);
    result.shapedChemistryRecipes = std::move(shapedChemistryRecipes);
    result.smithingTransformRecipes = std::move(smithingTransformRecipes);
    result.smithingTrimRecipes = std::move(smithingTrimRecipes);
    result.potionTypeRecipes = std::move(potionTypeRecipes);
    result.potionContainerRecipes = std::move(potionContainerRecipes);
    result.materialReducerRecipes = std::move(materialReducerRecipes);
    result.cleanRecipes = cleanRecipes;
    return result;
}

void CraftingDataPacket::decodePayload(encoding::ByteBufferReader &in)
{
    //gophertunnel minecraft/protocol/packet/crafting_data.go:38-51 - ten separately-typed recipe vectors in
    //this exact order, then the material reducers and the clear flag.
    shapedRecipes = decodeRecipeList<types::recipe::ShapedRecipe>(in);
    shapelessRecipes = decodeRecipeList<types::recipe::ShapelessRecipe>(in);
    multiRecipes = decodeRecipeList<types::recipe::MultiRecipe>(in);
    shulkerBoxRecipes = decodeRecipeList<types::recipe::ShapelessRecipe>(in);
    shapelessChemistryRecipes = decodeRecipeList<types::recipe::ShapelessRecipe>(in);
    shapedChemistryRecipes = decodeRecipeList<types::recipe::ShapedRecipe>(in);
    smithingTransformRecipes = decodeRecipeList<types::recipe::SmithingTransformRecipe>(in);
    smithingTrimRecipes = decodeRecipeList<types::recipe::SmithingTrimRecipe>(in);
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
    encodeRecipeList(out, shapedRecipes);
    encodeRecipeList(out, shapelessRecipes);
    encodeRecipeList(out, multiRecipes);
    encodeRecipeList(out, shulkerBoxRecipes);
    encodeRecipeList(out, shapelessChemistryRecipes);
    encodeRecipeList(out, shapedChemistryRecipes);
    encodeRecipeList(out, smithingTransformRecipes);
    encodeRecipeList(out, smithingTrimRecipes);
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
