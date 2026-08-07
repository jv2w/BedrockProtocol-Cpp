/*
 * This file is part of BedrockProtocol-Cpp.
 *
 * Fillers: world, chunk and level-event packets.
 *
 * Same two conventions as Fillers_Core.cpp, and for the same reasons: one create() call with every
 * argument spelled out, so a missing field is a wrong-arity error rather than a silent gap; and every
 * value drawn into a named local first, because C++ leaves argument evaluation order unspecified.
 *
 * ctx.well.pin(X) marks a value that must be LEGAL rather than unique - a discriminator the encoder
 * branches on. Every use names the branch it selects.
 *
 * UpdateBlockSyncedPacket is the one packet here without a generated create(): it inherits four
 * fields from UpdateBlockPacket and adds two, and the generator does not emit factories for derived
 * packets. It assigns its six fields individually.
 */

#include "bedrock_protocol/verify/Filler.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/AddVolumeEntityPacket.h"
#include "bedrock_protocol/protocol/BlockActorDataPacket.h"
#include "bedrock_protocol/protocol/BlockEventPacket.h"
#include "bedrock_protocol/protocol/ChunkRadiusUpdatedPacket.h"
#include "bedrock_protocol/protocol/DimensionDataPacket.h"
#include "bedrock_protocol/protocol/GameRulesChangedPacket.h"
#include "bedrock_protocol/protocol/LevelChunkPacket.h"
#include "bedrock_protocol/protocol/LevelEventGenericPacket.h"
#include "bedrock_protocol/protocol/LevelEventPacket.h"
#include "bedrock_protocol/protocol/NetworkChunkPublisherUpdatePacket.h"
#include "bedrock_protocol/protocol/RemoveVolumeEntityPacket.h"
#include "bedrock_protocol/protocol/RequestChunkRadiusPacket.h"
#include "bedrock_protocol/protocol/SetDifficultyPacket.h"
#include "bedrock_protocol/protocol/SetTimePacket.h"
#include "bedrock_protocol/protocol/SpawnParticleEffectPacket.h"
#include "bedrock_protocol/protocol/SubChunkPacket.h"
#include "bedrock_protocol/protocol/SubChunkRequestPacket.h"
#include "bedrock_protocol/protocol/UpdateBlockPacket.h"
#include "bedrock_protocol/protocol/UpdateBlockSyncedPacket.h"
#include "bedrock_protocol/protocol/UpdateSubChunkBlocksPacket.h"
#include "bedrock_protocol/protocol/types/DimensionData.h"
#include "bedrock_protocol/protocol/types/DimensionNameIds.h"
#include "bedrock_protocol/protocol/types/SubChunkPacketEntry.h"
#include "bedrock_protocol/protocol/types/SubChunkPacketHeightMapInfo.h"
#include "bedrock_protocol/protocol/types/SubChunkPositionOffset.h"
#include "bedrock_protocol/protocol/types/SubChunkRequestResult.h"
#include "bedrock_protocol/protocol/types/UpdateSubChunkBlocksPacketEntry.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

