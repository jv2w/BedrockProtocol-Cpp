/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/command/ConstrainedEnumValue.php
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
#include <utility>
#include <vector>

namespace bedrock_protocol::types::command {

class ConstrainedEnumValue final {
public:
    static constexpr std::uint8_t REQUIRES_CHEATS_ENABLED = 1 << 0;
    static constexpr std::uint8_t REQUIRES_ELEVATED_PERMISSIONS = 1 << 1;
    static constexpr std::uint8_t REQUIRES_HOST_PERMISSIONS = 1 << 2;
    static constexpr std::uint8_t REQUIRES_ALLOW_ALIASES = 1 << 3;

    /**
     * @param constraints
     */
    ConstrainedEnumValue(std::string value, std::vector<std::uint8_t> constraints)
        : value(std::move(value)), constraints(std::move(constraints))
    {
    }

    [[nodiscard]] const std::string &getValue() const
    {
        return value;
    }

    [[nodiscard]] const std::vector<std::uint8_t> &getConstraints() const
    {
        return constraints;
    }

private:
    std::string value;
    std::vector<std::uint8_t> constraints;
};

}  // namespace bedrock_protocol::types::command
