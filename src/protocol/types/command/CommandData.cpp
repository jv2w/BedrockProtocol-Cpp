/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/CommandData.h"

#include <utility>

#include "bedrock_protocol/protocol/types/command/CommandPermissions.h"

namespace bedrock_protocol::types::command {

CommandData::CommandData(std::string name, std::string description, std::uint16_t flags, std::int32_t permission,
                         std::optional<CommandHardEnum> aliases, std::vector<CommandOverload> overloads,
                         std::vector<ChainedSubCommandData> chainedSubCommandData)
    : name(std::move(name)), description(std::move(description)), flags(flags), aliases(std::move(aliases)),
      overloads(std::move(overloads)), chainedSubCommandData(std::move(chainedSubCommandData)),
      permission(CommandPermissions::toName(permission))
{
}

CommandData::CommandData(std::string name, std::string description, std::uint16_t flags, std::string permission,
                         std::optional<CommandHardEnum> aliases, std::vector<CommandOverload> overloads,
                         std::vector<ChainedSubCommandData> chainedSubCommandData)
    : name(std::move(name)), description(std::move(description)), flags(flags), aliases(std::move(aliases)),
      overloads(std::move(overloads)), chainedSubCommandData(std::move(chainedSubCommandData)),
      permission(std::move(permission))
{
}

}  // namespace bedrock_protocol::types::command
