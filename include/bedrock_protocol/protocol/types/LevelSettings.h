/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/LevelSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/types/BlockPosition.h"
#include "bedrock_protocol/protocol/types/ChatRestrictionLevel.h"
#include "bedrock_protocol/protocol/types/EditorWorldType.h"
#include "bedrock_protocol/protocol/types/EducationEditionOffer.h"
#include "bedrock_protocol/protocol/types/EducationUriResource.h"
#include "bedrock_protocol/protocol/types/Experiments.h"
#include "bedrock_protocol/protocol/types/GameRule.h"
#include "bedrock_protocol/protocol/types/GeneratorType.h"
#include "bedrock_protocol/protocol/types/MultiplayerGameVisibility.h"
#include "bedrock_protocol/protocol/types/PlayerPermissions.h"
#include "bedrock_protocol/protocol/types/SpawnSettings.h"

namespace bedrock_protocol::types {

class LevelSettings final {
public:
    std::uint64_t seed = 0;
    SpawnSettings spawnSettings;
    std::int32_t generator = GeneratorType::OVERWORLD;
    std::int32_t worldGamemode = 0;
    bool hardcore = false;
    std::int32_t difficulty = 0;
    BlockPosition spawnPosition;
    bool hasAchievementsDisabled = true;
    std::int32_t editorWorldType = EditorWorldType::NON_EDITOR;
    bool createdInEditorMode = false;
    bool exportedFromEditorMode = false;
    std::int32_t time = -1;
    std::int32_t eduEditionOffer = EducationEditionOffer::NONE;
    bool hasEduFeaturesEnabled = false;
    std::string eduProductUUID = "";
    float rainLevel = 0.0F;
    float lightningLevel = 0.0F;
    bool hasConfirmedPlatformLockedContent = false;
    bool isMultiplayerGame = true;
    bool hasLANBroadcast = true;
    std::int32_t xboxLiveBroadcastMode = MultiplayerGameVisibility::PUBLIC;
    std::int32_t platformBroadcastMode = MultiplayerGameVisibility::PUBLIC;
    bool commandsEnabled = false;
    bool isTexturePacksRequired = true;
    /**
     * @note keyed by game rule name; the encode order must match the decode order, so this is a list of pairs
     *       rather than a map
     */
    std::vector<std::pair<std::string, std::unique_ptr<GameRule>>> gameRules;
    Experiments experiments;
    bool hasBonusChestEnabled = false;
    bool hasStartWithMapEnabled = false;
    std::int32_t defaultPlayerPermission = PlayerPermissions::MEMBER; //TODO

    std::int32_t serverChunkTickRadius = 4; //TODO (leave as default for now)

    bool hasLockedBehaviorPack = false;
    bool hasLockedResourcePack = false;
    bool isFromLockedWorldTemplate = false;
    bool useMsaGamertagsOnly = false;
    bool isFromWorldTemplate = false;
    bool isWorldTemplateOptionLocked = false;
    bool onlySpawnV1Villagers = false;
    bool disablePersona = false;
    bool disableCustomSkins = false;
    bool muteEmoteAnnouncements = false;
    std::string vanillaVersion{ProtocolInfo::MINECRAFT_VERSION_NETWORK};
    std::int32_t limitedWorldWidth = 0;
    std::int32_t limitedWorldLength = 0;
    bool isNewNether = true;
    std::optional<EducationUriResource> eduSharedUriResource = std::nullopt;
    std::optional<bool> experimentalGameplayOverride = std::nullopt;
    std::uint8_t chatRestrictionLevel = ChatRestrictionLevel::NONE;
    bool disablePlayerInteractions = false;
    std::int32_t serverEditorConnectionPolicy = 0;
    bool allowAnonymousBlockDropsInEditorWorlds = false;

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static LevelSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    void internalRead(encoding::ByteBufferReader &in);
};

}  // namespace bedrock_protocol::types
