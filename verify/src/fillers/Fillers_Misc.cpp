/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: resource packs, commands, structures, maps, diagnostics and skins.
 *
 * The two conventions from Fillers_Core.cpp hold here as well: one create() call with every argument
 * spelled out, and every value drawn into a named local first.
 *
 * Packets in this file with no generated create(), and why:
 *   - ClientboundMapItemDataPacket: its `type` field is a bitmask DERIVED by encodePayload from which
 *     of parentMapIds / decorations / colors are populated, so a factory taking `type` would be able
 *     to construct instances that cannot exist on the wire.
 *   - BookEditPacket: `type` selects which of the remaining fields are written at all, so no factory
 *     can take all nine and mean anything.
 *   - CommandOutputPacket: PHP has no @generate-create-func on it either.
 */

#include "bedrock_protocol/verify/Filler.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "bedrock_protocol/protocol/AvailableActorIdentifiersPacket.h"
#include "bedrock_protocol/protocol/AvailableCommandsPacket.h"
#include "bedrock_protocol/protocol/BiomeDefinitionListPacket.h"
#include "bedrock_protocol/protocol/BookEditPacket.h"
#include "bedrock_protocol/protocol/ClientboundMapItemDataPacket.h"
#include "bedrock_protocol/protocol/CommandOutputPacket.h"
#include "bedrock_protocol/protocol/CommandRequestPacket.h"
#include "bedrock_protocol/protocol/MapInfoRequestPacket.h"
#include "bedrock_protocol/protocol/PlayerListPacket.h"
#include "bedrock_protocol/protocol/PlayerSkinPacket.h"
#include "bedrock_protocol/protocol/RequestAbilityPacket.h"
#include "bedrock_protocol/protocol/ResourcePackChunkDataPacket.h"
#include "bedrock_protocol/protocol/ResourcePackChunkRequestPacket.h"
#include "bedrock_protocol/protocol/ResourcePackClientResponsePacket.h"
#include "bedrock_protocol/protocol/ResourcePackDataInfoPacket.h"
#include "bedrock_protocol/protocol/ResourcePackStackPacket.h"
#include "bedrock_protocol/protocol/ResourcePacksInfoPacket.h"
#include "bedrock_protocol/protocol/ServerboundDiagnosticsPacket.h"
#include "bedrock_protocol/protocol/SettingsCommandPacket.h"
#include "bedrock_protocol/protocol/StructureBlockUpdatePacket.h"
#include "bedrock_protocol/protocol/StructureTemplateDataRequestPacket.h"
#include "bedrock_protocol/protocol/StructureTemplateDataResponsePacket.h"
#include "bedrock_protocol/protocol/UpdateAbilitiesPacket.h"
#include "bedrock_protocol/protocol/UpdateAdventureSettingsPacket.h"
#include "bedrock_protocol/protocol/UpdateSoftEnumPacket.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {
namespace {

// ---- builders needed only by this file --------------------------------------------------------
//
// These stop at the boundary of what the owning packet's wire format actually reaches. Nothing here
// is reused by another filler file, so they stay file-local rather than growing WellTypes.

types::AbilitiesLayer makeAbilitiesLayer(ValueWell &w)
{
    const auto layerId = w.u16();
    // Ability indexes are positions in a 32-bit flag word, so they must be LEGAL rather than unique.
    // 13/14/19 are deliberately absent: those three bits are owned by the fly/walk/vertical-fly speed
    // optionals below, and setting them from the bool map as well would make the two sources of the
    // same bit indistinguishable in a failure.
    std::unordered_map<std::int32_t, bool> boolAbilities;
    boolAbilities[ValueWell::pin(types::AbilitiesLayer::ABILITY_BUILD)] = w.flag();
    boolAbilities[ValueWell::pin(types::AbilitiesLayer::ABILITY_MINE)] = w.flag();
    boolAbilities[ValueWell::pin(types::AbilitiesLayer::ABILITY_OPERATOR)] = w.flag();
    boolAbilities[ValueWell::pin(types::AbilitiesLayer::ABILITY_FLYING)] = w.flag();
    boolAbilities[ValueWell::pin(types::AbilitiesLayer::ABILITY_NO_CLIP)] = w.flag();
    // All three engaged: a disengaged speed writes a hardcoded 0.0f and its flag bit stays clear, so
    // the value path would never be reached.
    const auto flySpeed = w.some(w.f32());
    const auto verticalFlySpeed = w.some(w.f32());
    const auto walkSpeed = w.some(w.f32());

    return {layerId, std::move(boolAbilities), flySpeed, verticalFlySpeed, walkSpeed};
}

types::AbilitiesData makeAbilitiesData(ValueWell &w)
{
    const auto commandPermission = w.u8();
    const auto playerPermission = w.u8();
    const auto targetActorUniqueId = w.i64();
    std::vector<types::AbilitiesLayer> layers = {makeAbilitiesLayer(w), makeAbilitiesLayer(w)};

    return {commandPermission, playerPermission, targetActorUniqueId, std::move(layers)};
}

types::resourcepacks::ResourcePackInfoEntry makeResourcePackInfoEntry(ValueWell &w)
{
    const auto packId = w.uuid();
    auto version = w.str("packVersion");
    const auto sizeBytes = w.u64();
    auto encryptionKey = w.str("encryptionKey");
    auto subPackName = w.str("subPackName");
    auto contentId = w.str("contentId");
    const auto hasScripts = w.flag();
    const auto isAddonPack = w.flag();
    const auto isRtxCapable = w.flag();
    auto cdnUrl = w.str("cdnUrl");

    return {packId,   std::move(version),   sizeBytes,    std::move(encryptionKey), std::move(subPackName),
            std::move(contentId), hasScripts, isAddonPack, isRtxCapable, std::move(cdnUrl)};
}

types::resourcepacks::ResourcePackStackEntry makeResourcePackStackEntry(ValueWell &w)
{
    auto packId = w.str("stackPackId");
    auto version = w.str("stackVersion");
    auto subPackName = w.str("stackSubPackName");

    return {std::move(packId), std::move(version), std::move(subPackName)};
}

types::MapInfoRequestPacketClientPixel makeClientPixel(ValueWell &w)
{
    const auto color = makeColor(w);
    // x/y are re-derived on decode from a single packed index (y * 128 + x), so x has to stay below
    // that multiplier or the pair cannot survive its own encoding.
    const auto x = ValueWell::pin<std::uint16_t>(37);
    const auto y = ValueWell::pin<std::uint16_t>(94);

    return {color, x, y};
}

types::MapDecoration makeMapDecoration(ValueWell &w)
{
    const auto icon = w.u8();
    const auto rotation = w.u8();
    const auto xOffset = w.u8();
    const auto yOffset = w.u8();
    auto label = w.str("decorationLabel");
    const auto color = makeColor(w);

    return {icon, rotation, xOffset, yOffset, std::move(label), color};
}

types::command::raw::CommandParameterRawData makeCommandParameter(ValueWell &w)
{
    auto name = w.str("paramName");
    // typeInfo is an opaque 32-bit word on the wire - CommandParameterRawData neither validates nor
    // resolves it - but it is composed of ARG_FLAG_VALID plus a type, so a legal shape is used to
    // keep the bytes representative of real traffic.
    const auto typeInfo = ValueWell::pin<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID |
                                                        AvailableCommandsPacket::ARG_TYPE_STRING);
    const auto optional = w.flag();
    const auto flags = w.u8();

