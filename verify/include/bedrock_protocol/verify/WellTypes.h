/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Builders for the nested protocol types that packet fillers need.
 *
 * A filler is a single create() call by design, so every argument has to be an expression. Most of the
 * nested types have no default constructor and several are polymorphic bases, so there is nothing a
 * filler could write inline. These builders are that missing expression: one call per type, each
 * returning a fully populated instance drawn from ValueWell.
 *
 * Two rules govern every value produced here, and both come from what DeepRoundTrip actually asserts -
 * BYTE equality of encode -> decode -> re-encode, never semantic equality:
 *
 *   - Collections carry at least two elements. A length prefix that is written but ignored, and an
 *     off-by-one in a read loop, are both invisible with a single element.
 *   - std::optional members are engaged, so the value branch of readOptional/writeOptional is reached.
 *
 * Discriminators are spelled ValueWell::pin(X), because the decoder branches on them: they must be
 * LEGAL, not unique. Each use names the branch it selects.
 *
 * Not covered
 * -----------
 *   - types::inventory::ItemStackExtraData: ItemStack carries its NBT as an opaque `rawExtraData`
 *     string that every wire path (putItemStackFooter, putNetworkItemStackDescriptor) copies verbatim,
 *     so no builder can influence those bytes beyond the string itself. makeItemStack covers it.
 *   - types::GameRule as a single instance is exposed only through makeGameRule; the vector shape the
 *     packets actually take is makeGameRules, which mixes all three concrete rule types.
 *   - Anything whose encoding consults a populated ItemTypeDictionary. makeItemStack therefore fills
 *     the descriptor form of an item, which is self-describing on the wire; a filler that needs the
 *     dictionary-resolved form has to supply the dictionary itself and cannot use these builders.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/nbt/tag/Tag.h"
#include "bedrock_protocol/protocol/serializer/BitSet.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncPayload.h"
#include "bedrock_protocol/protocol/types/BlockPaletteEntry.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"
#include "bedrock_protocol/protocol/types/ChunkPosition.h"
#include "bedrock_protocol/protocol/types/EducationUriResource.h"
#include "bedrock_protocol/protocol/types/Experiments.h"
#include "bedrock_protocol/protocol/types/GameRule.h"
#include "bedrock_protocol/protocol/types/LevelSettings.h"
#include "bedrock_protocol/protocol/types/NetworkPermissions.h"
#include "bedrock_protocol/protocol/types/PackSetting.h"
#include "bedrock_protocol/protocol/types/PlayerAuthInputFlagList.h"
#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"
#include "bedrock_protocol/protocol/types/PlayerMovementSettings.h"
#include "bedrock_protocol/protocol/types/ScoreboardIdentityPacketEntry.h"
#include "bedrock_protocol/protocol/types/PlayerListEntry.h"
#include "bedrock_protocol/protocol/types/ScorePacketEntry.h"
#include "bedrock_protocol/protocol/types/SpawnSettings.h"
#include "bedrock_protocol/protocol/types/StructureEditorData.h"
#include "bedrock_protocol/protocol/types/StructureSettings.h"
#include "bedrock_protocol/protocol/types/SubChunkPosition.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksPayload.h"
#include "bedrock_protocol/protocol/types/command/CommandOriginData.h"
#include "bedrock_protocol/protocol/types/command/CommandOutputMessage.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreOperation.h"
#include "bedrock_protocol/protocol/types/ddui/update/DataStoreUpdateValue.h"
#include "bedrock_protocol/protocol/types/entity/Attribute.h"
#include "bedrock_protocol/protocol/types/entity/AttributeModifier.h"
#include "bedrock_protocol/protocol/types/entity/EntityLink.h"
#include "bedrock_protocol/protocol/types/entity/PropertySyncData.h"
#include "bedrock_protocol/protocol/types/inventory/FullContainerName.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStack.h"
#include "bedrock_protocol/protocol/types/inventory/ItemStackWrapper.h"
#include "bedrock_protocol/protocol/types/inventory/NetworkInventoryAction.h"
#include "bedrock_protocol/protocol/types/inventory/TransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequest.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestSlotInfo.h"
#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponse.h"
#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.h"
#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.h"
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/MultiRecipe.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/PersonaSkinPiece.h"
#include "bedrock_protocol/protocol/types/skin/SkinAnimation.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"
#include "bedrock_protocol/verify/ValueWell.h"

