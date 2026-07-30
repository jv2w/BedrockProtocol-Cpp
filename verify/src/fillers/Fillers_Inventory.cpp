/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: inventory, container, item and recipe packets.
 *
 * The two conventions from Fillers_Core.cpp hold here as well: one create() call with every argument
 * spelled out, and every value drawn into a named local first. See that file's header for why.
 *
 * This batch carries most of the protocol's polymorphism - TransactionData, ItemStackRequestAction and
 * RecipeWithTypeId are all abstract bases whose concrete type is chosen by a tag on the wire - so the
 * collections below deliberately mix concrete types where the decoder dispatches per element.
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/CompletedUsingItemPacket.h"
#include "bedrock_protocol/protocol/ContainerClosePacket.h"
#include "bedrock_protocol/protocol/ContainerOpenPacket.h"
#include "bedrock_protocol/protocol/ContainerSetDataPacket.h"
#include "bedrock_protocol/protocol/CraftingDataPacket.h"
#include "bedrock_protocol/protocol/CreativeContentPacket.h"
#include "bedrock_protocol/protocol/InventoryContentPacket.h"
#include "bedrock_protocol/protocol/InventorySlotPacket.h"
#include "bedrock_protocol/protocol/InventoryTransactionPacket.h"
#include "bedrock_protocol/protocol/ItemRegistryPacket.h"
#include "bedrock_protocol/protocol/ItemStackRequestPacket.h"
#include "bedrock_protocol/protocol/ItemStackResponsePacket.h"
#include "bedrock_protocol/protocol/PlayerArmorDamagePacket.h"
#include "bedrock_protocol/protocol/PlayerEnchantOptionsPacket.h"
#include "bedrock_protocol/protocol/PlayerHotbarPacket.h"
#include "bedrock_protocol/protocol/PlayerToggleCrafterSlotRequestPacket.h"
#include "bedrock_protocol/protocol/SetPlayerInventoryOptionsPacket.h"
#include "bedrock_protocol/protocol/UnlockedRecipesPacket.h"
#include "bedrock_protocol/protocol/UpdateEquipPacket.h"
#include "bedrock_protocol/protocol/UpdateTradePacket.h"
#include "bedrock_protocol/protocol/types/recipe/ShapelessRecipe.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

namespace {

types::recipe::RecipeIngredient makeRecipeIngredient(ValueWell &w)
{
    return {makeItemDescriptor(w), w.i32()};
}

/**
 * A ShapelessRecipe, which makeRecipe (a MultiRecipe) deliberately is not.
 *
 * CraftingDataPacket writes a per-entry type tag and dispatches on it, so a homogeneous recipe list
 * would leave every branch but ENTRY_MULTI untested - and MultiRecipe is by far the smallest of them.
 * The unlockingIngredients optional is engaged because RecipeUnlockingRequirement inverts the flag it
 * writes, so the disengaged form is the one that emits nothing.
 */
std::unique_ptr<types::recipe::RecipeWithTypeId> makeShapelessRecipe(ValueWell &w)
{
    // ENTRY_SHAPELESS is the tag CraftingDataPacket maps back to ShapelessRecipe.
    const auto typeId = ValueWell::pin(CraftingDataPacket::ENTRY_SHAPELESS);
    auto recipeId = w.str("shapelessRecipeId");
    std::vector<types::recipe::RecipeIngredient> inputs;
    inputs.push_back(makeRecipeIngredient(w));
    inputs.push_back(makeRecipeIngredient(w));
    std::vector<types::inventory::ItemStack> outputs = {makeItemStack(w), makeItemStack(w)};
    const auto uuid = w.uuid();
    auto blockName = w.str("shapelessBlockName");
    const auto priority = w.i32();
    std::vector<types::recipe::RecipeIngredient> unlockingIngredients;
    unlockingIngredients.push_back(makeRecipeIngredient(w));
    unlockingIngredients.push_back(makeRecipeIngredient(w));
    types::recipe::RecipeUnlockingRequirement unlockingRequirement{std::move(unlockingIngredients)};
    const auto recipeNetId = w.u32();

    return std::make_unique<types::recipe::ShapelessRecipe>(typeId, std::move(recipeId), std::move(inputs),
                                                            std::move(outputs), uuid, std::move(blockName), priority,
                                                            std::move(unlockingRequirement), recipeNetId);
}

types::EnchantOption makeEnchantOption(ValueWell &w)
{
    const auto cost = w.u8();
    const auto slotFlags = w.u32();
    std::vector<types::Enchant> equipActivated = {{w.u32(), w.u8()}, {w.u32(), w.u8()}};
    std::vector<types::Enchant> heldActivated = {{w.u32(), w.u8()}, {w.u32(), w.u8()}};
    std::vector<types::Enchant> selfActivated = {{w.u32(), w.u8()}, {w.u32(), w.u8()}};
    auto name = w.str("enchantOptionName");
    const auto optionId = w.u32();

    return {cost,           slotFlags, std::move(equipActivated), std::move(heldActivated),
            std::move(selfActivated), std::move(name), optionId};
}

}  // namespace

