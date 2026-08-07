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

#include <array>
#include <string>
#include <string_view>
#include <utility>

#include "bedrock_protocol/color/Color.h"

namespace bedrock_protocol::types::skin {

class PersonaPieceTintColor final {
public:
    static constexpr std::string_view PIECE_TYPE_PERSONA_EYES = "persona_eyes";
    static constexpr std::string_view PIECE_TYPE_PERSONA_HAIR = "persona_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_MOUTH = "persona_mouth";

    /** Exactly four tint colours are on the wire, without a count prefix
     *  (gophertunnel minecraft/protocol/skin.go:246,256-258). */
    using Colors = std::array<color::Color, 4>;

    PersonaPieceTintColor(std::string pieceType, Colors colors)
        : pieceType(std::move(pieceType)), colors(colors) {}

    [[nodiscard]] const std::string &getPieceType() const { return pieceType; }

    [[nodiscard]] const Colors &getColors() const { return colors; }

private:
    std::string pieceType;
    Colors colors;
};

}  // namespace bedrock_protocol::types::skin
