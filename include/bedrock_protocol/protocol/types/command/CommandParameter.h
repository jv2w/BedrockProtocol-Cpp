/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/CommandParameter.php
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
#include <variant>

#include "bedrock_protocol/protocol/types/command/CommandHardEnum.h"
#include "bedrock_protocol/protocol/types/command/CommandSoftEnum.h"

namespace bedrock_protocol::types::command {

class CommandParameter {
public:
    static constexpr std::uint8_t FLAG_FORCE_COLLAPSE_ENUM = 0x1;
    static constexpr std::uint8_t FLAG_HAS_ENUM_CONSTRAINT = 0x2;

    /**
     * @note PHP `CommandHardEnum|CommandSoftEnum|null` — `std::monostate` is the null case.
     */
    using EnumType = std::variant<std::monostate, CommandHardEnum, CommandSoftEnum>;

    std::string paramName;
    std::uint32_t paramType = 0;
    bool isOptional = false;
    std::uint8_t flags = 0; //shows enum name if 1, always zero except for in /gamerule command
    EnumType enum_ = std::monostate{};  // PHP name: $enum ("enum" is a C++ keyword)
    std::optional<std::string> postfix = std::nullopt;

    static CommandParameter standard(std::string name, std::uint32_t type, std::uint8_t flags = 0,
                                     bool optional = false);

    static CommandParameter postfixed(std::string name, std::string postfix, std::uint8_t flags = 0,
                                      bool optional = false);

    /** PHP name: enum() ("enum" is a C++ keyword, and `enum_` is taken by the field above) */
    static CommandParameter enumParam(std::string name, CommandHardEnum enumValue, std::uint8_t flags,
                                      bool optional = false);

    static CommandParameter softEnum(std::string name, CommandSoftEnum enumValue, std::uint8_t flags,
                                     bool optional = false);

    /**
     * @generate-create-func
     */
    static CommandParameter allFields(std::string paramName, std::uint32_t paramType, bool isOptional,
                                      std::uint8_t flags, EnumType enum_, std::optional<std::string> postfix);

private:
    static CommandParameter baseline(std::string name, std::uint32_t type, std::uint8_t flags, bool optional);
};

}  // namespace bedrock_protocol::types::command
