/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: camera, registry and deeply-nested collection packets. See Fillers_Core.cpp for the two
 * conventions that hold across every filler file (one create() call with every argument spelled out;
 * values drawn into named locals first, never inline).
 *
 * The nested-type builders in this file are local rather than in WellTypes.h on purpose: each is used
 * by exactly one packet here, so there is nothing for a shared header to share.
 *
 * Three packets use BP_FILLER_NOCREATE, each for a stated reason at the filler.
 *
 * Where a nested element is built inline it is built with BRACE initialisation, never emplace_back:
 * the elements of a braced-init-list are sequenced left to right, whereas emplace_back's arguments
 * are function arguments and would be drawn in an unspecified order - the same reason the packet-level
 * convention forbids drawing inline into create().
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/CameraAimAssistActorPriorityPacket.h"
#include "bedrock_protocol/protocol/CameraAimAssistPresetsPacket.h"
#include "bedrock_protocol/protocol/CameraSplinePacket.h"
#include "bedrock_protocol/protocol/ClientCacheMissResponsePacket.h"
#include "bedrock_protocol/protocol/ClientCameraAimAssistPacket.h"
#include "bedrock_protocol/protocol/ClientboundDebugRendererPacket.h"
#include "bedrock_protocol/protocol/CommandBlockUpdatePacket.h"
#include "bedrock_protocol/protocol/EducationSettingsPacket.h"
#include "bedrock_protocol/protocol/FeatureRegistryPacket.h"
#include "bedrock_protocol/protocol/GraphicsOverrideParameterPacket.h"
#include "bedrock_protocol/protocol/LegacyTelemetryEventPacket.h"
#include "bedrock_protocol/protocol/LocatorBarPacket.h"
#include "bedrock_protocol/protocol/ServerPresenceInfoPacket.h"
#include "bedrock_protocol/protocol/ServerStoreInfoPacket.h"
#include "bedrock_protocol/protocol/ServerboundDataStorePacket.h"
#include "bedrock_protocol/protocol/TrimDataPacket.h"
#include "bedrock_protocol/protocol/UpdateClientOptionsPacket.h"
#include "bedrock_protocol/protocol/VoxelShapesPacket.h"
#include "bedrock_protocol/protocol/types/ChunkCacheBlob.h"
#include "bedrock_protocol/protocol/types/DebugMarkerData.h"
#include "bedrock_protocol/protocol/types/FeatureRegistryPacketEntry.h"
#include "bedrock_protocol/protocol/types/LocatorBarWaypoint.h"
#include "bedrock_protocol/protocol/types/LocatorBarWaypointPayload.h"
#include "bedrock_protocol/protocol/types/ParameterKeyframeValue.h"
#include "bedrock_protocol/protocol/types/PresenceInfo.h"
#include "bedrock_protocol/protocol/types/SerializableVoxelCells.h"
#include "bedrock_protocol/protocol/types/SerializableVoxelShape.h"
#include "bedrock_protocol/protocol/types/TrimMaterial.h"
#include "bedrock_protocol/protocol/types/TrimPattern.h"
#include "bedrock_protocol/protocol/types/WorldPosition.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistActorPriorityData.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistCategory.h"
#include "bedrock_protocol/protocol/types/camera/CameraAimAssistPreset.h"
#include "bedrock_protocol/protocol/types/camera/CameraProgressOption.h"
#include "bedrock_protocol/protocol/types/camera/CameraRotationOption.h"
#include "bedrock_protocol/protocol/types/camera/CameraSplineDefinition.h"
#include "bedrock_protocol/protocol/types/ddui/DataStoreUpdate.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {
namespace {

types::camera::CameraAimAssistCategoryPriority makeCategoryPriority(ValueWell &w)
{
    const auto identifier = w.str("priorityId");
    const auto priority = w.i32();
    return types::camera::CameraAimAssistCategoryPriority{identifier, priority};
}

types::camera::CameraAimAssistCategory makeAimAssistCategory(ValueWell &w)
{
    const auto name = w.str("categoryName");
    // Each of the four lists gets a DIFFERENT length: equal lengths would let two of the loops be
    // swapped without changing the byte count.
    std::vector<types::camera::CameraAimAssistCategoryPriority> entities;
    entities.push_back(makeCategoryPriority(w));
    entities.push_back(makeCategoryPriority(w));
    std::vector<types::camera::CameraAimAssistCategoryPriority> blocks;
    blocks.push_back(makeCategoryPriority(w));
    blocks.push_back(makeCategoryPriority(w));
    blocks.push_back(makeCategoryPriority(w));
    std::vector<types::camera::CameraAimAssistCategoryPriority> blockTags;
    blockTags.push_back(makeCategoryPriority(w));
    blockTags.push_back(makeCategoryPriority(w));
    std::vector<types::camera::CameraAimAssistCategoryPriority> entityTypeFamilies;
    entityTypeFamilies.push_back(makeCategoryPriority(w));
    entityTypeFamilies.push_back(makeCategoryPriority(w));
    entityTypeFamilies.push_back(makeCategoryPriority(w));
    const auto defaultEntityPriority = w.some(w.i32());
    const auto defaultBlockPriority = w.some(w.i32());

    types::camera::CameraAimAssistCategoryPriorities priorities{
        std::move(entities),   std::move(blocks),     std::move(blockTags), std::move(entityTypeFamilies),
        defaultEntityPriority, defaultBlockPriority};
    return types::camera::CameraAimAssistCategory{name, std::move(priorities)};
}

types::camera::CameraAimAssistPreset makeAimAssistPreset(ValueWell &w)
{
    const auto identifier = w.str("presetId");
    types::camera::CameraAimAssistPresetExclusionDefinition exclusionSettings{
        {w.str("exBlock0"), w.str("exBlock1")},
        {w.str("exEntity0"), w.str("exEntity1"), w.str("exEntity2")},
        {w.str("exBlockTag0"), w.str("exBlockTag1")},
        {w.str("exFamily0"), w.str("exFamily1"), w.str("exFamily2")}};
    const std::vector<std::string> liquidTargetingList{w.str("liquid0"), w.str("liquid1")};
    std::vector<types::camera::CameraAimAssistPresetItemSettings> itemSettings;
    itemSettings.push_back(types::camera::CameraAimAssistPresetItemSettings{w.str("itemId0"), w.str("itemCategory0")});
    itemSettings.push_back(types::camera::CameraAimAssistPresetItemSettings{w.str("itemId1"), w.str("itemCategory1")});
    const auto defaultItemSettings = w.some(w.str("defaultItemSettings"));
    const auto defaultHandSettings = w.some(w.str("defaultHandSettings"));

    return types::camera::CameraAimAssistPreset{identifier,
                                                std::move(exclusionSettings),
                                                liquidTargetingList,
                                                std::move(itemSettings),
                                                defaultItemSettings,
                                                defaultHandSettings};
}

types::camera::CameraSplineDefinition makeCameraSplineDefinition(ValueWell &w)
{
    const auto name = w.str("splineName");
    const auto totalTime = w.f32();
    const auto easeType = w.u8();
    const std::vector<math::Vector3> curve{w.vec3(), w.vec3(), w.vec3()};
    std::vector<types::camera::CameraProgressOption> progressKeyFrames;
    // The string overload of the ease type, because that is what CameraProgressOption::read produces:
    // the byte overload runs the value through a name table and would not survive the round trip for
    // an arbitrary drawn byte.
    progressKeyFrames.push_back(types::camera::CameraProgressOption{w.f32(), w.f32(), w.str("progressEase0")});
    progressKeyFrames.push_back(types::camera::CameraProgressOption{w.f32(), w.f32(), w.str("progressEase1")});
    std::vector<types::camera::CameraRotationOption> rotationOptions;
    rotationOptions.push_back(types::camera::CameraRotationOption{w.vec3(), w.f32(), w.str("rotationEase0")});
    rotationOptions.push_back(types::camera::CameraRotationOption{w.vec3(), w.f32(), w.str("rotationEase1")});
    const auto splineIdentifier = w.str("splineIdentifier");
    const auto loadFromJson = w.flag();

    types::camera::CameraSplineInstruction instruction{totalTime,
                                                       easeType,
                                                       curve,
                                                       std::move(progressKeyFrames),
                                                       std::move(rotationOptions),
                                                       splineIdentifier,
                                                       loadFromJson};
    return types::camera::CameraSplineDefinition{name, std::move(instruction)};
}

types::LocatorBarWaypointPayload makeLocatorBarWaypointPayload(ValueWell &w)
{
    const auto group = w.uuid();
    const auto updateFlag = w.u32();
    // Every optional engaged: LocatorBarWaypoint::write emits each through writeOptional
    // unconditionally, so a disengaged one would leave that field untested.
    const auto visible = w.some(true);
    const auto worldPosition = w.some(types::WorldPosition{w.vec3(), w.i32()});
    const auto texturePath = w.some(w.str("texturePath"));
    const auto iconSize = w.some(w.vec2());
    const auto color = w.some(makeColor(w));
    const auto clientPositionAuthority = w.some(true);
    const auto actorUniqueId = w.some(w.i64());
    const auto action = w.u8();

    types::LocatorBarWaypoint waypoint{updateFlag, visible, worldPosition,          texturePath,
                                       iconSize,   color,   clientPositionAuthority, actorUniqueId};
    return types::LocatorBarWaypointPayload{group, std::move(waypoint), action};
}

types::SerializableVoxelShape makeSerializableVoxelShape(ValueWell &w)
{
    const auto xSize = w.u8();
    const auto ySize = w.u8();
    const auto zSize = w.u8();
    // The storage length is written as its own varint and is independent of the three sizes, so a
    // short list is enough and keeps the packet small.
    const std::vector<std::uint8_t> storage{w.u8(), w.u8(), w.u8()};
    const std::vector<float> xCoordinates{w.f32(), w.f32()};
    const std::vector<float> yCoordinates{w.f32(), w.f32(), w.f32()};
    const std::vector<float> zCoordinates{w.f32(), w.f32()};

    types::SerializableVoxelCells cells{xSize, ySize, zSize, storage};
    return types::SerializableVoxelShape{std::move(cells), xCoordinates, yCoordinates, zCoordinates};
}

}  // namespace

BP_FILLER(CameraAimAssistActorPriorityPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::camera::CameraAimAssistActorPriorityData> priorityData;
    priorityData.push_back(types::camera::CameraAimAssistActorPriorityData{w.i32(), w.i32(), w.i32(), w.i32()});
    priorityData.push_back(types::camera::CameraAimAssistActorPriorityData{w.i32(), w.i32(), w.i32(), w.i32()});

    return std::make_unique<CameraAimAssistActorPriorityPacket>(
        CameraAimAssistActorPriorityPacket::create(std::move(priorityData)));
}

BP_FILLER(CameraAimAssistPresetsPacket, 3)
{
    auto &w = ctx.well;
    std::vector<types::camera::CameraAimAssistCategory> categories;
    categories.push_back(makeAimAssistCategory(w));
    categories.push_back(makeAimAssistCategory(w));
    std::vector<types::camera::CameraAimAssistPreset> presets;
    presets.push_back(makeAimAssistPreset(w));
    presets.push_back(makeAimAssistPreset(w));
    presets.push_back(makeAimAssistPreset(w));
    const auto operation = w.u8();

    return std::make_unique<CameraAimAssistPresetsPacket>(
        CameraAimAssistPresetsPacket::create(std::move(categories), std::move(presets), operation));
}

BP_FILLER(CameraSplinePacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::camera::CameraSplineDefinition> splines;
    splines.push_back(makeCameraSplineDefinition(w));
    splines.push_back(makeCameraSplineDefinition(w));

    return std::make_unique<CameraSplinePacket>(CameraSplinePacket::create(std::move(splines)));
}

BP_FILLER(ClientCacheMissResponsePacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::ChunkCacheBlob> blobs;
    blobs.push_back(types::ChunkCacheBlob{w.u64(), w.str("blob0")});
    blobs.push_back(types::ChunkCacheBlob{w.u64(), w.str("blob1")});

    return std::make_unique<ClientCacheMissResponsePacket>(
        ClientCacheMissResponsePacket::create(std::move(blobs)));
}

BP_FILLER(ClientCameraAimAssistPacket, 3)
{
    auto &w = ctx.well;
    const auto presetId = w.str("presetId");
    // Pinned: decode validates the byte through CameraAimAssistActionTypeFromPacket (0..1).
    const auto actionType = ValueWell::pin(types::camera::CameraAimAssistActionType::CLEAR);
    const auto allowAimAssist = w.flag();

    return std::make_unique<ClientCameraAimAssistPacket>(
        ClientCameraAimAssistPacket::create(presetId, actionType, allowAimAssist));
}

/*
 * NOCREATE: this packet has no generated create() (the PHP original has none either); its public
 * surface is clear() and addCube(). addCube is the branch that populates both fields - clear() leaves
 * `data` disengaged, so it would test only the type string.
 */
BP_FILLER_NOCREATE(ClientboundDebugRendererPacket, 2)
{
    auto &w = ctx.well;
    const auto text = w.str("markerText");
    const auto position = w.vec3();
    const auto color = makeColor(w);
    const auto durationMillis = w.u64();

    return std::make_unique<ClientboundDebugRendererPacket>(ClientboundDebugRendererPacket::addCube(
        types::DebugMarkerData{text, position, color, durationMillis}));
}

