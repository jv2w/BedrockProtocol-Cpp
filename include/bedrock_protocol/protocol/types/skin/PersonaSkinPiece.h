/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/skin/PersonaSkinPiece.php
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

namespace bedrock_protocol::types::skin {

class PersonaSkinPiece final {
public:
    static constexpr std::string_view PIECE_TYPE_PERSONA_BODY = "persona_body";
    static constexpr std::string_view PIECE_TYPE_PERSONA_BOTTOM = "persona_bottom";
    static constexpr std::string_view PIECE_TYPE_PERSONA_EYES = "persona_eyes";
    static constexpr std::string_view PIECE_TYPE_PERSONA_FACIAL_HAIR = "persona_facial_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_FEET = "persona_feet";
    static constexpr std::string_view PIECE_TYPE_PERSONA_HAIR = "persona_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_MOUTH = "persona_mouth";
    static constexpr std::string_view PIECE_TYPE_PERSONA_SKELETON = "persona_skeleton";
    static constexpr std::string_view PIECE_TYPE_PERSONA_SKIN = "persona_skin";
    static constexpr std::string_view PIECE_TYPE_PERSONA_TOP = "persona_top";

    PersonaSkinPiece(std::string pieceId, std::string pieceType, std::string packId, bool isDefaultPiece,
                     std::string productId)
        : pieceId(std::move(pieceId)), pieceType(std::move(pieceType)), packId(std::move(packId)),
          isDefaultPiece_(isDefaultPiece), productId(std::move(productId)) {}

    [[nodiscard]] const std::string &getPieceId() const { return pieceId; }

    [[nodiscard]] const std::string &getPieceType() const { return pieceType; }

    [[nodiscard]] const std::string &getPackId() const { return packId; }

    [[nodiscard]] bool isDefaultPiece() const { return isDefaultPiece_; }

    [[nodiscard]] const std::string &getProductId() const { return productId; }

private:
    std::string pieceId;
    std::string pieceType;
    std::string packId;
    /** PHP field name `isDefaultPiece`, renamed to avoid colliding with the getter of the same name. */
    bool isDefaultPiece_;
    std::string productId;
};

}  // namespace bedrock_protocol::types::skin
