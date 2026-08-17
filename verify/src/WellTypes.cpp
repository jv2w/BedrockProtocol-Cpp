/*
 * This file is part of BedrockProtocol-Cpp.
 */

#include "bedrock_protocol/verify/WellTypes.h"

#include "bedrock_protocol/nbt/NBT.h"
#include "bedrock_protocol/nbt/tag/IntTag.h"
#include "bedrock_protocol/nbt/tag/ListTag.h"
#include "bedrock_protocol/nbt/tag/StringTag.h"
#include "bedrock_protocol/protocol/types/AttributesRemoveEnvironment.h"
#include "bedrock_protocol/protocol/types/BoolGameRule.h"
#include "bedrock_protocol/protocol/types/BoolPackSetting.h"
#include "bedrock_protocol/protocol/types/FloatGameRule.h"
#include "bedrock_protocol/protocol/types/IntGameRule.h"
#include "bedrock_protocol/protocol/types/PlayerBlockAction.h"
#include "bedrock_protocol/protocol/types/SyncWorldClockMarkerData.h"
#include "bedrock_protocol/protocol/types/SyncWorldClocksAddTimeMarker.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreUpdate.h"
#include "bedrock_protocol/protocol/types/ddui/update/StringDataStoreUpdateValue.h"
#include "bedrock_protocol/protocol/types/entity/ByteMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/CompoundTagMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/FloatMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/IntMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/LongMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/ShortMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/StringMetadataProperty.h"
#include "bedrock_protocol/protocol/types/entity/Vec3MetadataProperty.h"
#include "bedrock_protocol/protocol/types/inventory/NormalTransactionData.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DestroyStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/DropStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/SwapStackRequestAction.h"
#include "bedrock_protocol/protocol/types/recipe/MultiRecipe.h"
#include "bedrock_protocol/protocol/types/recipe/StringIdMetaItemDescriptor.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeLinePayload.h"

namespace bedrock_protocol::verify {

namespace {

/**
 * A SkinImage side, in pixels.
 *
 * Pinned rather than drawn: SkinImage's constructor throws unless the pixel buffer is exactly
 * height * width * 4 bytes, so height and width are not free values. Kept tiny because the buffer is
 * copied into every packet that carries a skin and the round-trip gains nothing from a large one.
 */
constexpr std::uint32_t SKIN_IMAGE_HEIGHT = 2;
constexpr std::uint32_t SKIN_IMAGE_WIDTH = 3;

/** Fills a pixel buffer of exactly the size SkinImage demands, with per-byte distinct content. */
std::string makeSkinPixels(ValueWell &w)
{
    std::string pixels;
    const std::size_t size = static_cast<std::size_t>(SKIN_IMAGE_HEIGHT) * SKIN_IMAGE_WIDTH * 4;
    pixels.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        pixels.push_back(static_cast<char>(w.u8()));
    }
    return pixels;
}

}  // namespace

types::BlockPosition makeBlockPosition(ValueWell &w)
{
    const auto x = w.i32();
    const auto y = w.i32();
    const auto z = w.i32();
    return {x, y, z};
}

types::ChunkPosition makeChunkPosition(ValueWell &w)
{
    const auto x = w.i32();
    const auto z = w.i32();
    return {x, z};
}

types::SubChunkPosition makeSubChunkPosition(ValueWell &w)
{
    const auto x = w.i32();
    const auto y = w.i32();
    const auto z = w.i32();
    return {x, y, z};
}

color::Color makeColor(ValueWell &w)
{
    // One packed u32 draw rather than four u8 draws. The u8 pool holds 255 tokens per packet, and
    // 1.26.40 turned persona tint colours into a fixed array of four colours per piece
    // (gophertunnel v1.58.0 minecraft/protocol/skin.go:246), which multiplied the channel draws in
    // PlayerListPacket until that pool ran dry. Drawing the colour as one token keeps colours
    // distinct from each other without competing with genuine byte-width fields.
    const auto packed = w.u32();
    // Never 0, matching ValueWell::u8: a channel that is dropped has to change the bytes. The alpha
    // is drawn rather than left at the 0xff default so that dropping or reordering it also shows.
    const auto channel = [](std::uint32_t v) {
        const auto byte = static_cast<std::uint8_t>(v & 0xffu);
        return byte == 0 ? static_cast<std::uint8_t>(1) : byte;
    };
    return {channel(packed >> 24), channel(packed >> 16), channel(packed >> 8), channel(packed)};
}

