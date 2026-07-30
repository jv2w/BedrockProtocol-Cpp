/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/clientdata/ClientData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "bedrock_protocol/protocol/types/login/clientdata/ClientDataAnimationFrame.h"
#include "bedrock_protocol/protocol/types/login/clientdata/ClientDataPersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/login/clientdata/ClientDataPersonaSkinPiece.h"

namespace bedrock_protocol::types::login::clientdata {

/**
 * Model class for LoginPacket JSON data for JsonMapper
 */
class ClientData final {
public:
    /**
     * @required
     */
    std::vector<ClientDataAnimationFrame> AnimatedImageData;

    /** @required */
    std::string ArmSize;

    /** @required */
    std::string CapeData;

    /** @required */
    std::string CapeId;

    /** @required */
    std::int64_t CapeImageHeight = 0;

    /** @required */
    std::int64_t CapeImageWidth = 0;

    /** @required */
    bool CapeOnClassicSkin = false;

    /** @required */
    std::int64_t ClientEditorConnectionIntent = 0;

    /** @required */
    bool ClientIsEditorCapable = false;

    /** @required */
    std::int64_t ClientRandomId = 0;

    /** @required */
    bool CompatibleWithClientSideChunkGen = false;

    /** @required */
    std::int64_t CurrentInputMode = 0;

    /** @required */
    std::int64_t DefaultInputMode = 0;

    /** @required */
    std::string DeviceId;

    /** @required */
    std::string DeviceModel;

    /** @required */
    std::int64_t DeviceOS = 0;

    /** @required */
    bool FilterProfanity = false;

    /** @required */
    std::string GameVersion;

    /** @required */
    std::int64_t GraphicsMode = 0;

    /** @required */
    std::int64_t GuiScale = 0;

    /** @required */
    std::string LanguageCode;

    /** @required */
    std::int64_t MaxViewDistance = 0;

    /** @required */
    std::int64_t MemoryTier = 0;

    std::optional<bool> OverrideSkin;

    std::string PartyId;
    bool IsPartyLeader = false;

    /**
     * @required
     */
    std::vector<ClientDataPersonaSkinPiece> PersonaPieces;

    /** @required */
    bool PersonaSkin = false;

    /**
     * @required
     */
    std::vector<ClientDataPersonaPieceTintColor> PieceTintColors;

    /** @required */
    std::string PlatformOfflineId;

    /** @required */
    std::string PlatformOnlineId;

    /** @required */
    std::int64_t PlatformType = 0;

    std::string PlatformUserId = "";  //xbox-only, apparently

    /** @required */
    bool PremiumSkin = false;

    /** @required */
    std::string SelfSignedId;

    /** @required */
    std::string ServerAddress;

    /** @required */
    std::string SkinAnimationData;

    /** @required */
    std::string SkinColor;

    /** @required */
    std::string SkinData;

    /** @required */
    std::string SkinGeometryData;

    /** @required */
    std::string SkinGeometryDataEngineVersion;

    /** @required */
    std::string SkinId;

    /** @required */
    std::int64_t SkinImageHeight = 0;

    /** @required */
    std::int64_t SkinImageWidth = 0;

    /** @required */
    std::string SkinResourcePatch;

    /** @required */
    std::string ThirdPartyName;

    /** @required */
    bool TrustedSkin = false;

    /** @required */
    std::int64_t UIProfile = 0;
};

}  // namespace bedrock_protocol::types::login::clientdata