    return {std::move(name), typeInfo, optional, flags};
}

types::command::raw::CommandOverloadRawData makeCommandOverload(ValueWell &w)
{
    const auto chaining = w.flag();
    std::vector<types::command::raw::CommandParameterRawData> parameters = {makeCommandParameter(w),
                                                                            makeCommandParameter(w)};

    return {chaining, std::move(parameters)};
}

types::command::raw::CommandRawData makeCommandRawData(ValueWell &w)
{
    auto name = w.str("commandName");
    auto description = w.str("commandDescription");
    const auto flags = w.u16();
    auto permission = w.str("commandPermission");
    const auto aliasEnumIndex = w.i32();
    std::vector<std::uint32_t> chainedSubCommandDataIndexes = {w.u32(), w.u32()};
    std::vector<types::command::raw::CommandOverloadRawData> overloads = {makeCommandOverload(w),
                                                                          makeCommandOverload(w)};

    return {std::move(name),
            std::move(description),
            flags,
            std::move(permission),
            aliasEnumIndex,
            std::move(chainedSubCommandDataIndexes),
            std::move(overloads)};
}

types::command::raw::CommandEnumRawData makeCommandEnumRawData(ValueWell &w)
{
    auto name = w.str("enumName");
    std::vector<std::uint32_t> valueIndexes = {w.u32(), w.u32(), w.u32()};

    return {std::move(name), std::move(valueIndexes)};
}

