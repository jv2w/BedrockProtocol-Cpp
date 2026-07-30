/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/skin/PersonaPieceTintColor.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace bedrock_protocol::types::skin {

class PersonaPieceTintColor final {
public:
    static constexpr std::string_view PIECE_TYPE_PERSONA_EYES = "persona_eyes";
    static constexpr std::string_view PIECE_TYPE_PERSONA_HAIR = "persona_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_MOUTH = "persona_mouth";

    /**
     * @param colors
     */
    PersonaPieceTintColor(std::string pieceType, std::vector<std::string> colors)
        : pieceType(std::move(pieceType)), colors(std::move(colors)) {}

    [[nodiscard]] const std::string &getPieceType() const { return pieceType; }

    [[nodiscard]] const std::vector<std::string> &getColors() const { return colors; }

private:
    std::string pieceType;
    std::vector<std::string> colors;
};

}  // namespace bedrock_protocol::types::skin