namespace bedrock_protocol::verify {

// ---- simple value types ----------------------------------------------------------------------
types::BlockPosition makeBlockPosition(ValueWell &w);
types::ChunkPosition makeChunkPosition(ValueWell &w);
types::SubChunkPosition makeSubChunkPosition(ValueWell &w);
color::Color makeColor(ValueWell &w);

// ---- inventory -------------------------------------------------------------------------------
types::inventory::ItemStack makeItemStack(ValueWell &w);
types::inventory::ItemStackWrapper makeItemStackWrapper(ValueWell &w);
types::inventory::FullContainerName makeFullContainerName(ValueWell &w);
types::inventory::NetworkInventoryAction makeNetworkInventoryAction(ValueWell &w);
std::unique_ptr<types::inventory::TransactionData> makeTransactionData(ValueWell &w);
types::inventory::stackrequest::ItemStackRequestSlotInfo makeItemStackRequestSlotInfo(ValueWell &w);
std::unique_ptr<types::inventory::stackrequest::ItemStackRequestAction> makeItemStackRequestAction(ValueWell &w);
types::inventory::stackrequest::ItemStackRequest makeItemStackRequest(ValueWell &w);
types::inventory::stackresponse::ItemStackResponseSlotInfo makeItemStackResponseSlotInfo(ValueWell &w);
types::inventory::stackresponse::ItemStackResponseContainerInfo makeItemStackResponseContainerInfo(ValueWell &w);
types::inventory::stackresponse::ItemStackResponse makeItemStackResponse(ValueWell &w);

// ---- entity ----------------------------------------------------------------------------------
types::entity::AttributeModifier makeAttributeModifier(ValueWell &w);
types::entity::Attribute makeAttribute(ValueWell &w);
types::entity::EntityLink makeEntityLink(ValueWell &w);
types::entity::PropertySyncData makePropertySyncData(ValueWell &w);
serializer::CommonTypes::EntityMetadata makeEntityMetadata(ValueWell &w);

// ---- NBT -------------------------------------------------------------------------------------
std::unique_ptr<nbt::tag::Tag> makeTag(ValueWell &w);
nbt::tag::CompoundTag makeCompoundTag(ValueWell &w);
types::CacheableNbt<nbt::tag::CompoundTag> makeCacheableNbt(ValueWell &w);

// ---- serializer ------------------------------------------------------------------------------
/**
 * @param length the exact bit length the owning packet decodes with, e.g. PlayerAuthInputPacket::FLAG_LENGTH.
 *               BitSet writes and reads a length-derived number of parts, so a mismatch desynchronises
 *               the whole remainder of the packet.
 */
serializer::BitSet makeBitSet(ValueWell &w, std::int32_t length);
types::PlayerAuthInputFlagList makePlayerAuthInputFlagList(ValueWell &w, std::int32_t size);

// ---- level and game --------------------------------------------------------------------------
std::unique_ptr<types::GameRule> makeGameRule(ValueWell &w);
std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>> makeGameRules(ValueWell &w);
types::LevelSettings makeLevelSettings(ValueWell &w);
types::Experiments makeExperiments(ValueWell &w);
types::BlockPaletteEntry makeBlockPaletteEntry(ValueWell &w);
types::NetworkPermissions makeNetworkPermissions(ValueWell &w);
types::PlayerMovementSettings makePlayerMovementSettings(ValueWell &w);
types::SpawnSettings makeSpawnSettings(ValueWell &w);
types::EducationUriResource makeEducationUriResource(ValueWell &w);
std::unique_ptr<types::PackSetting> makePackSetting(ValueWell &w);
types::PlayerBlockAction makePlayerBlockAction(ValueWell &w);
std::unique_ptr<types::shape::PrimitiveShapePayload> makePrimitiveShapePayload(ValueWell &w);
std::unique_ptr<types::AttributeLayerSyncPayload> makeAttributeLayerSyncPayload(ValueWell &w);
std::unique_ptr<types::SyncWorldClocksPayload> makeSyncWorldClocksPayload(ValueWell &w);
std::unique_ptr<types::ddui::update::DataStoreUpdateValue> makeDataStoreUpdateValue(ValueWell &w);
std::unique_ptr<types::ddui::DataStoreOperation> makeDataStoreOperation(ValueWell &w);
types::StructureSettings makeStructureSettings(ValueWell &w);
types::StructureEditorData makeStructureEditorData(ValueWell &w);

// ---- login and skin --------------------------------------------------------------------------
types::skin::SkinImage makeSkinImage(ValueWell &w);
types::skin::SkinAnimation makeSkinAnimation(ValueWell &w);
types::skin::PersonaSkinPiece makePersonaSkinPiece(ValueWell &w);
types::skin::PersonaPieceTintColor makePersonaPieceTintColor(ValueWell &w);
types::skin::SkinData makeSkinData(ValueWell &w);
types::PlayerListEntry makePlayerListEntry(ValueWell &w);

// ---- command ---------------------------------------------------------------------------------
types::command::CommandOriginData makeCommandOriginData(ValueWell &w);
types::command::CommandOutputMessage makeCommandOutputMessage(ValueWell &w);

// ---- recipe ----------------------------------------------------------------------------------
std::unique_ptr<types::recipe::ItemDescriptor> makeItemDescriptor(ValueWell &w);
types::recipe::MultiRecipe makeRecipe(ValueWell &w);

// ---- scoreboard ------------------------------------------------------------------------------
types::ScorePacketEntry makeScorePacketEntry(ValueWell &w, std::uint8_t type);
types::ScoreboardIdentityPacketEntry makeScoreboardIdentityPacketEntry(ValueWell &w);

}  // namespace bedrock_protocol::verify