types::command::raw::ChainedSubCommandRawData makeChainedSubCommandRawData(ValueWell &w)
{
    auto name = w.str("subCommandName");
    std::vector<types::command::raw::ChainedSubCommandValueRawData> valueData = {
        {w.u32(), w.u32()}, {w.u32(), w.u32()}};

    return {std::move(name), std::move(valueData)};
}

types::command::CommandSoftEnum makeCommandSoftEnum(ValueWell &w)
{
    auto name = w.str("softEnumName");
    std::vector<std::string> values = {w.str("softEnumValue0"), w.str("softEnumValue1")};

    return {std::move(name), std::move(values)};
}

types::command::raw::CommandEnumConstraintRawData makeCommandEnumConstraint(ValueWell &w)
{
    const auto affectedValueIndex = w.u32();
    const auto enumIndex = w.u32();
    std::vector<std::uint8_t> constraints = {w.u8(), w.u8()};

    return {affectedValueIndex, enumIndex, std::move(constraints)};
}

types::biome::BiomeDefinitionData makeBiomeDefinitionData(ValueWell &w)
{
    const auto nameIndex = w.u16();
    const auto id = w.u16();
    const auto temperature = w.f32();
    const auto downfall = w.f32();
    const auto foliageSnow = w.f32();
    const auto depth = w.f32();
    const auto scale = w.f32();
    const auto mapWaterColor = makeColor(w);
    const auto rain = w.flag();
    std::optional<std::vector<std::uint16_t>> tagIndexes = std::vector<std::uint16_t>{w.u16(), w.u16(), w.u16()};
    // Left disengaged. chunkGenData is a 22-type tree of its own, and nothing in this batch would
    // build it; the absent branch of writeOptional is still exercised. See the batch report.
    std::optional<types::biome::chunkgen::BiomeDefinitionChunkGenData> chunkGenData = std::nullopt;

    return {nameIndex, id,    temperature,   downfall, foliageSnow,           depth,
            scale,     mapWaterColor, rain, std::move(tagIndexes), std::move(chunkGenData)};
}

}  // namespace

BP_FILLER(ServerboundDiagnosticsPacket, 13)
{
    auto &w = ctx.well;
    const auto avgFps = w.f32();
    const auto avgServerSimTickTimeMS = w.f32();
    const auto avgClientSimTickTimeMS = w.f32();
    const auto avgBeginFrameTimeMS = w.f32();
    const auto avgInputTimeMS = w.f32();
    const auto avgRenderTimeMS = w.f32();
    const auto avgEndFrameTimeMS = w.f32();
    const auto avgRemainderTimePercent = w.f32();
    const auto avgUnaccountedTimePercent = w.f32();
    std::vector<types::MemoryCategoryCounter> memoryCategoryValues = {{w.u8(), w.u64()}, {w.u8(), w.u64()}};
    std::vector<types::EntityDiagnosticTimingInfo> entityDiagnostics = {
        {w.str("entityDisplayName0"), w.str("entity0"), w.u64(), w.u8()},
        {w.str("entityDisplayName1"), w.str("entity1"), w.u64(), w.u8()}};
    std::vector<types::SystemDiagnosticTimingInfo> systemDiagnostics = {
        {w.str("systemDisplayName0"), w.u64(), w.u64(), w.u8()},
        {w.str("systemDisplayName1"), w.u64(), w.u64(), w.u8()}};
    std::vector<types::WhiskerScopeDataSummary> whiskerScopes = {
        {w.str("whiskerLabel0"), w.str("whiskerIndent0"), w.u64(), w.u64(), w.u64()},
        {w.str("whiskerLabel1"), w.str("whiskerIndent1"), w.u64(), w.u64(), w.u64()}};

    return std::make_unique<ServerboundDiagnosticsPacket>(ServerboundDiagnosticsPacket::create(
        avgFps, avgServerSimTickTimeMS, avgClientSimTickTimeMS, avgBeginFrameTimeMS, avgInputTimeMS, avgRenderTimeMS,
        avgEndFrameTimeMS, avgRemainderTimePercent, avgUnaccountedTimePercent, std::move(memoryCategoryValues),
        std::move(entityDiagnostics), std::move(systemDiagnostics), std::move(whiskerScopes)));
}

