/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/LevelSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/LevelSettings.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

LevelSettings LevelSettings::read(encoding::ByteBufferReader &in)
{
    //TODO: in the future we'll use promoted properties + named arguments for decoding, but for now we stick with
    //this shitty way to limit BC breaks (needs more R&D)
    LevelSettings result;
    result.internalRead(in);
    return result;
}

void LevelSettings::internalRead(encoding::ByteBufferReader &in)
{
    seed = LE::readUnsignedLong(in);
    spawnSettings = SpawnSettings::read(in);
    generator = VarInt::readSignedInt(in);
    worldGamemode = VarInt::readSignedInt(in);
    hardcore = CommonTypes::getBool(in);
    difficulty = VarInt::readSignedInt(in);
    spawnPosition = CommonTypes::getBlockPosition(in);
    hasAchievementsDisabled = CommonTypes::getBool(in);
    editorWorldType = VarInt::readSignedInt(in);
    createdInEditorMode = CommonTypes::getBool(in);
    exportedFromEditorMode = CommonTypes::getBool(in);
    time = VarInt::readSignedInt(in);
    eduEditionOffer = VarInt::readSignedInt(in);
    hasEduFeaturesEnabled = CommonTypes::getBool(in);
    eduProductUUID = CommonTypes::getString(in);
    rainLevel = LE::readFloat(in);
    lightningLevel = LE::readFloat(in);
    hasConfirmedPlatformLockedContent = CommonTypes::getBool(in);
    isMultiplayerGame = CommonTypes::getBool(in);
    hasLANBroadcast = CommonTypes::getBool(in);
    xboxLiveBroadcastMode = VarInt::readSignedInt(in);
    platformBroadcastMode = VarInt::readSignedInt(in);
    commandsEnabled = CommonTypes::getBool(in);
    isTexturePacksRequired = CommonTypes::getBool(in);
    gameRules = CommonTypes::getGameRules(in, true);
    experiments = Experiments::read(in);
    hasBonusChestEnabled = CommonTypes::getBool(in);
    hasStartWithMapEnabled = CommonTypes::getBool(in);
    defaultPlayerPermission = VarInt::readSignedInt(in);
    serverChunkTickRadius = LE::readSignedInt(in); //doesn't make sense for this to be signed, but that's what the spec says
    hasLockedBehaviorPack = CommonTypes::getBool(in);
    hasLockedResourcePack = CommonTypes::getBool(in);
    isFromLockedWorldTemplate = CommonTypes::getBool(in);
    useMsaGamertagsOnly = CommonTypes::getBool(in);
    isFromWorldTemplate = CommonTypes::getBool(in);
    isWorldTemplateOptionLocked = CommonTypes::getBool(in);
    onlySpawnV1Villagers = CommonTypes::getBool(in);
    disablePersona = CommonTypes::getBool(in);
    disableCustomSkins = CommonTypes::getBool(in);
    muteEmoteAnnouncements = CommonTypes::getBool(in);
    vanillaVersion = CommonTypes::getString(in);
    limitedWorldWidth = LE::readSignedInt(in); //doesn't make sense for this to be signed, but that's what the spec says
    limitedWorldLength = LE::readSignedInt(in); //same as above
    isNewNether = CommonTypes::getBool(in);
    eduSharedUriResource = EducationUriResource::read(in);
    experimentalGameplayOverride =
        CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) { return CommonTypes::getBool(in); });
    chatRestrictionLevel = Byte::readUnsigned(in);
    disablePlayerInteractions = CommonTypes::getBool(in);
    serverEditorConnectionPolicy = VarInt::readSignedInt(in);
    allowAnonymousBlockDropsInEditorWorlds = CommonTypes::getBool(in);
}

void LevelSettings::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedLong(out, seed);
    spawnSettings.write(out);
    VarInt::writeSignedInt(out, generator);
    VarInt::writeSignedInt(out, worldGamemode);
    CommonTypes::putBool(out, hardcore);
    VarInt::writeSignedInt(out, difficulty);
    CommonTypes::putBlockPosition(out, spawnPosition);
    CommonTypes::putBool(out, hasAchievementsDisabled);
    VarInt::writeSignedInt(out, editorWorldType);
    CommonTypes::putBool(out, createdInEditorMode);
    CommonTypes::putBool(out, exportedFromEditorMode);
    VarInt::writeSignedInt(out, time);
    VarInt::writeSignedInt(out, eduEditionOffer);
    CommonTypes::putBool(out, hasEduFeaturesEnabled);
    CommonTypes::putString(out, eduProductUUID);
    LE::writeFloat(out, rainLevel);
    LE::writeFloat(out, lightningLevel);
    CommonTypes::putBool(out, hasConfirmedPlatformLockedContent);
    CommonTypes::putBool(out, isMultiplayerGame);
    CommonTypes::putBool(out, hasLANBroadcast);
    VarInt::writeSignedInt(out, xboxLiveBroadcastMode);
    VarInt::writeSignedInt(out, platformBroadcastMode);
    CommonTypes::putBool(out, commandsEnabled);
    CommonTypes::putBool(out, isTexturePacksRequired);
    CommonTypes::putGameRules(out, gameRules, true);
    experiments.write(out);
    CommonTypes::putBool(out, hasBonusChestEnabled);
    CommonTypes::putBool(out, hasStartWithMapEnabled);
    VarInt::writeSignedInt(out, defaultPlayerPermission);
    LE::writeSignedInt(out, serverChunkTickRadius); //doesn't make sense for this to be signed, but that's what the spec says
    CommonTypes::putBool(out, hasLockedBehaviorPack);
    CommonTypes::putBool(out, hasLockedResourcePack);
    CommonTypes::putBool(out, isFromLockedWorldTemplate);
    CommonTypes::putBool(out, useMsaGamertagsOnly);
    CommonTypes::putBool(out, isFromWorldTemplate);
    CommonTypes::putBool(out, isWorldTemplateOptionLocked);
    CommonTypes::putBool(out, onlySpawnV1Villagers);
    CommonTypes::putBool(out, disablePersona);
    CommonTypes::putBool(out, disableCustomSkins);
    CommonTypes::putBool(out, muteEmoteAnnouncements);
    CommonTypes::putString(out, vanillaVersion);
    LE::writeSignedInt(out, limitedWorldWidth); //doesn't make sense for this to be signed, but that's what the spec says
    LE::writeSignedInt(out, limitedWorldLength); //same as above
    CommonTypes::putBool(out, isNewNether);
    (eduSharedUriResource.has_value() ? *eduSharedUriResource : EducationUriResource("", "")).write(out);
    CommonTypes::writeOptional(out, experimentalGameplayOverride,
                               [](encoding::ByteBufferWriter &out, const bool v) { CommonTypes::putBool(out, v); });
    Byte::writeUnsigned(out, chatRestrictionLevel);
    CommonTypes::putBool(out, disablePlayerInteractions);
    VarInt::writeSignedInt(out, serverEditorConnectionPolicy);
    CommonTypes::putBool(out, allowAnonymousBlockDropsInEditorWorlds);
}

}  // namespace bedrock_protocol::types
