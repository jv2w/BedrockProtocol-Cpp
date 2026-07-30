/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/CommandPermissions.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/CommandPermissions.h"

#include <array>
#include <stdexcept>
#include <utility>

namespace bedrock_protocol::types::command {
namespace {

constexpr std::array<std::pair<std::int32_t, std::string_view>, 6> PERMISSION_NAMES{{
    {0, "any"},
    {1, "gamedirectors"},
    {2, "admin"},
    {3, "host"},
    {4, "owner"},
    {5, "internal"},
}};

}  // namespace

std::string CommandPermissions::toName(std::int32_t value)
{
    for (const auto &[rawValue, name] : PERMISSION_NAMES) {
        if (rawValue == value) {
            return std::string(name);
        }
    }
    throw std::invalid_argument("Invalid raw value \"" + std::to_string(value) + "\" for CommandPermission");
}

std::int32_t CommandPermissions::fromName(std::string_view name)
{
    for (const auto &[rawValue, permissionName] : PERMISSION_NAMES) {
        if (permissionName == name) {
            return rawValue;
        }
    }
    throw std::invalid_argument("Invalid raw value \"" + std::string(name) + "\" for CommandPermission");
}

}  // namespace bedrock_protocol::types::command