BP_FILLER_NOCREATE(ClientboundMapItemDataPacket, 12)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<ClientboundMapItemDataPacket>();

    packet->mapId = w.i64();
    packet->dimensionId = w.u8();
    packet->isLocked = w.flag();
    packet->origin = makeBlockPosition(w);
    // parentMapIds, decorations and colors are all populated because encodePayload derives `type`
    // from exactly those three: leaving any of them empty would clear its bitflag and silently drop
    // a third of the packet from the wire.
    packet->parentMapIds = {w.i64(), w.i64()};
    packet->scale = w.u8();
    // One of each tracked-object kind, so both arms of the TYPE_BLOCK / TYPE_ENTITY branch run.
    types::MapTrackedObject blockObject;
    blockObject.type = ValueWell::pin(types::MapTrackedObject::TYPE_BLOCK);
    blockObject.blockPosition = makeBlockPosition(w);
    types::MapTrackedObject entityObject;
    entityObject.type = ValueWell::pin(types::MapTrackedObject::TYPE_ENTITY);
    entityObject.actorUniqueId = w.i64();
    packet->trackedEntities = {blockObject, entityObject};
    packet->decorations = {makeMapDecoration(w), makeMapDecoration(w)};
    packet->xOffset = w.i32();
    packet->yOffset = w.i32();
    // Deliberately not square: width and height are written in one order and consumed in another, so
    // a 2x2 image would hide a transposition.
    packet->colors = types::MapImage({{makeColor(w), makeColor(w), makeColor(w)},
                                      {makeColor(w), makeColor(w), makeColor(w)}});
    // Derived, never read by encodePayload - set to what encodePayload will compute so the filled
    // instance matches a decoded one on this member too.
    packet->type = ClientboundMapItemDataPacket::BITFLAG_MAP_CREATION |
                   ClientboundMapItemDataPacket::BITFLAG_DECORATION_UPDATE |
                   ClientboundMapItemDataPacket::BITFLAG_TEXTURE_UPDATE;

    return packet;
}

BP_FILLER_NOCREATE(BookEditPacket, 9)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<BookEditPacket>();

    // TYPE_REPLACE_PAGE writes pageNumber, text and photoName - five of the nine fields, the most any
    // single branch reaches. TYPE_SIGN_BOOK reaches five too but skips pageNumber, the only field the
    // three page branches share and therefore the one most worth exercising.
    packet->type = ValueWell::pin<std::uint32_t>(BookEditPacket::TYPE_REPLACE_PAGE);
    packet->inventorySlot = w.i32();
    packet->pageNumber = w.i32();
    packet->secondaryPageNumber = w.i32();
    packet->text = w.str("text");
    packet->photoName = w.str("photoName");
    packet->title = w.str("title");
    packet->author = w.str("author");
    packet->xuid = w.str("xuid");

    return packet;
}

