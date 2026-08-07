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

#include "bedrock_protocol/protocol/types/PlayerListEntry.h"

namespace bedrock_protocol::types {

PlayerListEntry PlayerListEntry::createRemovalEntry(const uuid::Uuid &uuid)
{
    PlayerListEntry entry;
    entry.actionType = ACTION_REMOVE;
    entry.uuid = uuid;

    return entry;
}

PlayerListEntry PlayerListEntry::createAdditionEntry(const uuid::Uuid &uuid, const std::int64_t actorUniqueId,
                                                     const std::string &username, const skin::SkinData &skinData,
                                                     const std::string &xboxUserId,
                                                     const std::string &platformChatId,
                                                     const std::int32_t buildPlatform, const bool isTeacher,
                                                     const bool isHost, const bool isSubClient,
                                                     const std::optional<color::Color> &color)
{
    PlayerListEntry entry;
    entry.actionType = ACTION_ADD;
    entry.uuid = uuid;
    entry.actorUniqueId = actorUniqueId;
    entry.username = username;
    entry.skinData = skinData;
    entry.xboxUserId = xboxUserId;
    entry.platformChatId = platformChatId;
    entry.buildPlatform = buildPlatform;
    entry.isTeacher = isTeacher;
    entry.isHost = isHost;
    entry.isSubClient = isSubClient;
    entry.color = color;

    return entry;
}

}  // namespace bedrock_protocol::types