types::inventory::ItemStack makeItemStack(ValueWell &w)
{
    // putNetworkItemStackDescriptor narrows the id to a signed short, so a wider draw would not
    // survive its own encoding. i16 is also never zero, which keeps the item-stack user-data blob
    // from being suppressed by the air-item rule in putItemStackFooter.
    const auto id = static_cast<std::int32_t>(w.i16());
    const auto meta = w.u32();
    const auto count = w.u16();
    // Written as an unsigned varint on the descriptor path, so it is kept below 2^31.
    const auto blockRuntimeId = static_cast<std::int32_t>(w.u32());
    // Opaque on every wire path: it is copied verbatim as a length-prefixed string, never parsed.
    auto rawExtraData = w.str("rawExtraData");

    return {id, meta, count, blockRuntimeId, std::move(rawExtraData)};
}

types::inventory::ItemStackWrapper makeItemStackWrapper(ValueWell &w)
{
    // Non-zero: the writers treat stackId == 0 as "no net ID" and skip the ID entirely.
    const auto stackId = w.i32();
    auto itemStack = makeItemStack(w);

    return {stackId, std::move(itemStack)};
}

types::inventory::FullContainerName makeFullContainerName(ValueWell &w)
{
    const auto containerId = w.u8();
    const auto dynamicId = w.some(w.u32());

    return types::inventory::FullContainerName(containerId, dynamicId);
}

types::inventory::NetworkInventoryAction makeNetworkInventoryAction(ValueWell &w)
{
    types::inventory::NetworkInventoryAction action;
    action.sourceType = w.u32();
    // windowId goes out as a signed BYTE, so anything wider would not survive its own encoding.
    action.windowId = w.some(static_cast<std::int32_t>(w.i8()));
    action.sourceFlags = w.some(w.u32());
    action.inventorySlot = w.u32();
    action.oldItem = makeItemStackWrapper(w);
    action.newItem = makeItemStackWrapper(w);

    return action;
}

std::unique_ptr<types::inventory::TransactionData> makeTransactionData(ValueWell &w)
{
    std::vector<types::inventory::NetworkInventoryAction> actions;
    actions.push_back(makeNetworkInventoryAction(w));
    actions.push_back(makeNetworkInventoryAction(w));

    return std::make_unique<types::inventory::NormalTransactionData>(
        types::inventory::NormalTransactionData::create(std::move(actions)));
}

types::inventory::stackrequest::ItemStackRequestSlotInfo makeItemStackRequestSlotInfo(ValueWell &w)
{
    auto containerName = makeFullContainerName(w);
    const auto slotId = w.u8();
    const auto stackId = w.i32();

    return {std::move(containerName), slotId, stackId};
}

std::unique_ptr<types::inventory::stackrequest::ItemStackRequestAction> makeItemStackRequestAction(ValueWell &w)
{
    const auto count = w.u8();
    auto source = makeItemStackRequestSlotInfo(w);
    const auto randomly = w.flag();

    return std::make_unique<types::inventory::stackrequest::DropStackRequestAction>(count, std::move(source),
                                                                                    randomly);
}