namespace {

/** A 256-entry heightmap. Every height is written verbatim; the type byte no longer suppresses any. */
types::SubChunkPacketHeightMapInfo makeHeightMap(ValueWell &w)
{
    const auto base = w.u8();
    std::vector<std::int8_t> heights;
    heights.reserve(256);
    for (int i = 0; i < 256; ++i) {
        heights.push_back(static_cast<std::int8_t>((base + i) & 0xf));
    }
    return types::SubChunkPacketHeightMapInfo(std::move(heights));
}

/** Offsets are three signed bytes on the wire, and the constructor rejects anything wider. */
types::SubChunkPositionOffset makeSubChunkPositionOffset(ValueWell &w)
{
    const auto xOffset = w.i8();
    const auto yOffset = w.i8();
    const auto zOffset = w.i8();
    return {xOffset, yOffset, zOffset};
}

types::SubChunkPacketEntry makeSubChunkPacketEntry(ValueWell &w)
{
    auto offset = makeSubChunkPositionOffset(w);
    const auto requestResult = ValueWell::pin(types::SubChunkRequestResult::SUCCESS);
    auto terrainData = w.some(w.str("terrainData"));
    // The height map types are free-standing bytes now - they no longer decide whether the map that
    // follows is on the wire, so they can be drawn rather than pinned to a branch-selecting value.
    const auto heightMapType = w.u8();
    auto heightMap = w.some(makeHeightMap(w));
    const auto renderHeightMapType = w.u8();
    auto renderHeightMap = w.some(makeHeightMap(w));
    auto usedBlobHash = w.some(w.u64());

    return {std::move(offset),   requestResult,          std::move(terrainData), heightMapType,
            std::move(heightMap), renderHeightMapType,   std::move(renderHeightMap), usedBlobHash};
}

types::UpdateSubChunkBlocksPacketEntry makeUpdateSubChunkBlocksEntry(ValueWell &w)
{
    auto blockPosition = makeBlockPosition(w);
    const auto blockRuntimeId = w.u32();
    const auto flags = w.u32();
    const auto syncedUpdateActorUniqueId = w.u64();
    const auto syncedUpdateType = w.u32();

    return {std::move(blockPosition), blockRuntimeId, flags, syncedUpdateActorUniqueId, syncedUpdateType};
}

types::DimensionData makeDimensionData(ValueWell &w)
{
    const auto maxHeight = w.i32();
    const auto minHeight = w.i32();
    const auto generator = w.i32();
    const auto dimensionType = w.i32();
    const auto packId = w.uuid();

    return {maxHeight, minHeight, generator, dimensionType, packId};
}

}  // namespace

BP_FILLER(UpdateBlockPacket, 4)
{
    auto &w = ctx.well;
    auto blockPosition = makeBlockPosition(w);
    const auto blockRuntimeId = w.u32();
    const auto flags = w.u32();
    const auto dataLayerId = w.u32();

    return std::make_unique<UpdateBlockPacket>(
        UpdateBlockPacket::create(std::move(blockPosition), blockRuntimeId, flags, dataLayerId));
}

BP_FILLER_NOCREATE(UpdateBlockSyncedPacket, 6)
{
    auto &w = ctx.well;
    auto packet = std::make_unique<UpdateBlockSyncedPacket>();
    packet->blockPosition = makeBlockPosition(w);
    packet->blockRuntimeId = w.u32();
    packet->flags = w.u32();
    packet->dataLayerId = w.u32();
    packet->actorUniqueId = w.u64();
    packet->updateType = w.u64();

    return packet;
}

BP_FILLER(LevelChunkPacket, 7)
{
    auto &w = ctx.well;
    auto chunkPosition = makeChunkPosition(w);
    const auto dimensionId = w.i32();
    // Pinned: decodePayload rejects anything above 64, so a drawn value would not survive its own
    // encoding. 64 is the largest legal one, so a narrowing of the field still shows.
    const auto subChunkCount = ValueWell::pin<std::uint32_t>(64);
    const auto subChunkLimit = w.some(w.i32());
    const auto cacheEnabled = w.flag();
    std::vector<std::uint64_t> usedBlobHashes = {w.u64(), w.u64(), w.u64()};
    auto extraPayload = w.str("extraPayload");

    return std::make_unique<LevelChunkPacket>(LevelChunkPacket::create(std::move(chunkPosition), dimensionId,
                                                                      subChunkCount, subChunkLimit, cacheEnabled,
                                                                      std::move(usedBlobHashes),
                                                                      std::move(extraPayload)));
}