/*
 * NOCREATE: no generated create() (matching PHP), so the thirteen fields are assigned individually.
 * isBlock is pinned true - the block branch writes blockPosition, commandBlockMode, isRedstoneMode and
 * isConditional, four fields against the single minecartActorRuntimeId of the other branch.
 * minecartActorRuntimeId is therefore the one field this filler cannot reach.
 */
BP_FILLER_NOCREATE(CommandBlockUpdatePacket, 13)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<CommandBlockUpdatePacket>();
    packet->isBlock = ValueWell::pin(true);
    packet->blockPosition = makeBlockPosition(w);
    packet->commandBlockMode = w.u32();
    packet->isRedstoneMode = true;
    packet->isConditional = true;
    packet->minecartActorRuntimeId = w.u64();
    packet->command = w.str("command");
    packet->lastOutput = w.str("lastOutput");
    packet->name = w.str("name");
    packet->filteredName = w.str("filteredName");
    packet->shouldTrackOutput = true;
    packet->tickDelay = w.u32();
    packet->executeOnFirstTick = true;

    return packet;
}

BP_FILLER(FeatureRegistryPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::FeatureRegistryPacketEntry> entries;
    entries.push_back(types::FeatureRegistryPacketEntry{w.str("featureName0"), w.str("featureJson0")});
    entries.push_back(types::FeatureRegistryPacketEntry{w.str("featureName1"), w.str("featureJson1")});

    return std::make_unique<FeatureRegistryPacket>(FeatureRegistryPacket::create(std::move(entries)));
}

BP_FILLER(GraphicsOverrideParameterPacket, 7)
{
    auto &w = ctx.well;
    std::vector<types::ParameterKeyframeValue> values;
    values.push_back(types::ParameterKeyframeValue{w.f32(), w.vec3()});
    values.push_back(types::ParameterKeyframeValue{w.f32(), w.vec3()});
    const auto unknownFloat = w.some(w.f32());
    const auto unknownVector3 = w.some(w.vec3());
    const auto biomeIdentifier = w.str("biomeIdentifier");
    const auto playerIdentifier = w.some(w.str("playerIdentifier"));
    // Pinned: decode validates the byte through GraphicsOverrideParameterTypeFromPacket (0..48).
    const auto parameterType = ValueWell::pin(types::GraphicsOverrideParameterType::AMBIENT_ILLUMINANCE);
    const auto reset = w.flag();

    return std::make_unique<GraphicsOverrideParameterPacket>(GraphicsOverrideParameterPacket::create(
        std::move(values), unknownFloat, unknownVector3, biomeIdentifier, playerIdentifier, parameterType,
        reset));
}

/*
 * NOCREATE: no generated create() (matching PHP - the encode side carries a "nice confusing mess" TODO
 * upstream). All three fields are public and are assigned here.
 */
BP_FILLER_NOCREATE(LegacyTelemetryEventPacket, 3)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<LegacyTelemetryEventPacket>();
    packet->playerRuntimeId = w.u64();
    packet->eventData = w.i32();
    packet->type = w.u8();

    return packet;
}

BP_FILLER(LocatorBarPacket, 1)
{
    auto &w = ctx.well;
    std::vector<types::LocatorBarWaypointPayload> waypoints;
    waypoints.push_back(makeLocatorBarWaypointPayload(w));
    waypoints.push_back(makeLocatorBarWaypointPayload(w));

    return std::make_unique<LocatorBarPacket>(LocatorBarPacket::create(std::move(waypoints)));
}

BP_FILLER(ServerPresenceInfoPacket, 1)
{
    auto &w = ctx.well;
    const auto richPresenceId = w.some(w.str("richPresenceId"));
    const auto presenceConfig = w.some(types::PresenceInfo{richPresenceId});

    return std::make_unique<ServerPresenceInfoPacket>(
        ServerPresenceInfoPacket::create(presenceConfig));
}

BP_FILLER(ServerStoreInfoPacket, 1)
{
    auto &w = ctx.well;
    const auto storeId = w.str("storeId");
    const auto storeName = w.str("storeName");
    const auto clientStoreEntrypointConfig =
        w.some(types::ClientStoreEntrypointConfig{storeId, storeName});

    return std::make_unique<ServerStoreInfoPacket>(
        ServerStoreInfoPacket::create(clientStoreEntrypointConfig));
}