types::inventory::stackrequest::ItemStackRequest makeItemStackRequest(ValueWell &w)
{
    const auto requestId = w.i32();

    // Three actions of two different concrete types: the per-action type ID is written inline, so a
    // homogeneous list would not prove the dispatch reads the right one for each element.
    std::vector<std::unique_ptr<types::inventory::stackrequest::ItemStackRequestAction>> actions;
    actions.push_back(makeItemStackRequestAction(w));
    {
        const auto count = w.u8();
        auto source = makeItemStackRequestSlotInfo(w);
        actions.push_back(
            std::make_unique<types::inventory::stackrequest::DestroyStackRequestAction>(count, std::move(source)));
    }
    {
        auto slot1 = makeItemStackRequestSlotInfo(w);
        auto slot2 = makeItemStackRequestSlotInfo(w);
        actions.push_back(std::make_unique<types::inventory::stackrequest::SwapStackRequestAction>(std::move(slot1),
                                                                                                   std::move(slot2)));
    }

    std::vector<std::string> filterStrings = {w.str("filter0"), w.str("filter1"), w.str("filter2")};
    const auto filterStringCause = w.i32();

    return {requestId, std::move(actions), std::move(filterStrings), filterStringCause};
}

types::inventory::stackresponse::ItemStackResponseSlotInfo makeItemStackResponseSlotInfo(ValueWell &w)
{
    const auto slot = w.u8();
    const auto hotbarSlot = w.u8();
    const auto count = w.u8();
    // The stack ID is written only when positive, so a negative draw would not survive the round trip.
    const auto itemStackId = static_cast<std::int32_t>(w.u32());
    auto customName = w.str("responseCustomName");
    auto filteredCustomName = w.str("responseFilteredCustomName");
    const auto durabilityCorrection = w.i32();

    return {slot,
            hotbarSlot,
            count,
            itemStackId,
            std::move(customName),
            std::move(filteredCustomName),
            durabilityCorrection};
}

types::inventory::stackresponse::ItemStackResponseContainerInfo makeItemStackResponseContainerInfo(ValueWell &w)
{
    auto containerName = makeFullContainerName(w);
    std::vector<types::inventory::stackresponse::ItemStackResponseSlotInfo> slots = {
        makeItemStackResponseSlotInfo(w), makeItemStackResponseSlotInfo(w)};

    return {std::move(containerName), std::move(slots)};
}

types::inventory::stackresponse::ItemStackResponse makeItemStackResponse(ValueWell &w)
{
    // The container list is now gated on being non-empty rather than on the result, so the result is a
    // plain drawn value.
    const auto result = w.u8();
    const auto requestId = w.i32();
    std::vector<types::inventory::stackresponse::ItemStackResponseContainerInfo> containerInfos = {
        makeItemStackResponseContainerInfo(w), makeItemStackResponseContainerInfo(w)};

    return {result, requestId, std::move(containerInfos)};
}

types::entity::AttributeModifier makeAttributeModifier(ValueWell &w)
{
    auto id = w.str("modifierId");
    auto name = w.str("modifierName");
    const auto amount = w.f32();
    const auto operation = w.i32();
    const auto operand = w.i32();
    const auto serializable = w.flag();

    return {std::move(id), std::move(name), amount, operation, operand, serializable};
}

types::entity::Attribute makeAttribute(ValueWell &w)
{
    auto id = w.str("attributeId");
    const auto min = w.f32();
    const auto max = w.f32();
    const auto current = w.f32();
    const auto defaultValue = w.f32();
    std::vector<types::entity::AttributeModifier> modifiers = {makeAttributeModifier(w), makeAttributeModifier(w)};

    return {std::move(id), min, max, current, defaultValue, std::move(modifiers)};
}

types::entity::EntityLink makeEntityLink(ValueWell &w)
{
    const auto fromActorUniqueId = w.i64();
    const auto toActorUniqueId = w.i64();
    const auto type = w.u8();
    const auto immediate = w.flag();
    const auto causedByRider = w.flag();
    const auto vehicleAngularVelocity = w.f32();

    return {fromActorUniqueId, toActorUniqueId, type, immediate, causedByRider, vehicleAngularVelocity};
}

types::entity::PropertySyncData makePropertySyncData(ValueWell &w)
{
    std::vector<std::pair<std::uint32_t, std::int32_t>> intProperties = {
        {w.u32(), w.i32()}, {w.u32(), w.i32()}, {w.u32(), w.i32()}};
    std::vector<std::pair<std::uint32_t, float>> floatProperties = {{w.u32(), w.f32()}, {w.u32(), w.f32()}};

    return {std::move(intProperties), std::move(floatProperties)};
}