BP_FILLER(SubChunkPacket, 4)
{
    auto &w = ctx.well;
    const auto cacheEnabled = w.flag();
    const auto dimension = w.i32();
    auto baseSubChunkPosition = makeSubChunkPosition(w);
    // Two entries, so a count read as a fixed LE32 instead of a varuint32 cannot line up by accident.
    std::vector<types::SubChunkPacketEntry> entries = {makeSubChunkPacketEntry(w), makeSubChunkPacketEntry(w)};

    return std::make_unique<SubChunkPacket>(
        SubChunkPacket::create(cacheEnabled, dimension, std::move(baseSubChunkPosition), std::move(entries)));
}

BP_FILLER(SubChunkRequestPacket, 3)
{
    auto &w = ctx.well;
    const auto dimension = w.i32();
    auto basePosition = makeSubChunkPosition(w);
    std::vector<types::SubChunkPositionOffset> entries = {makeSubChunkPositionOffset(w),
                                                          makeSubChunkPositionOffset(w),
                                                          makeSubChunkPositionOffset(w)};

    return std::make_unique<SubChunkRequestPacket>(
        SubChunkRequestPacket::create(dimension, std::move(basePosition), std::move(entries)));
}

BP_FILLER(UpdateSubChunkBlocksPacket, 3)
{
    auto &w = ctx.well;
    auto baseBlockPosition = makeBlockPosition(w);
    // Both layers populated: the two counts are written back to back, so a filler that left layer1
    // empty could not tell the two read loops apart.
    std::vector<types::UpdateSubChunkBlocksPacketEntry> layer0Updates = {makeUpdateSubChunkBlocksEntry(w),
                                                                         makeUpdateSubChunkBlocksEntry(w)};
    std::vector<types::UpdateSubChunkBlocksPacketEntry> layer1Updates = {makeUpdateSubChunkBlocksEntry(w),
                                                                         makeUpdateSubChunkBlocksEntry(w)};

    return std::make_unique<UpdateSubChunkBlocksPacket>(UpdateSubChunkBlocksPacket::create(
        std::move(baseBlockPosition), std::move(layer0Updates), std::move(layer1Updates)));
}

BP_FILLER(NetworkChunkPublisherUpdatePacket, 3)
{
    auto &w = ctx.well;
    auto blockPosition = makeBlockPosition(w);
    const auto radius = w.u32();
    std::vector<types::ChunkPosition> savedChunks = {makeChunkPosition(w), makeChunkPosition(w),
                                                     makeChunkPosition(w)};

    return std::make_unique<NetworkChunkPublisherUpdatePacket>(
        NetworkChunkPublisherUpdatePacket::create(std::move(blockPosition), radius, std::move(savedChunks)));
}

BP_FILLER(LevelEventPacket, 3)
{
    auto &w = ctx.well;
    const auto eventId = w.i32();
    const auto eventData = w.i32();
    // putVector3Nullable writes three floats either way, but decodePayload always produces an engaged
    // optional - so a disengaged one would not survive the round trip as itself.
    const auto position = w.some(w.vec3());

    return std::make_unique<LevelEventPacket>(LevelEventPacket::create(eventId, eventData, position));
}

BP_FILLER(LevelEventGenericPacket, 2)
{
    auto &w = ctx.well;
    const auto eventId = w.i32();
    // The reader is headless and hardcoded to TAG_Compound, so the tag must be a compound root.
    auto eventData = std::make_unique<nbt::tag::CompoundTag>(makeCompoundTag(w));

    return std::make_unique<LevelEventGenericPacket>(
        LevelEventGenericPacket::create(eventId, std::move(eventData)));
}

BP_FILLER(BlockEventPacket, 3)
{
    auto &w = ctx.well;
    auto blockPosition = makeBlockPosition(w);
    const auto eventType = w.i32();
    const auto eventData = w.i32();

    return std::make_unique<BlockEventPacket>(
        BlockEventPacket::create(std::move(blockPosition), eventType, eventData));
}

BP_FILLER(BlockActorDataPacket, 2)
{
    auto &w = ctx.well;
    auto blockPosition = makeBlockPosition(w);
    auto nbt = makeCacheableNbt(w);

    return std::make_unique<BlockActorDataPacket>(
        BlockActorDataPacket::create(std::move(blockPosition), std::move(nbt)));
}

