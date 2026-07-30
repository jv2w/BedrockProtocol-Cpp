/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/command/CommandParameter.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/command/CommandParameter.h"

#include <utility>

namespace bedrock_protocol::types::command {

CommandParameter CommandParameter::baseline(std::string name, std::uint32_t type, std::uint8_t flags, bool optional)
{
    CommandParameter result;
    result.paramName = std::move(name);
    result.paramType = type;
    result.flags = flags;
    result.isOptional = optional;
    return result;
}

CommandParameter CommandParameter::standard(std::string name, std::uint32_t type, std::uint8_t flags, bool optional)
{
    return baseline(std::move(name), type, flags, optional);
}

CommandParameter CommandParameter::postfixed(std::string name, std::string postfix, std::uint8_t flags, bool optional)
{
    CommandParameter result = baseline(std::move(name), 0, flags, optional);
    result.postfix = std::move(postfix);
    return result;
}

CommandParameter CommandParameter::enumParam(std::string name, CommandHardEnum enumValue, std::uint8_t flags,
                                             bool optional)
{
    CommandParameter result = baseline(std::move(name), 0, flags, optional);
    result.enum_ = std::move(enumValue);
    return result;
}

CommandParameter CommandParameter::softEnum(std::string name, CommandSoftEnum enumValue, std::uint8_t flags,
                                            bool optional)
{
    CommandParameter result = baseline(std::move(name), 0, flags, optional);
    result.enum_ = std::move(enumValue);
    return result;
}

CommandParameter CommandParameter::allFields(std::string paramName, std::uint32_t paramType, bool isOptional,
                                             std::uint8_t flags, EnumType enum_, std::optional<std::string> postfix)
{
    CommandParameter result;
    result.paramName = std::move(paramName);
    result.paramType = paramType;
    result.isOptional = isOptional;
    result.flags = flags;
    result.enum_ = std::move(enum_);
    result.postfix = std::move(postfix);
    return result;
}

}  // namespace bedrock_protocol::types::command
