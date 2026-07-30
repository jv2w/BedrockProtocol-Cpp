/*
 * This file is part of BedrockProtocol for Endstone.
 *
 * Fillers: scalar and string-only packets. See Fillers_Core.cpp for the two conventions that hold
 * across every filler file (one create() call with every argument spelled out; values drawn into
 * named locals first, never inline).
 *
 * None of the packets here branch on a discriminator: every encodePayload writes every field
 * unconditionally, so no ValueWell::pin is needed.
 */

#include "bedrock_protocol/verify/Filler.h"

#include "bedrock_protocol/protocol/AddBehaviorTreePacket.h"
#include "bedrock_protocol/protocol/AgentActionEventPacket.h"
#include "bedrock_protocol/protocol/AgentAnimationPacket.h"
#include "bedrock_protocol/protocol/AnvilDamagePacket.h"
#include "bedrock_protocol/protocol/AutomationClientConnectPacket.h"
#include "bedrock_protocol/protocol/AwardAchievementPacket.h"
#include "bedrock_protocol/protocol/BlockPickRequestPacket.h"
#include "bedrock_protocol/protocol/CameraPacket.h"
#include "bedrock_protocol/protocol/ClientCacheBlobStatusPacket.h"
#include "bedrock_protocol/protocol/ClientCacheStatusPacket.h"
#include "bedrock_protocol/protocol/ClientboundUpdateSoundDataPacket.h"
#include "bedrock_protocol/protocol/CodeBuilderPacket.h"
#include "bedrock_protocol/protocol/CodeBuilderSourcePacket.h"
#include "bedrock_protocol/protocol/CreatePhotoPacket.h"
#include "bedrock_protocol/protocol/CurrentStructureFeaturePacket.h"
#include "bedrock_protocol/protocol/DeathInfoPacket.h"
#include "bedrock_protocol/protocol/DebugInfoPacket.h"
#include "bedrock_protocol/protocol/GameTestRequestPacket.h"
#include "bedrock_protocol/protocol/GameTestResultsPacket.h"
#include "bedrock_protocol/protocol/GuiDataPickItemPacket.h"
#include "bedrock_protocol/protocol/HurtArmorPacket.h"
#include "bedrock_protocol/protocol/LabTablePacket.h"
#include "bedrock_protocol/protocol/LecternUpdatePacket.h"
#include "bedrock_protocol/protocol/LessonProgressPacket.h"
#include "bedrock_protocol/verify/WellTypes.h"

namespace bedrock_protocol::verify {

BP_FILLER(AddBehaviorTreePacket, 1)
{
    auto &w = ctx.well;
    const auto behaviorTreeJson = w.str("behaviorTreeJson");

    return std::make_unique<AddBehaviorTreePacket>(AddBehaviorTreePacket::create(behaviorTreeJson));
}

BP_FILLER(AgentActionEventPacket, 3)
{
    auto &w = ctx.well;
    const auto requestId = w.str("requestId");
    const auto action = w.u32();
    const auto responseJson = w.str("responseJson");

    return std::make_unique<AgentActionEventPacket>(
        AgentActionEventPacket::create(requestId, action, responseJson));
}

BP_FILLER(AgentAnimationPacket, 2)
{
    auto &w = ctx.well;
    const auto animationType = w.u8();
    const auto actorRuntimeId = w.u64();

    return std::make_unique<AgentAnimationPacket>(
        AgentAnimationPacket::create(animationType, actorRuntimeId));
}

BP_FILLER(AnvilDamagePacket, 2)
{
    auto &w = ctx.well;
    const auto blockPosition = makeBlockPosition(w);
    const auto damageAmount = w.u8();

    return std::make_unique<AnvilDamagePacket>(AnvilDamagePacket::create(blockPosition, damageAmount));
}

BP_FILLER(AutomationClientConnectPacket, 1)
{
    auto &w = ctx.well;
    const auto serverUri = w.str("serverUri");

    return std::make_unique<AutomationClientConnectPacket>(
        AutomationClientConnectPacket::create(serverUri));
}

BP_FILLER(AwardAchievementPacket, 1)
{
    auto &w = ctx.well;
    const auto achievementId = w.i32();

    return std::make_unique<AwardAchievementPacket>(AwardAchievementPacket::create(achievementId));
}

BP_FILLER(BlockPickRequestPacket, 3)
{
    auto &w = ctx.well;
    const auto blockPosition = makeBlockPosition(w);
    const auto addUserData = w.flag();
    const auto hotbarSlot = w.u8();

    return std::make_unique<BlockPickRequestPacket>(
        BlockPickRequestPacket::create(blockPosition, addUserData, hotbarSlot));
}

BP_FILLER(CameraPacket, 2)
{
    auto &w = ctx.well;
    const auto cameraActorUniqueId = w.i64();
    const auto playerActorUniqueId = w.i64();

    return std::make_unique<CameraPacket>(
        CameraPacket::create(cameraActorUniqueId, playerActorUniqueId));
}

BP_FILLER(ClientCacheBlobStatusPacket, 2)
{
    auto &w = ctx.well;
    const std::vector<std::uint64_t> hitHashes{w.u64(), w.u64()};
    // Three, and a different count from hitHashes: the two lists are written in the reverse of
    // declaration order, so equal lengths would let a swap of the two loops go unnoticed.
    const std::vector<std::uint64_t> missHashes{w.u64(), w.u64(), w.u64()};

    return std::make_unique<ClientCacheBlobStatusPacket>(
        ClientCacheBlobStatusPacket::create(hitHashes, missHashes));
}

BP_FILLER(ClientCacheStatusPacket, 1)
{
    auto &w = ctx.well;
    // Pinned rather than drawn: the packet's only field is a bool, so a drawn `false` would encode
    // identically to the default-constructed packet and the anti-tautology stage would fail. true is
    // the only value that distinguishes a filled instance from an empty one.
    const auto enabled = ValueWell::pin(true);
    (void)w;

    return std::make_unique<ClientCacheStatusPacket>(ClientCacheStatusPacket::create(enabled));
}

BP_FILLER(ClientboundUpdateSoundDataPacket, 2)
{
    auto &w = ctx.well;
    const auto serverSoundHandle = w.u64();
    const auto soundEvent = w.str("soundEvent");

    return std::make_unique<ClientboundUpdateSoundDataPacket>(
        ClientboundUpdateSoundDataPacket::create(serverSoundHandle, soundEvent));
}

BP_FILLER(CodeBuilderPacket, 2)
{
    auto &w = ctx.well;
    const auto url = w.str("url");
    const auto openCodeBuilder = w.flag();

    return std::make_unique<CodeBuilderPacket>(CodeBuilderPacket::create(url, openCodeBuilder));
}

BP_FILLER(CodeBuilderSourcePacket, 3)
{
    auto &w = ctx.well;
    const auto operation = w.u8();
    const auto category = w.u8();
    const auto codeStatus = w.u8();

    return std::make_unique<CodeBuilderSourcePacket>(
        CodeBuilderSourcePacket::create(operation, category, codeStatus));
}

BP_FILLER(CreatePhotoPacket, 3)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto photoName = w.str("photoName");
    const auto photoItemName = w.str("photoItemName");

