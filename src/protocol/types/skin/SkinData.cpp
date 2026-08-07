/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/skin/SkinData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/skin/SkinData.h"

#include <utility>

#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::skin {

SkinData::SkinData(std::string skinId, std::string playFabId, std::string resourcePatch, SkinImage skinImage,
                   std::vector<SkinAnimation> animations, std::optional<SkinImage> capeImage, std::string geometryData,
                   std::string geometryDataEngineVersion, std::string animationData, std::string capeId,
                   std::optional<std::string> fullSkinId, std::uint8_t armSize, color::Color skinColor,
                   std::vector<PersonaSkinPiece> personaPieces, std::vector<PersonaPieceTintColor> pieceTintColors,
                   bool isVerified, bool premium, bool persona, bool personaCapeOnClassic, bool isPrimaryUser,
                   bool override, std::string profileHash)
    : skinId(std::move(skinId)), playFabId(std::move(playFabId)), resourcePatch(std::move(resourcePatch)),
      skinImage(std::move(skinImage)), animations(std::move(animations)),
      capeImage(capeImage.has_value() ? std::move(*capeImage) : SkinImage(0, 0, "")),
      geometryData(std::move(geometryData)), geometryDataEngineVersion(std::move(geometryDataEngineVersion)),
      animationData(std::move(animationData)), capeId(std::move(capeId)),
      //this has to be unique or the client will do stupid things
      fullSkinId(fullSkinId.has_value() ? std::move(*fullSkinId) : uuid::Uuid::uuid4().toString()),
      armSize(armSize), skinColor(skinColor), personaPieces(std::move(personaPieces)),
      pieceTintColors(std::move(pieceTintColors)), isVerified_(isVerified), premium(premium), persona(persona),
      personaCapeOnClassic(personaCapeOnClassic), isPrimaryUser_(isPrimaryUser), override_(override),
      profileHash(std::move(profileHash)) {}

}  // namespace bedrock_protocol::types::skin
