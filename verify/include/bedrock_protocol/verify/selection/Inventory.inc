/*
 * Selection fragment: Inventory packets.
 * Owned by verify/src/fillers/Fillers_Inventory.cpp. See PacketSelection.inc for the column meanings.
 *
 * Every line added here needs a matching fill<Type> in that file, or the build fails at link time -
 * which is exactly the enforcement this layout is for.
 */

BP_SELECTED_PACKET(UpdateTradePacket, Rare, "FT", 10, 1, "10 fields; CacheableNbt offers written as a raw byte array")
BP_SELECTED_PACKET(InventorySlotPacket, Core, "RFT", 5, 1, "5 fields; two optionals plus a NetworkItemStackDescriptor")
BP_SELECTED_PACKET(CraftingDataPacket, Common, "FT", 5, 1, "5 fields; vector<unique_ptr<RecipeWithTypeId>> mixing MultiRecipe and ShapelessRecipe")
BP_SELECTED_PACKET(UpdateEquipPacket, Rare, "FT", 5, 1, "5 fields; CacheableNbt payload")
BP_SELECTED_PACKET(SetPlayerInventoryOptionsPacket, Common, "RF", 5, 1, "5 fields; all four enums validated on decode")
BP_SELECTED_PACKET(InventoryContentPacket, Core, "RFT", 4, 1, "4 fields; item vector plus two optionals the encoder dereferences unconditionally")
BP_SELECTED_PACKET(ContainerOpenPacket, Core, "RF", 4, 1, "4 fields; BlockPosition and ActorUniqueId")
BP_SELECTED_PACKET(InventoryTransactionPacket, Core, "RT", 3, 1, "3 fields; unique_ptr<TransactionData> plus an optional vector of changed-slot hacks")
BP_SELECTED_PACKET(ContainerClosePacket, Core, "R", 3, 1, "3 fields; two bytes and a bool")
BP_SELECTED_PACKET(ContainerSetDataPacket, Common, "R", 3, 1, "3 fields; byte plus two signed varints")
BP_SELECTED_PACKET(PlayerHotbarPacket, Core, "R", 3, 1, "3 fields; unsigned varint, byte and bool")
BP_SELECTED_PACKET(PlayerToggleCrafterSlotRequestPacket, Rare, "R", 3, 1, "3 fields; BlockPosition written as three LE ints, not a varint triple")
BP_SELECTED_PACKET(CreativeContentPacket, Common, "T", 2, 1, "2 fields; CreativeGroupEntry and CreativeItemEntry vectors carrying full ItemStacks")
BP_SELECTED_PACKET(UnlockedRecipesPacket, Rare, "T", 2, 1, "2 fields; LE u32 type plus a string vector")
BP_SELECTED_PACKET(CompletedUsingItemPacket, Common, "R", 2, 1, "2 fields; LE short and LE int, no varints")
BP_SELECTED_PACKET(ItemStackRequestPacket, Core, "RT", 1, 1, "1 field; vector<ItemStackRequest>, each with three concrete ItemStackRequestAction types")
BP_SELECTED_PACKET(ItemStackResponsePacket, Core, "RT", 1, 1, "1 field; nested response/container/slot vectors")
BP_SELECTED_PACKET(ItemRegistryPacket, Common, "T", 1, 1, "1 field; ItemTypeEntry vector with per-entry CacheableNbt")
BP_SELECTED_PACKET(PlayerEnchantOptionsPacket, Rare, "T", 1, 1, "1 field; EnchantOption with three separate enchant lists")
BP_SELECTED_PACKET(PlayerArmorDamagePacket, Rare, "T", 1, 1, "1 field; ArmorSlot validated on decode")