BP_FILLER(SpawnParticleEffectPacket, 5)
{
    auto &w = ctx.well;
    const auto dimensionId = w.u8();
    const auto actorUniqueId = w.i64();
    const auto position = w.vec3();
    auto particleName = w.str("particleName");
    // Engaged: the JSON string is written only behind a presence bool.
    auto molangVariablesJson = w.some(w.str("molangVariablesJson"));

    return std::make_unique<SpawnParticleEffectPacket>(SpawnParticleEffectPacket::create(
        dimensionId, actorUniqueId, position, std::move(particleName), std::move(molangVariablesJson)));
}

BP_FILLER(AddVolumeEntityPacket, 8)
{
    auto &w = ctx.well;
    const auto entityNetId = w.u32();
    auto data = makeCacheableNbt(w);
    auto jsonIdentifier = w.str("jsonIdentifier");
    auto instanceName = w.str("instanceName");
    auto minBound = makeBlockPosition(w);
    auto maxBound = makeBlockPosition(w);
    const auto dimension = w.i32();
    auto engineVersion = w.str("engineVersion");

    return std::make_unique<AddVolumeEntityPacket>(AddVolumeEntityPacket::create(
        entityNetId, std::move(data), std::move(jsonIdentifier), std::move(instanceName), std::move(minBound),
        std::move(maxBound), dimension, std::move(engineVersion)));
}

BP_FILLER(RemoveVolumeEntityPacket, 2)
{
    auto &w = ctx.well;
    const auto entityNetId = w.u32();
    const auto dimension = w.i32();

    return std::make_unique<RemoveVolumeEntityPacket>(RemoveVolumeEntityPacket::create(entityNetId, dimension));
}

BP_FILLER(SetTimePacket, 1)
{
    auto &w = ctx.well;
    const auto time = w.i32();

    return std::make_unique<SetTimePacket>(SetTimePacket::create(time));
}

BP_FILLER(SetDifficultyPacket, 1)
{
    auto &w = ctx.well;
    const auto difficulty = w.u32();

    return std::make_unique<SetDifficultyPacket>(SetDifficultyPacket::create(difficulty));
}

BP_FILLER(GameRulesChangedPacket, 1)
{
    auto &w = ctx.well;
    auto gameRules = makeGameRules(w);

    return std::make_unique<GameRulesChangedPacket>(GameRulesChangedPacket::create(std::move(gameRules)));
}

BP_FILLER(ChunkRadiusUpdatedPacket, 1)
{
    auto &w = ctx.well;
    const auto radius = w.i32();

    return std::make_unique<ChunkRadiusUpdatedPacket>(ChunkRadiusUpdatedPacket::create(radius));
}

BP_FILLER(RequestChunkRadiusPacket, 2)
{
    auto &w = ctx.well;
    const auto radius = w.i32();
    const auto maxRadius = w.u8();

    return std::make_unique<RequestChunkRadiusPacket>(RequestChunkRadiusPacket::create(radius, maxRadius));
}

BP_FILLER(DimensionDataPacket, 1)
{
    auto &w = ctx.well;
    // The keys are pinned: decodePayload rejects anything outside these three name IDs, and rejects
    // repeats, so all three legal keys is both the maximum and the only shape available.
    std::vector<std::pair<std::string, types::DimensionData>> definitions;
    definitions.emplace_back(std::string(types::DimensionNameIds::OVERWORLD), makeDimensionData(w));
    definitions.emplace_back(std::string(types::DimensionNameIds::NETHER), makeDimensionData(w));
    definitions.emplace_back(std::string(types::DimensionNameIds::THE_END), makeDimensionData(w));

    return std::make_unique<DimensionDataPacket>(DimensionDataPacket::create(std::move(definitions)));
}

}  // namespace bedrock_protocol::verify