serializer::CommonTypes::EntityMetadata makeEntityMetadata(ValueWell &w)
{
    // One entry per concrete MetadataProperty the reader dispatches on. The type ID is written per
    // entry, so covering only one subclass would leave eight of the nine read branches untouched.
    serializer::CommonTypes::EntityMetadata metadata;
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::ByteMetadataProperty>(w.i8()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::ShortMetadataProperty>(w.i16()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::IntMetadataProperty>(w.i32()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::LongMetadataProperty>(w.i64()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::FloatMetadataProperty>(w.f32()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::StringMetadataProperty>(w.str("metadataString")));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::Vec3MetadataProperty>(w.vec3()));
    metadata.emplace_back(w.u32(), std::make_unique<types::entity::CompoundTagMetadataProperty>(makeCacheableNbt(w)));

    return metadata;
}

std::unique_ptr<nbt::tag::Tag> makeTag(ValueWell &w)
{
    return std::make_unique<nbt::tag::StringTag>(w.str("tagValue"));
}

nbt::tag::CompoundTag makeCompoundTag(ValueWell &w)
{
    nbt::tag::CompoundTag root;
    root.setByte("byte", w.i8());
    root.setShort("short", w.i16());
    root.setInt("int", w.i32());
    root.setLong("long", w.i64());
    root.setFloat("float", w.f32());
    root.setDouble("double", w.f32());
    root.setString("string", w.str("nbtString"));
    root.setByteArray("byteArray", w.str("nbtByteArray"));
    root.setIntArray("intArray", {w.i32(), w.i32(), w.i32()});

    // A list is where the NBT serializer writes an element type and a count separately from the
    // elements; three entries are what make an off-by-one in that loop visible.
    std::vector<std::unique_ptr<nbt::tag::Tag>> listItems;
    listItems.push_back(std::make_unique<nbt::tag::IntTag>(w.i32()));
    listItems.push_back(std::make_unique<nbt::tag::IntTag>(w.i32()));
    listItems.push_back(std::make_unique<nbt::tag::IntTag>(w.i32()));
    root.setTag("list", std::make_unique<nbt::tag::ListTag>(std::move(listItems), nbt::NBT::TAG_Int));

    // Nesting is what exercises the serializer's recursion and its TAG_End terminator handling.
    auto child = std::make_unique<nbt::tag::CompoundTag>();
    child->setInt("childInt", w.i32());
    child->setString("childString", w.str("childString"));
    root.setTag("compound", std::move(child));

    return root;
}

types::CacheableNbt<nbt::tag::CompoundTag> makeCacheableNbt(ValueWell &w)
{
    return types::CacheableNbt<nbt::tag::CompoundTag>(makeCompoundTag(w));
}

serializer::BitSet makeBitSet(ValueWell &w, std::int32_t length)
{
    (void)w;  // the bit indices are positional, not drawn - see below

    serializer::BitSet bits(length);
    // First, middle and last bit rather than random indices: those are the three positions that catch
    // a wrong parts count, since a missing or extra 64-bit part only shows up at the ends.
    if (length > 0) {
        bits.set(0, true);
        bits.set(length / 2, true);
        bits.set(length - 1, true);
    }

    return bits;
}

types::PlayerAuthInputFlagList makePlayerAuthInputFlagList(ValueWell &w, std::int32_t size)
{
    (void)w;  // the flag IDs are positional, not drawn - see below

    types::PlayerAuthInputFlagList flags(size);
    // First, middle and last ID rather than random ones: the ends are what catch an off-by-one in the
    // size bound, which is the only thing read() validates the IDs against.
    if (size > 0) {
        flags.set(0, true);
        flags.set(size / 2, true);
        flags.set(size - 1, true);
    }

    return flags;
}

std::unique_ptr<types::GameRule> makeGameRule(ValueWell &w)
{
    const auto value = w.f32();
    const auto isPlayerModifiable = w.flag();

    return std::make_unique<types::FloatGameRule>(value, isPlayerModifiable);
}

std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>> makeGameRules(ValueWell &w)
{
    // All three concrete rule types, because the rule's type ID is written per entry and the reader
    // dispatches on it; a list of one type would leave two of the three branches untested.
    std::vector<std::pair<std::string, std::unique_ptr<types::GameRule>>> rules;
    rules.emplace_back(w.str("boolRule"), std::make_unique<types::BoolGameRule>(w.flag(), w.flag()));
    rules.emplace_back(w.str("intRule"), std::make_unique<types::IntGameRule>(w.u32(), w.flag()));
    rules.emplace_back(w.str("floatRule"), makeGameRule(w));

    return rules;
}

types::Experiments makeExperiments(ValueWell &w)
{
    std::vector<std::pair<std::string, bool>> experiments = {
        {w.str("experiment0"), w.flag()}, {w.str("experiment1"), w.flag()}, {w.str("experiment2"), w.flag()}};
    const auto hasPreviouslyUsedExperiments = w.flag();

    return {std::move(experiments), hasPreviouslyUsedExperiments};
}

types::SpawnSettings makeSpawnSettings(ValueWell &w)
{
    const auto biomeType = w.u16();
    auto biomeName = w.str("biomeName");
    const auto dimension = w.i32();

    return {biomeType, std::move(biomeName), dimension};
}

types::EducationUriResource makeEducationUriResource(ValueWell &w)
{
    auto buttonName = w.str("buttonName");
    auto linkUri = w.str("linkUri");

    return {std::move(buttonName), std::move(linkUri)};
}

types::LevelSettings makeLevelSettings(ValueWell &w)
{
    types::LevelSettings settings;
    settings.seed = w.u64();
    settings.spawnSettings = makeSpawnSettings(w);
    settings.generator = w.i32();
    settings.worldGamemode = w.i32();
    settings.hardcore = w.flag();
    settings.difficulty = w.i32();
    settings.spawnPosition = makeBlockPosition(w);
    settings.hasAchievementsDisabled = w.flag();
    settings.editorWorldType = w.i32();
    settings.createdInEditorMode = w.flag();
    settings.exportedFromEditorMode = w.flag();
    settings.time = w.i32();
    settings.eduEditionOffer = w.i32();
    settings.hasEduFeaturesEnabled = w.flag();
    settings.eduProductUUID = w.str("eduProductUUID");
    settings.rainLevel = w.f32();
    settings.lightningLevel = w.f32();
    settings.hasConfirmedPlatformLockedContent = w.flag();
    settings.isMultiplayerGame = w.flag();
    settings.hasLANBroadcast = w.flag();
    settings.xboxLiveBroadcastMode = w.i32();
    settings.platformBroadcastMode = w.i32();
    settings.commandsEnabled = w.flag();
    settings.isTexturePacksRequired = w.flag();
    settings.gameRules = makeGameRules(w);
    settings.experiments = makeExperiments(w);
    settings.hasBonusChestEnabled = w.flag();
    settings.hasStartWithMapEnabled = w.flag();
    settings.defaultPlayerPermission = w.i32();
    settings.serverChunkTickRadius = w.i32();
    settings.hasLockedBehaviorPack = w.flag();
    settings.hasLockedResourcePack = w.flag();
    settings.isFromLockedWorldTemplate = w.flag();
    settings.useMsaGamertagsOnly = w.flag();
    settings.isFromWorldTemplate = w.flag();
    settings.isWorldTemplateOptionLocked = w.flag();
    settings.onlySpawnV1Villagers = w.flag();
    settings.disablePersona = w.flag();
    settings.disableCustomSkins = w.flag();
    settings.muteEmoteAnnouncements = w.flag();
    settings.vanillaVersion = w.str("vanillaVersion");
    settings.limitedWorldWidth = w.i32();
    settings.limitedWorldLength = w.i32();
    settings.isNewNether = w.flag();
    settings.eduSharedUriResource = w.some(makeEducationUriResource(w));
    settings.experimentalGameplayOverride = w.some(w.flag());
    settings.chatRestrictionLevel = w.u8();
    settings.disablePlayerInteractions = w.flag();
    settings.serverEditorConnectionPolicy = w.i32();
    settings.allowAnonymousBlockDropsInEditorWorlds = w.flag();

    return settings;
}

types::BlockPaletteEntry makeBlockPaletteEntry(ValueWell &w)
{
    auto name = w.str("blockName");
    auto states = makeCacheableNbt(w);

    return {std::move(name), std::move(states)};
}

types::NetworkPermissions makeNetworkPermissions(ValueWell &w)
{
    return types::NetworkPermissions(w.flag());
}

types::PlayerMovementSettings makePlayerMovementSettings(ValueWell &w)
{
    const auto rewindHistorySize = w.i32();
    const auto serverAuthoritativeBlockBreaking = w.flag();

    return {rewindHistorySize, serverAuthoritativeBlockBreaking};
}

std::unique_ptr<types::PackSetting> makePackSetting(ValueWell &w)
{
    auto name = w.str("packSettingName");
    const auto value = w.flag();

    return std::make_unique<types::BoolPackSetting>(std::move(name), value);
}

types::PlayerBlockAction makePlayerBlockAction(ValueWell &w)
{
    // There is one element shape and no per-action branch, so the action type is a plain value
    // rather than a discriminator (player.go:158-163).
    const auto actionType = w.i32();
    const auto blockPosition = makeBlockPosition(w);
    const auto face = w.i32();

    return {actionType, blockPosition, face};
}

std::unique_ptr<types::shape::PrimitiveShapePayload> makePrimitiveShapePayload(ValueWell &w)
{
    return std::make_unique<types::shape::PrimitiveShapeLinePayload>(w.vec3());
}

std::unique_ptr<types::AttributeLayerSyncPayload> makeAttributeLayerSyncPayload(ValueWell &w)
{
    auto name = w.str("environmentName");
    const auto dimension = w.u32();
    std::vector<std::string> attributes = {w.str("attribute0"), w.str("attribute1"), w.str("attribute2")};

    return std::make_unique<types::AttributesRemoveEnvironment>(std::move(name), dimension, std::move(attributes));
}

std::unique_ptr<types::SyncWorldClocksPayload> makeSyncWorldClocksPayload(ValueWell &w)
{
    const auto clockId = w.u64();

    std::vector<types::SyncWorldClockMarkerData> markers;
    markers.emplace_back(w.u64(), w.str("marker0"), w.i32(), w.some(w.i32()));
    markers.emplace_back(w.u64(), w.str("marker1"), w.i32(), w.some(w.i32()));

    return std::make_unique<types::SyncWorldClocksAddTimeMarker>(clockId, std::move(markers));
}

std::unique_ptr<types::ddui::update::DataStoreUpdateValue> makeDataStoreUpdateValue(ValueWell &w)
{
    // The string variant, because it is the only one whose payload has a variable length: a wrong
    // value-type tag would then change the byte count rather than merely reinterpret fixed-width bytes.
    return std::make_unique<types::ddui::update::StringDataStoreUpdateValue>(w.str("storeValue"));
}

std::unique_ptr<types::ddui::DataStoreOperation> makeDataStoreOperation(ValueWell &w)
{
    auto name = w.str("storeName");
    auto property = w.str("storeProperty");
    auto path = w.str("storePath");
    auto data = makeDataStoreUpdateValue(w);
    const auto updateCount = w.u32();
    const auto pathUpdateCount = w.u32();

    return std::make_unique<types::ddui::DataStoreUpdate>(std::move(name), std::move(property), std::move(path),
                                                          std::move(data), updateCount, pathUpdateCount);
}

types::StructureSettings makeStructureSettings(ValueWell &w)
{
    types::StructureSettings settings;
    settings.paletteName = w.str("paletteName");
    settings.ignoreEntities = w.flag();
    settings.ignoreBlocks = w.flag();
    settings.allowNonTickingChunks = w.flag();
    settings.dimensions = makeBlockPosition(w);
    settings.offset = makeBlockPosition(w);
    settings.lastTouchedByPlayerID = w.i64();
    settings.rotation = w.u8();
    settings.mirror = w.u8();
    settings.animationMode = w.u8();
    settings.animationSeconds = w.f32();
    settings.integrityValue = w.f32();
    settings.integritySeed = w.u32();
    settings.pivot = w.vec3();

    return settings;
}

types::StructureEditorData makeStructureEditorData(ValueWell &w)
{
    types::StructureEditorData data;
    data.structureName = w.str("structureName");
    data.filteredStructureName = w.str("filteredStructureName");
    data.structureDataField = w.str("structureDataField");
    data.includePlayers = w.flag();
    data.showBoundingBox = w.flag();
    // TYPE_SAVE rather than a drawn value: the block type is a discriminator, and TYPE_INVALID or an
    // unknown value would put the decoder on a branch that reads none of the settings back.
    data.structureBlockType = ValueWell::pin(types::StructureEditorData::TYPE_SAVE);
    data.structureSettings = makeStructureSettings(w);
    data.structureRedstoneSaveMode = w.u8();

    return data;
}

types::skin::SkinImage makeSkinImage(ValueWell &w)
{
    return {SKIN_IMAGE_HEIGHT, SKIN_IMAGE_WIDTH, makeSkinPixels(w)};
}

types::skin::SkinAnimation makeSkinAnimation(ValueWell &w)
{
    auto image = makeSkinImage(w);
    const auto type = w.u32();
    const auto frames = w.f32();
    const auto expressionType = w.u32();

    return {std::move(image), type, frames, expressionType};
}

types::skin::PersonaSkinPiece makePersonaSkinPiece(ValueWell &w)
{
    auto pieceId = w.str("pieceId");
    const auto pieceType = w.u32();
    const auto packId = w.uuid();
    const auto isDefaultPiece = w.flag();
    auto productId = w.str("productId");

    return {std::move(pieceId), pieceType, packId, isDefaultPiece, std::move(productId)};
}

types::skin::PersonaPieceTintColor makePersonaPieceTintColor(ValueWell &w)
{
    // Pinned: putSkin strips the persona_ prefix on write and getSkin puts it back, so a filled instance
    // only compares equal to a decoded one if the name survives that round trip.
    auto pieceType = ValueWell::pin(std::string(types::skin::PersonaPieceTintColor::PIECE_TYPE_PERSONA_EYES));
    types::skin::PersonaPieceTintColor::Colors colors = {makeColor(w), makeColor(w), makeColor(w), makeColor(w)};

    return {std::move(pieceType), colors};
}

types::skin::SkinData makeSkinData(ValueWell &w)
{
    auto skinId = w.str("skinId");
    auto playFabId = w.str("playFabId");
    auto resourcePatch = w.str("resourcePatch");
    auto skinImage = makeSkinImage(w);
    std::vector<types::skin::SkinAnimation> animations = {makeSkinAnimation(w), makeSkinAnimation(w)};
    auto capeImage = w.some(makeSkinImage(w));
    auto geometryData = w.str("geometryData");
    auto geometryDataEngineVersion = w.str("geometryDataEngineVersion");
    auto animationData = w.str("animationData");
    auto capeId = w.str("capeId");
    // Engaged: left empty, the constructor substitutes a random UUID and the packet would encode to
    // different bytes on every run, which would make any failure impossible to reproduce.
    auto fullSkinId = w.some(w.str("fullSkinId"));
    const auto armSize = w.u8();
    const auto skinColor = makeColor(w);
    std::vector<types::skin::PersonaSkinPiece> personaPieces = {makePersonaSkinPiece(w), makePersonaSkinPiece(w)};
    std::vector<types::skin::PersonaPieceTintColor> pieceTintColors = {makePersonaPieceTintColor(w),
                                                                      makePersonaPieceTintColor(w)};
    // isVerified is the skin body's trailing "true"/"false" string as of protocol 2168 (skin.go:113-118).
    const auto isVerified = w.flag();
    const auto premium = w.flag();
    const auto persona = w.flag();
    const auto personaCapeOnClassic = w.flag();
    const auto isPrimaryUser = w.flag();
    const auto override = w.flag();
    auto profileHash = w.str("profileHash");

    return {std::move(skinId),
            std::move(playFabId),
            std::move(resourcePatch),
            std::move(skinImage),
            std::move(animations),
            std::move(capeImage),
            std::move(geometryData),
            std::move(geometryDataEngineVersion),
            std::move(animationData),
            std::move(capeId),
            std::move(fullSkinId),
            armSize,
            skinColor,
            std::move(personaPieces),
            std::move(pieceTintColors),
            isVerified,
            premium,
            persona,
            personaCapeOnClassic,
            isPrimaryUser,
            override,
            std::move(profileHash)};
}

types::PlayerListEntry makePlayerListEntry(ValueWell &w)
{
    // Built member-wise rather than through createAdditionEntry, because that factory leaves `color`
    // disengaged by default and the optional colour branch is exactly what needs exercising.
    types::PlayerListEntry entry;
    // ACTION_REMOVE ends the entry after its UUID; only ACTION_ADD puts the rest on the wire.
    entry.actionType = ValueWell::pin(types::PlayerListEntry::ACTION_ADD);
    entry.uuid = w.uuid();
    entry.actorUniqueId = w.i64();
    entry.username = w.str("username");
    entry.skinData = w.some(makeSkinData(w));
    entry.xboxUserId = w.str("xboxUserId");
    entry.platformChatId = w.str("platformChatId");
    entry.buildPlatform = w.i32();
    entry.isTeacher = w.flag();
    entry.isHost = w.flag();
    entry.isSubClient = w.flag();
    entry.color = w.some(makeColor(w));

    return entry;
}

types::command::CommandOriginData makeCommandOriginData(ValueWell &w)
{
    types::command::CommandOriginData data;
    data.type = w.str("originType");
    data.uuid = w.uuid();
    data.requestId = w.str("requestId");
    data.playerActorUniqueId = w.i64();

    return data;
}

types::command::CommandOutputMessage makeCommandOutputMessage(ValueWell &w)
{
    types::command::CommandOutputMessage message;
    message.isInternal = w.flag();
    message.messageId = w.str("messageId");
    message.parameters = {w.str("parameter0"), w.str("parameter1"), w.str("parameter2")};

    return message;
}

std::unique_ptr<types::recipe::ItemDescriptor> makeItemDescriptor(ValueWell &w)
{
    auto id = w.str("descriptorId");
    // The name descriptor carries a signed varint32 meta, so a signed draw is what separates it from a
    // plain varint or a fixed short.
    const auto meta = w.i32();

    return std::make_unique<types::recipe::StringIdMetaItemDescriptor>(std::move(id), meta);
}

types::recipe::MultiRecipe makeRecipe(ValueWell &w)
{
    const auto recipeId = w.uuid();
    const auto recipeNetId = w.u32();

    return {recipeId, recipeNetId};
}

types::ScorePacketEntry makeScorePacketEntry(ValueWell &w, std::uint8_t type)
{
    types::ScorePacketEntry entry;
    entry.scoreboardId = w.i64();
    entry.objectiveName = w.str("objectiveName");
    entry.score = w.i32();
    // The decoder dispatches on this, so it is a pinned legal variant rather than a drawn value. Every
    // branch's members are filled whatever the variant is, so a decoder that reads the wrong branch
    // produces a visibly different value rather than an empty one.
    entry.type = ValueWell::pin(type);
    entry.actorUniqueId = w.some(w.i64());
    entry.customName = w.some(w.str("customName"));

    return entry;
}

types::ScoreboardIdentityPacketEntry makeScoreboardIdentityPacketEntry(ValueWell &w)
{
    types::ScoreboardIdentityPacketEntry entry;
    entry.scoreboardId = w.i64();
    entry.actorUniqueId = w.some(w.i64());

    return entry;
}

}  // namespace bedrock_protocol::verify
