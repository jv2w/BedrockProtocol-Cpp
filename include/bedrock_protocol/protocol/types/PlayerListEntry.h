/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerListEntry.php
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

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/protocol/types/DeviceOS.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types {

class PlayerListEntry {
public:
    static constexpr std::uint8_t ACTION_ADD = 0;
    static constexpr std::uint8_t ACTION_REMOVE = 1;

    /**
     * The action is per entry as of 1.26.40 - the packet-level action byte is gone
     * (packet/player_list.go:23-25, player.go:96-97).
     */
    std::uint8_t actionType = ACTION_ADD;
    uuid::Uuid uuid;
    std::int64_t actorUniqueId = 0;
    std::string username;
    std::optional<skin::SkinData> skinData;
    std::string xboxUserId;
    std::string platformChatId = "";
    std::int32_t buildPlatform = DeviceOS::UNKNOWN;
    bool isTeacher = false;
    bool isHost = false;
    bool isSubClient = false;
    std::optional<color::Color> color = std::nullopt;

    static PlayerListEntry createRemovalEntry(const uuid::Uuid &uuid);

    static PlayerListEntry createAdditionEntry(const uuid::Uuid &uuid, std::int64_t actorUniqueId,
                                               const std::string &username, const skin::SkinData &skinData,
                                               const std::string &xboxUserId = "",
                                               const std::string &platformChatId = "",
                                               std::int32_t buildPlatform = -1, bool isTeacher = false,
                                               bool isHost = false, bool isSubClient = false,
                                               const std::optional<color::Color> &color = std::nullopt);
};

}  // namespace bedrock_protocol::types
