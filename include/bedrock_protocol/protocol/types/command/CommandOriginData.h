/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandOriginData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::command {

class CommandOriginData {
public:
    static constexpr std::string_view ORIGIN_PLAYER = "player";
    static constexpr std::string_view ORIGIN_BLOCK = "commandblock";
    static constexpr std::string_view ORIGIN_MINECART_BLOCK = "minecartcommandblock";
    static constexpr std::string_view ORIGIN_DEV_CONSOLE = "devconsole";
    static constexpr std::string_view ORIGIN_TEST = "test";
    static constexpr std::string_view ORIGIN_AUTOMATION_PLAYER = "automationplayer";
    static constexpr std::string_view ORIGIN_CLIENT_AUTOMATION = "clientautomation";
    static constexpr std::string_view ORIGIN_DEDICATED_SERVER = "dedicatedserver";
    static constexpr std::string_view ORIGIN_ENTITY = "entity";
    static constexpr std::string_view ORIGIN_VIRTUAL = "virtual";
    static constexpr std::string_view ORIGIN_GAME_ARGUMENT = "gameargument";
    static constexpr std::string_view ORIGIN_ENTITY_SERVER = "entityserver";
    static constexpr std::string_view ORIGIN_PRECOMPILED = "precompiled";
    static constexpr std::string_view ORIGIN_GAME_DIRECTOR_ENTITY_SERVER = "gamedirectorentityserver";
    static constexpr std::string_view ORIGIN_SCRIPTING = "scripting";
    static constexpr std::string_view ORIGIN_EXECUTE_CONTEXT = "executecontext";

    std::string type;
    uuid::Uuid uuid;
    std::string requestId;
    std::int64_t playerActorUniqueId = 0;
};

}  // namespace bedrock_protocol::types::command