BP_FILLER(UpdateTradePacket, 10)
{
    auto &w = ctx.well;
    const auto windowId = w.u8();
    const auto windowType = w.u8();
    const auto windowSlotCount = w.i32();
    const auto tradeTier = w.i32();
    const auto traderActorUniqueId = w.i64();
    const auto playerActorUniqueId = w.i64();
    auto displayName = w.str("displayName");
    const auto isV2Trading = w.flag();
    const auto isEconomyTrading = w.flag();
    auto offers = makeCacheableNbt(w);

    return std::make_unique<UpdateTradePacket>(
        UpdateTradePacket::create(windowId, windowType, windowSlotCount, tradeTier, traderActorUniqueId,
                                  playerActorUniqueId, std::move(displayName), isV2Trading, isEconomyTrading,
                                  std::move(offers)));
}

BP_FILLER(InventorySlotPacket, 5)
{
    auto &w = ctx.well;
    const auto windowId = w.u32();
    const auto inventorySlot = w.u32();
    auto containerName = w.some(makeFullContainerName(w));
    auto storage = w.some(makeItemStackWrapper(w));
    auto item = makeItemStackWrapper(w);

    return std::make_unique<InventorySlotPacket>(InventorySlotPacket::create(
        windowId, inventorySlot, std::move(containerName), std::move(storage), std::move(item)));
}

BP_FILLER(CraftingDataPacket, 5)
{
    auto &w = ctx.well;
    std::vector<std::unique_ptr<types::recipe::RecipeWithTypeId>> recipesWithTypeIds;
    recipesWithTypeIds.push_back(makeRecipe(w));
    recipesWithTypeIds.push_back(makeShapelessRecipe(w));
    recipesWithTypeIds.push_back(makeRecipe(w));

    std::vector<types::recipe::PotionTypeRecipe> potionTypeRecipes = {
        {w.i32(), w.i32(), w.i32(), w.i32(), w.i32(), w.i32()},
        {w.i32(), w.i32(), w.i32(), w.i32(), w.i32(), w.i32()}};
    std::vector<types::recipe::PotionContainerChangeRecipe> potionContainerRecipes = {{w.i32(), w.i32(), w.i32()},
                                                                                      {w.i32(), w.i32(), w.i32()}};

    // The material reducer packs id and meta into one signed varint as (id << 16) | meta, and the
    // decoder masks the meta with 0x7fff. Anything wider than a byte on either side would either
    // overflow the shift or lose the top meta bit, so the round trip would fail for a reason that has
    // nothing to do with the packet.
    const auto reducerInputIdA = static_cast<std::int32_t>(w.u8());
    const auto reducerInputMetaA = static_cast<std::int32_t>(w.u8());
    const auto reducerInputIdB = static_cast<std::int32_t>(w.u8());
    const auto reducerInputMetaB = static_cast<std::int32_t>(w.u8());
    std::vector<types::recipe::MaterialReducerRecipeOutput> reducerOutputsA = {{w.i32(), w.i32()}, {w.i32(), w.i32()}};
    std::vector<types::recipe::MaterialReducerRecipeOutput> reducerOutputsB = {{w.i32(), w.i32()}, {w.i32(), w.i32()}};
    std::vector<types::recipe::MaterialReducerRecipe> materialReducerRecipes = {
        {reducerInputIdA, reducerInputMetaA, std::move(reducerOutputsA)},
        {reducerInputIdB, reducerInputMetaB, std::move(reducerOutputsB)}};

    const auto cleanRecipes = w.flag();

    return std::make_unique<CraftingDataPacket>(CraftingDataPacket::create(
        std::move(recipesWithTypeIds), std::move(potionTypeRecipes), std::move(potionContainerRecipes),
        std::move(materialReducerRecipes), cleanRecipes));
}

BP_FILLER(UpdateEquipPacket, 5)
{
    auto &w = ctx.well;
    const auto windowId = w.u8();
    const auto windowType = w.u8();
    const auto windowSlotCount = w.i32();
    const auto actorUniqueId = w.i64();
    auto nbt = makeCacheableNbt(w);

    return std::make_unique<UpdateEquipPacket>(
        UpdateEquipPacket::create(windowId, windowType, windowSlotCount, actorUniqueId, std::move(nbt)));
}