BP_FILLER(ServerboundDataStorePacket, 1)
{
    auto &w = ctx.well;
    // Engaged on purpose: encodePayload calls update.value() unconditionally and would throw on a
    // disengaged optional.
    const auto name = w.str("updateName");
    const auto property = w.str("updateProperty");
    const auto path = w.str("updatePath");
    auto data = makeDataStoreUpdateValue(w);
    const auto updateCount = w.u32();
    const auto pathUpdateCount = w.u32();
    std::optional<types::ddui::DataStoreUpdate> update{
        types::ddui::DataStoreUpdate{name, property, path, std::move(data), updateCount, pathUpdateCount}};

    return std::make_unique<ServerboundDataStorePacket>(
        ServerboundDataStorePacket::create(std::move(update)));
}

BP_FILLER(TrimDataPacket, 2)
{
    auto &w = ctx.well;
    std::vector<types::TrimPattern> trimPatterns;
    trimPatterns.push_back(types::TrimPattern{w.str("patternItemId0"), w.str("patternId0")});
    trimPatterns.push_back(types::TrimPattern{w.str("patternItemId1"), w.str("patternId1")});
    // Three, against the patterns' two: equal lengths would let the two loops be swapped unnoticed.
    std::vector<types::TrimMaterial> trimMaterials;
    trimMaterials.push_back(types::TrimMaterial{w.str("materialId0"), w.str("materialColor0"), w.str("materialItemId0")});
    trimMaterials.push_back(types::TrimMaterial{w.str("materialId1"), w.str("materialColor1"), w.str("materialItemId1")});
    trimMaterials.push_back(types::TrimMaterial{w.str("materialId2"), w.str("materialColor2"), w.str("materialItemId2")});

    return std::make_unique<TrimDataPacket>(
        TrimDataPacket::create(std::move(trimPatterns), std::move(trimMaterials)));
}

BP_FILLER(UpdateClientOptionsPacket, 2)
{
    auto &w = ctx.well;
    // Pinned: decode validates the byte through GraphicsModeFromPacket (0..3).
    const auto graphicsMode = w.some(ValueWell::pin(types::GraphicsMode::RAY_TRACED));
    const auto filterProfanityChange = w.some(true);

    return std::make_unique<UpdateClientOptionsPacket>(
        UpdateClientOptionsPacket::create(graphicsMode, filterProfanityChange));
}

BP_FILLER(EducationSettingsPacket, 10)
{
    auto &w = ctx.well;
    const auto codeBuilderDefaultUri = w.str("codeBuilderDefaultUri");
    const auto codeBuilderTitle = w.str("codeBuilderTitle");
    const auto canResizeCodeBuilder = w.flag();
    const auto disableLegacyTitleBar = w.flag();
    const auto postProcessFilter = w.str("postProcessFilter");
    const auto screenshotBorderResourcePath = w.str("screenshotBorderResourcePath");
    const auto agentCapabilities =
        w.some(types::EducationSettingsAgentCapabilities(w.some(w.flag())));
    const auto codeBuilderOverrideUri = w.some(w.str("codeBuilderOverrideUri"));
    const auto hasQuiz = w.flag();
    // Distinct, self-identifying strings on purpose: this pair is transposed by PocketMine's own
    // read(), and the port deliberately corrects it. If that correction is ever reverted, this
    // filler is what catches it.
    const auto url = w.str("linkUrl");
    const auto displayName = w.str("linkDisplayName");
    const auto linkSettings = w.some(types::EducationSettingsExternalLinkSettings(url, displayName));

    return std::make_unique<EducationSettingsPacket>(EducationSettingsPacket::create(
        codeBuilderDefaultUri, codeBuilderTitle, canResizeCodeBuilder, disableLegacyTitleBar,
        postProcessFilter, screenshotBorderResourcePath, agentCapabilities, codeBuilderOverrideUri, hasQuiz,
        linkSettings));
}

BP_FILLER(VoxelShapesPacket, 3)
{
    auto &w = ctx.well;
    std::vector<types::SerializableVoxelShape> shapes;
    shapes.push_back(makeSerializableVoxelShape(w));
    shapes.push_back(makeSerializableVoxelShape(w));
    const std::vector<std::pair<std::string, std::uint16_t>> nameMap{
        {w.str("shapeName0"), w.u16()}, {w.str("shapeName1"), w.u16()}, {w.str("shapeName2"), w.u16()}};
    const auto customShapeCount = w.u16();

    return std::make_unique<VoxelShapesPacket>(
        VoxelShapesPacket::create(std::move(shapes), nameMap, customShapeCount));
}

}  // namespace bedrock_protocol::verify