    return std::make_unique<CreatePhotoPacket>(
        CreatePhotoPacket::create(actorUniqueId, photoName, photoItemName));
}

BP_FILLER(CurrentStructureFeaturePacket, 1)
{
    auto &w = ctx.well;
    const auto currentStructureFeature = w.str("currentStructureFeature");

    return std::make_unique<CurrentStructureFeaturePacket>(
        CurrentStructureFeaturePacket::create(currentStructureFeature));
}

BP_FILLER(DeathInfoPacket, 2)
{
    auto &w = ctx.well;
    const auto messageTranslationKey = w.str("messageTranslationKey");
    const std::vector<std::string> messageParameters{w.str("param0"), w.str("param1"), w.str("param2")};

    return std::make_unique<DeathInfoPacket>(
        DeathInfoPacket::create(messageTranslationKey, messageParameters));
}

BP_FILLER(DebugInfoPacket, 2)
{
    auto &w = ctx.well;
    const auto actorUniqueId = w.i64();
    const auto data = w.str("data");

    return std::make_unique<DebugInfoPacket>(DebugInfoPacket::create(actorUniqueId, data));
}

BP_FILLER(GameTestRequestPacket, 7)
{
    auto &w = ctx.well;
    const auto maxTestsPerBatch = w.i32();
    const auto repeatCount = w.i32();
    const auto rotation = w.u8();
    const auto stopOnFailure = w.flag();
    const auto testPosition = makeBlockPosition(w);
    const auto testsPerRow = w.i32();
    const auto testName = w.str("testName");

    return std::make_unique<GameTestRequestPacket>(GameTestRequestPacket::create(
        maxTestsPerBatch, repeatCount, rotation, stopOnFailure, testPosition, testsPerRow, testName));
}

BP_FILLER(GameTestResultsPacket, 3)
{
    auto &w = ctx.well;
    const auto success = w.flag();
    const auto error = w.str("error");
    const auto testName = w.str("testName");

    return std::make_unique<GameTestResultsPacket>(
        GameTestResultsPacket::create(success, error, testName));
}

BP_FILLER(GuiDataPickItemPacket, 3)
{
    auto &w = ctx.well;
    const auto itemDescription = w.str("itemDescription");
    const auto itemEffects = w.str("itemEffects");
    const auto hotbarSlot = w.i32();

    return std::make_unique<GuiDataPickItemPacket>(
        GuiDataPickItemPacket::create(itemDescription, itemEffects, hotbarSlot));
}

BP_FILLER(HurtArmorPacket, 3)
{
    auto &w = ctx.well;
    const auto cause = w.i32();
    const auto health = w.i32();
    const auto armorSlotFlags = w.u64();

    return std::make_unique<HurtArmorPacket>(HurtArmorPacket::create(cause, health, armorSlotFlags));
}

BP_FILLER(LabTablePacket, 3)
{
    auto &w = ctx.well;
    const auto actionType = w.u8();
    const auto blockPosition = makeBlockPosition(w);
    const auto reactionType = w.u8();

    return std::make_unique<LabTablePacket>(
        LabTablePacket::create(actionType, blockPosition, reactionType));
}

BP_FILLER(LecternUpdatePacket, 3)
{
    auto &w = ctx.well;
    const auto page = w.u8();
    const auto totalPages = w.u8();
    const auto blockPosition = makeBlockPosition(w);

    return std::make_unique<LecternUpdatePacket>(
        LecternUpdatePacket::create(page, totalPages, blockPosition));
}

BP_FILLER(LessonProgressPacket, 3)
{
    auto &w = ctx.well;
    const auto action = w.i32();
    const auto score = w.i32();
    const auto activityId = w.str("activityId");

    return std::make_unique<LessonProgressPacket>(
        LessonProgressPacket::create(action, score, activityId));
}

}  // namespace bedrock_protocol::verify