BP_FILLER(AvailableCommandsPacket, 8)
{
    auto &w = ctx.well;
    // The eight vectors are parallel index spaces on the wire but the raw layer resolves none of
    // them, so the indexes only have to be distinctive, not consistent with each other.
    std::vector<std::string> enumValues = {w.str("enumValue0"), w.str("enumValue1"), w.str("enumValue2")};
    std::vector<std::string> chainedSubCommandValues = {w.str("chainedValue0"), w.str("chainedValue1")};
    std::vector<std::string> postfixes = {w.str("postfix0"), w.str("postfix1")};
    std::vector<types::command::raw::CommandEnumRawData> enums = {makeCommandEnumRawData(w),
                                                                  makeCommandEnumRawData(w)};
    std::vector<types::command::raw::ChainedSubCommandRawData> chainedSubCommandData = {
        makeChainedSubCommandRawData(w), makeChainedSubCommandRawData(w)};
    std::vector<types::command::raw::CommandRawData> commandData = {makeCommandRawData(w), makeCommandRawData(w)};
    std::vector<types::command::CommandSoftEnum> softEnums = {makeCommandSoftEnum(w), makeCommandSoftEnum(w)};
    std::vector<types::command::raw::CommandEnumConstraintRawData> enumConstraints = {makeCommandEnumConstraint(w),
                                                                                      makeCommandEnumConstraint(w)};

    return std::make_unique<AvailableCommandsPacket>(AvailableCommandsPacket::create(
        std::move(enumValues), std::move(chainedSubCommandValues), std::move(postfixes), std::move(enums),
        std::move(chainedSubCommandData), std::move(commandData), std::move(softEnums), std::move(enumConstraints)));
}

BP_FILLER(ResourcePacksInfoPacket, 7)
{
    auto &w = ctx.well;
    std::vector<types::resourcepacks::ResourcePackInfoEntry> resourcePackEntries = {makeResourcePackInfoEntry(w),
                                                                                    makeResourcePackInfoEntry(w)};
    const auto mustAccept = w.flag();
    const auto hasAddons = w.flag();
    const auto hasScripts = w.flag();
    const auto worldTemplateId = w.uuid();
    auto worldTemplateVersion = w.str("worldTemplateVersion");
    const auto forceDisableVibrantVisuals = w.flag();

    return std::make_unique<ResourcePacksInfoPacket>(
        ResourcePacksInfoPacket::create(std::move(resourcePackEntries), mustAccept, hasAddons, hasScripts,
                                        worldTemplateId, std::move(worldTemplateVersion), forceDisableVibrantVisuals));
}

BP_FILLER(ResourcePackDataInfoPacket, 7)
{
    auto &w = ctx.well;
    auto packId = w.str("packId");
    const auto maxChunkSize = w.u32();
    const auto chunkCount = w.u32();
    const auto compressedPackSize = w.u64();
    auto sha256 = w.str("sha256");
    const auto isPremium = w.flag();
    const auto packType = w.u8();

    return std::make_unique<ResourcePackDataInfoPacket>(ResourcePackDataInfoPacket::create(
        std::move(packId), maxChunkSize, chunkCount, compressedPackSize, std::move(sha256), isPremium, packType));
}

BP_FILLER_NOCREATE(CommandOutputPacket, 5)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<CommandOutputPacket>();

    packet->originData = makeCommandOriginData(w);
    packet->outputType = w.str("outputType");
    packet->successCount = w.u32();
    packet->messages = {makeCommandOutputMessage(w), makeCommandOutputMessage(w)};
    packet->data = w.some(w.str("data"));

    return packet;
}

BP_FILLER(ResourcePackStackPacket, 5)
{
    auto &w = ctx.well;
    std::vector<types::resourcepacks::ResourcePackStackEntry> resourcePackStack = {makeResourcePackStackEntry(w),
                                                                                   makeResourcePackStackEntry(w)};
    const auto mustAccept = w.flag();
    auto baseGameVersion = w.str("baseGameVersion");
    auto experiments = makeExperiments(w);
    const auto useVanillaEditorPacks = w.flag();

    return std::make_unique<ResourcePackStackPacket>(
        ResourcePackStackPacket::create(std::move(resourcePackStack), mustAccept, std::move(baseGameVersion),
                                        std::move(experiments), useVanillaEditorPacks));
}