BP_FILLER(SetPlayerInventoryOptionsPacket, 5)
{
    auto &w = ctx.well;
    // All five fields are validated enums: the decoder throws on any value outside the enumeration,
    // so each has to be pinned to a legal member rather than drawn. They are pinned to four DIFFERENT
    // members so that a pair of fields read in the wrong order still changes the bytes.
    const auto leftTab = ValueWell::pin(types::inventory::InventoryLeftTab::SURVIVAL);
    const auto rightTab = ValueWell::pin(types::inventory::InventoryRightTab::ARMOR);
    const auto filtering = w.flag();
    const auto inventoryLayout = ValueWell::pin(types::inventory::InventoryLayout::RECIPE_BOOK_ONLY);
    const auto craftingLayout = ValueWell::pin(types::inventory::InventoryLayout::INVENTORY_ONLY);

    return std::make_unique<SetPlayerInventoryOptionsPacket>(SetPlayerInventoryOptionsPacket::create(
        leftTab, rightTab, filtering, inventoryLayout, craftingLayout));
}

BP_FILLER(InventoryContentPacket, 4)
{
    auto &w = ctx.well;
    const auto windowId = w.u32();
    std::vector<types::inventory::ItemStackWrapper> items = {makeItemStackWrapper(w), makeItemStackWrapper(w),
                                                             makeItemStackWrapper(w)};
    // Both optionals are dereferenced unconditionally by encodePayload - there is no writeOptional
    // here - so a disengaged value would abort the encode rather than select a shorter branch.
    auto containerName = w.some(makeFullContainerName(w));
    auto storage = w.some(makeItemStackWrapper(w));

    return std::make_unique<InventoryContentPacket>(
        InventoryContentPacket::create(windowId, std::move(items), std::move(containerName), std::move(storage)));
}

BP_FILLER(ContainerOpenPacket, 4)
{
    auto &w = ctx.well;
    const auto windowId = w.u8();
    const auto windowType = w.u8();
    const auto blockPosition = makeBlockPosition(w);
    const auto actorUniqueId = w.i64();

    return std::make_unique<ContainerOpenPacket>(
        ContainerOpenPacket::create(windowId, windowType, blockPosition, actorUniqueId));
}

BP_FILLER(InventoryTransactionPacket, 3)
{
    auto &w = ctx.well;
    const auto requestId = w.i32();
    const auto changedContainerIdA = w.u8();
    const std::vector<std::uint8_t> changedIndexesA = {w.u8(), w.u8()};
    const auto changedContainerIdB = w.u8();
    const std::vector<std::uint8_t> changedIndexesB = {w.u8(), w.u8(), w.u8()};
    std::vector<types::inventory::InventoryTransactionChangedSlotsHack> changedSlots = {
        {changedContainerIdA, changedIndexesA}, {changedContainerIdB, changedIndexesB}};
    auto requestChangedSlots = w.some(std::move(changedSlots));
    auto trData = makeTransactionData(w);

    return std::make_unique<InventoryTransactionPacket>(
        InventoryTransactionPacket::create(requestId, std::move(requestChangedSlots), std::move(trData)));
}

BP_FILLER(ContainerClosePacket, 3)
{
    auto &w = ctx.well;
    const auto windowId = w.u8();
    const auto windowType = w.u8();
    const auto server = w.flag();

    return std::make_unique<ContainerClosePacket>(ContainerClosePacket::create(windowId, windowType, server));
}

BP_FILLER(ContainerSetDataPacket, 3)
{
    auto &w = ctx.well;
    const auto windowId = w.u8();
    const auto property = w.i32();
    const auto value = w.i32();

    return std::make_unique<ContainerSetDataPacket>(ContainerSetDataPacket::create(windowId, property, value));
}

BP_FILLER(PlayerHotbarPacket, 3)
{
    auto &w = ctx.well;
    const auto selectedHotbarSlot = w.u32();
    const auto windowId = w.u8();
    const auto selectHotbarSlot = w.flag();

    return std::make_unique<PlayerHotbarPacket>(
        PlayerHotbarPacket::create(selectedHotbarSlot, windowId, selectHotbarSlot));
}

BP_FILLER(PlayerToggleCrafterSlotRequestPacket, 3)
{
    auto &w = ctx.well;
    const auto position = makeBlockPosition(w);
    const auto slot = w.u8();
    const auto disabled = w.flag();

    return std::make_unique<PlayerToggleCrafterSlotRequestPacket>(
        PlayerToggleCrafterSlotRequestPacket::create(position, slot, disabled));
}

