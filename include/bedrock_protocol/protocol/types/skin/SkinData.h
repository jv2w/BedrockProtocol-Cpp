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

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/PersonaSkinPiece.h"
#include "bedrock_protocol/protocol/types/skin/SkinAnimation.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

namespace bedrock_protocol::types::skin {

class SkinData {
public:
    static constexpr std::string_view ARM_SIZE_SLIM = "slim";
    static constexpr std::string_view ARM_SIZE_WIDE = "wide";

    /**
     * @param animations
     * @param personaPieces
     * @param pieceTintColors
     */
    SkinData(std::string skinId, std::string playFabId, std::string resourcePatch, SkinImage skinImage,
             std::vector<SkinAnimation> animations = {}, std::optional<SkinImage> capeImage = std::nullopt,
             std::string geometryData = "",
             std::string geometryDataEngineVersion = std::string(ProtocolInfo::MINECRAFT_VERSION_NETWORK),
             std::string animationData = "", std::string capeId = "",
             std::optional<std::string> fullSkinId = std::nullopt, std::string armSize = std::string(ARM_SIZE_WIDE),
             std::string skinColor = "", std::vector<PersonaSkinPiece> personaPieces = {},
             std::vector<PersonaPieceTintColor> pieceTintColors = {}, bool isVerified = true, bool premium = false,
             bool persona = false, bool personaCapeOnClassic = false, bool isPrimaryUser = true, bool override = true);

    [[nodiscard]] const std::string &getSkinId() const { return skinId; }

    [[nodiscard]] const std::string &getPlayFabId() const { return playFabId; }

    [[nodiscard]] const std::string &getResourcePatch() const { return resourcePatch; }

    [[nodiscard]] const SkinImage &getSkinImage() const { return skinImage; }

    [[nodiscard]] const std::vector<SkinAnimation> &getAnimations() const { return animations; }

    [[nodiscard]] const SkinImage &getCapeImage() const { return capeImage; }

    [[nodiscard]] const std::string &getGeometryData() const { return geometryData; }

    [[nodiscard]] const std::string &getGeometryDataEngineVersion() const { return geometryDataEngineVersion; }

    [[nodiscard]] const std::string &getAnimationData() const { return animationData; }

    [[nodiscard]] const std::string &getCapeId() const { return capeId; }

    [[nodiscard]] const std::string &getFullSkinId() const { return fullSkinId; }

    [[nodiscard]] const std::string &getArmSize() const { return armSize; }

    [[nodiscard]] const std::string &getSkinColor() const { return skinColor; }

    [[nodiscard]] const std::vector<PersonaSkinPiece> &getPersonaPieces() const { return personaPieces; }

    [[nodiscard]] const std::vector<PersonaPieceTintColor> &getPieceTintColors() const { return pieceTintColors; }

    [[nodiscard]] bool isPersona() const { return persona; }

    [[nodiscard]] bool isPremium() const { return premium; }

    [[nodiscard]] bool isPersonaCapeOnClassic() const { return personaCapeOnClassic; }

    [[nodiscard]] bool isPrimaryUser() const { return isPrimaryUser_; }

    [[nodiscard]] bool isOverride() const { return override_; }

    [[nodiscard]] bool isVerified() const { return isVerified_; }

    /**
     * @internal
     */
    void setVerified(bool verified) { isVerified_ = verified; }

private:
    std::string skinId;
    std::string playFabId;
    std::string resourcePatch;
    SkinImage skinImage;
    std::vector<SkinAnimation> animations;
    SkinImage capeImage;
    std::string geometryData;
    std::string geometryDataEngineVersion;
    std::string animationData;
    std::string capeId;
    std::string fullSkinId;
    std::string armSize;
    std::string skinColor;
    std::vector<PersonaSkinPiece> personaPieces;
    std::vector<PersonaPieceTintColor> pieceTintColors;
    /** PHP field name `isVerified`, renamed to avoid colliding with the getter of the same name. */
    bool isVerified_;
    bool premium;
    bool persona;
    bool personaCapeOnClassic;
    /** PHP field name `isPrimaryUser`, renamed to avoid colliding with the getter of the same name. */
    bool isPrimaryUser_;
    /** PHP field name `override`, renamed because `override` is a contextual keyword in C++. */
    bool override_;
};

}  // namespace bedrock_protocol::types::skin