BP_FILLER(UpdateAdventureSettingsPacket, 5)
{
    auto &w = ctx.well;
    const auto noAttackingMobs = w.flag();
    const auto noAttackingPlayers = w.flag();
    const auto worldImmutable = w.flag();
    const auto showNameTags = w.flag();
    const auto autoJump = w.flag();

    return std::make_unique<UpdateAdventureSettingsPacket>(UpdateAdventureSettingsPacket::create(
        noAttackingMobs, noAttackingPlayers, worldImmutable, showNameTags, autoJump));
}

BP_FILLER(CommandRequestPacket, 4)
{
    auto &w = ctx.well;
    auto command = w.str("command");
    auto originData = makeCommandOriginData(w);
    const auto isInternal = w.flag();
    auto version = w.str("version");

    return std::make_unique<CommandRequestPacket>(
        CommandRequestPacket::create(std::move(command), std::move(originData), isInternal, std::move(version)));
}

BP_FILLER(ResourcePackChunkDataPacket, 4)
{
    auto &w = ctx.well;
    auto packId = w.str("packId");
    const auto chunkIndex = w.u32();
    const auto offset = w.u64();
    auto data = w.str("data");

    return std::make_unique<ResourcePackChunkDataPacket>(
        ResourcePackChunkDataPacket::create(std::move(packId), chunkIndex, offset, std::move(data)));
}

BP_FILLER(StructureBlockUpdatePacket, 4)
{
    auto &w = ctx.well;
    auto blockPosition = makeBlockPosition(w);
    auto structureEditorData = makeStructureEditorData(w);
    const auto isPowered = w.flag();
    const auto waterlogged = w.flag();

    return std::make_unique<StructureBlockUpdatePacket>(StructureBlockUpdatePacket::create(
        std::move(blockPosition), std::move(structureEditorData), isPowered, waterlogged));
}

BP_FILLER(StructureTemplateDataRequestPacket, 4)
{
    auto &w = ctx.well;
    auto structureTemplateName = w.str("structureTemplateName");
    auto structureBlockPosition = makeBlockPosition(w);
    auto structureSettings = makeStructureSettings(w);
    const auto requestType = ValueWell::pin<std::uint8_t>(StructureTemplateDataRequestPacket::TYPE_QUERY_SAVED_STRUCTURE);

    return std::make_unique<StructureTemplateDataRequestPacket>(
        StructureTemplateDataRequestPacket::create(std::move(structureTemplateName), std::move(structureBlockPosition),
                                                   std::move(structureSettings), requestType));
}

BP_FILLER(PlayerSkinPacket, 4)
{
    auto &w = ctx.well;
    const auto uuid = w.uuid();
    auto oldSkinName = w.str("oldSkinName");
    auto newSkinName = w.str("newSkinName");
    auto skin = makeSkinData(w);

    return std::make_unique<PlayerSkinPacket>(
        PlayerSkinPacket::create(uuid, std::move(oldSkinName), std::move(newSkinName), std::move(skin)));
}

BP_FILLER(UpdateSoftEnumPacket, 3)
{
    auto &w = ctx.well;
    auto enumName = w.str("enumName");
    std::vector<std::string> values = {w.str("value0"), w.str("value1"), w.str("value2")};
    const auto type = ValueWell::pin<std::uint8_t>(UpdateSoftEnumPacket::TYPE_SET);

    return std::make_unique<UpdateSoftEnumPacket>(
        UpdateSoftEnumPacket::create(std::move(enumName), std::move(values), type));
}

BP_FILLER(StructureTemplateDataResponsePacket, 3)
{
    auto &w = ctx.well;
    auto structureTemplateName = w.str("structureTemplateName");
    // Engaged: encodePayload writes the NBT blob only when this is present.
    auto nbtData = w.some(makeCacheableNbt(w));
    const auto responseType = ValueWell::pin<std::uint8_t>(StructureTemplateDataResponsePacket::TYPE_QUERY);

    return std::make_unique<StructureTemplateDataResponsePacket>(StructureTemplateDataResponsePacket::create(
        std::move(structureTemplateName), std::move(nbtData), responseType));
}