BP_FILLER(CreativeContentPacket, 2)
{
    auto &w = ctx.well;
    const auto categoryIdA = w.i32();
    auto categoryNameA = w.str("groupName0");
    auto iconA = makeItemStack(w);
    const auto categoryIdB = w.i32();
    auto categoryNameB = w.str("groupName1");
    auto iconB = makeItemStack(w);
    std::vector<types::inventory::CreativeGroupEntry> groups = {
        {categoryIdA, std::move(categoryNameA), std::move(iconA)},
        {categoryIdB, std::move(categoryNameB), std::move(iconB)}};

    const auto entryIdA = w.u32();
    auto itemA = makeItemStack(w);
    const auto groupIdA = w.u32();
    const auto entryIdB = w.u32();
    auto itemB = makeItemStack(w);
    const auto groupIdB = w.u32();
    std::vector<types::inventory::CreativeItemEntry> items = {{entryIdA, std::move(itemA), groupIdA},
                                                              {entryIdB, std::move(itemB), groupIdB}};

    return std::make_unique<CreativeContentPacket>(CreativeContentPacket::create(std::move(groups), std::move(items)));
}

BP_FILLER(UnlockedRecipesPacket, 2)
{
    auto &w = ctx.well;
    const auto type = w.u32();
    std::vector<std::string> recipes = {w.str("recipe0"), w.str("recipe1"), w.str("recipe2")};

    return std::make_unique<UnlockedRecipesPacket>(UnlockedRecipesPacket::create(type, std::move(recipes)));
}

BP_FILLER(CompletedUsingItemPacket, 2)
{
    auto &w = ctx.well;
    const auto itemId = w.i16();
    const auto action = w.i32();

    return std::make_unique<CompletedUsingItemPacket>(CompletedUsingItemPacket::create(itemId, action));
}

BP_FILLER(ItemStackRequestPacket, 1)
{
    auto &w = ctx.well;
    // push_back rather than a braced list: ItemStackRequest owns unique_ptr actions and is move-only,
    // and an initializer_list copies its elements.
    std::vector<types::inventory::stackrequest::ItemStackRequest> requests;
    requests.push_back(makeItemStackRequest(w));
    requests.push_back(makeItemStackRequest(w));

    return std::make_unique<ItemStackRequestPacket>(ItemStackRequestPacket::create(std::move(requests)));
}

BP_FILLER(ItemStackResponsePacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::inventory::stackresponse::ItemStackResponse> responses;
    responses.push_back(makeItemStackResponse(w));
    responses.push_back(makeItemStackResponse(w));

    return std::make_unique<ItemStackResponsePacket>(ItemStackResponsePacket::create(std::move(responses)));
}

BP_FILLER(ItemRegistryPacket, 1)
{
    auto &w = ctx.well;
    auto stringIdA = w.str("stringId0");
    const auto numericIdA = w.i16();
    const auto componentBasedA = w.flag();
    const auto versionA = w.i32();
    auto componentNbtA = makeCacheableNbt(w);
    auto stringIdB = w.str("stringId1");
    const auto numericIdB = w.i16();
    const auto componentBasedB = w.flag();
    const auto versionB = w.i32();
    auto componentNbtB = makeCacheableNbt(w);

    std::vector<types::ItemTypeEntry> entries;
    entries.emplace_back(std::move(stringIdA), numericIdA, componentBasedA, versionA, std::move(componentNbtA));
    entries.emplace_back(std::move(stringIdB), numericIdB, componentBasedB, versionB, std::move(componentNbtB));

    return std::make_unique<ItemRegistryPacket>(ItemRegistryPacket::create(std::move(entries)));
}

BP_FILLER(PlayerEnchantOptionsPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::EnchantOption> options = {makeEnchantOption(w), makeEnchantOption(w)};

    return std::make_unique<PlayerEnchantOptionsPacket>(PlayerEnchantOptionsPacket::create(std::move(options)));
}

BP_FILLER(PlayerArmorDamagePacket, 1)
{
    auto &w = ctx.well;
    // ArmorSlot is validated on decode, so the slots are pinned; two different members, so a pair
    // swapped against the damage values still changes the bytes.
    const auto slotA = ValueWell::pin(types::ArmorSlot::LEGS);
    const auto damageA = w.u16();
    const auto slotB = ValueWell::pin(types::ArmorSlot::BODY);
    const auto damageB = w.u16();
    std::vector<types::ArmorSlotAndDamagePair> armorSlotAndDamagePairs = {{slotA, damageA}, {slotB, damageB}};

    return std::make_unique<PlayerArmorDamagePacket>(
        PlayerArmorDamagePacket::create(std::move(armorSlotAndDamagePairs)));
}

}  // namespace bedrock_protocol::verify