BP_FILLER(PlayerListPacket, 2)
{
    auto &w = ctx.well;
    // TYPE_REMOVE writes nothing but the UUID of each entry - eleven of the twelve members of every
    // PlayerListEntry, SkinData included, are only on the wire under TYPE_ADD.
    const auto type = ValueWell::pin(PlayerListPacket::TYPE_ADD);
    std::vector<types::PlayerListEntry> entries = {makePlayerListEntry(w), makePlayerListEntry(w)};

    return std::make_unique<PlayerListPacket>(PlayerListPacket::create(type, std::move(entries)));
}

BP_FILLER(MapInfoRequestPacket, 2)
{
    auto &w = ctx.well;
    const auto mapId = w.i64();
    std::vector<types::MapInfoRequestPacketClientPixel> clientPixels = {makeClientPixel(w), makeClientPixel(w)};

    return std::make_unique<MapInfoRequestPacket>(MapInfoRequestPacket::create(mapId, std::move(clientPixels)));
}

BP_FILLER(SettingsCommandPacket, 2)
{
    auto &w = ctx.well;
    auto command = w.str("command");
    const auto suppressOutput = w.flag();

    return std::make_unique<SettingsCommandPacket>(
        SettingsCommandPacket::create(std::move(command), suppressOutput));
}

BP_FILLER(ResourcePackChunkRequestPacket, 2)
{
    auto &w = ctx.well;
    auto packId = w.str("packId");
    const auto chunkIndex = w.u32();

    return std::make_unique<ResourcePackChunkRequestPacket>(
        ResourcePackChunkRequestPacket::create(std::move(packId), chunkIndex));
}

BP_FILLER(ResourcePackClientResponsePacket, 2)
{
    auto &w = ctx.well;
    const auto status = ValueWell::pin<std::uint8_t>(ResourcePackClientResponsePacket::STATUS_SEND_PACKS);
    std::vector<std::string> packIds = {w.str("packId0"), w.str("packId1"), w.str("packId2")};

    return std::make_unique<ResourcePackClientResponsePacket>(
        ResourcePackClientResponsePacket::create(status, std::move(packIds)));
}

BP_FILLER(RequestAbilityPacket, 2)
{
    auto &w = ctx.well;
    const auto abilityId = w.i32();
    // The float alternative. Both a bool and a float are always on the wire; the alternative held is
    // what selects which of the two the decoder keeps, and the float carries actual information
    // whereas the bool arm would pin the float to a hardcoded zero.
    const std::variant<bool, float> abilityValue = w.f32();

    return std::make_unique<RequestAbilityPacket>(RequestAbilityPacket::create(abilityId, abilityValue));
}

BP_FILLER(BiomeDefinitionListPacket, 2)
{
    auto &w = ctx.well;
    std::vector<types::biome::BiomeDefinitionData> definitionData = {makeBiomeDefinitionData(w),
                                                                     makeBiomeDefinitionData(w)};
    std::vector<std::string> strings = {w.str("biomeString0"), w.str("biomeString1"), w.str("biomeString2")};

    return std::make_unique<BiomeDefinitionListPacket>(
        BiomeDefinitionListPacket::create(std::move(definitionData), std::move(strings)));
}

BP_FILLER(UpdateAbilitiesPacket, 1)
{
    auto &w = ctx.well;
    auto data = makeAbilitiesData(w);

    return std::make_unique<UpdateAbilitiesPacket>(UpdateAbilitiesPacket::create(std::move(data)));
}

BP_FILLER(AvailableActorIdentifiersPacket, 1)
{
    auto &w = ctx.well;
    auto identifiers = makeCacheableNbt(w);

    return std::make_unique<AvailableActorIdentifiersPacket>(
        AvailableActorIdentifiersPacket::create(std::move(identifiers)));
}

}  // namespace bedrock_protocol::verify
