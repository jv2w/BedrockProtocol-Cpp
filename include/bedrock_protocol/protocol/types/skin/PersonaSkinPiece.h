/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/skin/PersonaSkinPiece.php
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

#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::skin {

class PersonaSkinPiece final {
public:
    //piece type is a uint32 enum on the wire as of protocol 2168
    //(gophertunnel minecraft/protocol/skin.go:183-213,234)
    static constexpr std::uint32_t PIECE_TYPE_UNKNOWN = 0;
    static constexpr std::uint32_t PIECE_TYPE_SKELETON = 1;
    static constexpr std::uint32_t PIECE_TYPE_BODY = 2;
    static constexpr std::uint32_t PIECE_TYPE_SKIN = 3;
    static constexpr std::uint32_t PIECE_TYPE_BOTTOM = 4;
    static constexpr std::uint32_t PIECE_TYPE_FEET = 5;
    static constexpr std::uint32_t PIECE_TYPE_DRESS = 6;
    static constexpr std::uint32_t PIECE_TYPE_TOP = 7;
    static constexpr std::uint32_t PIECE_TYPE_HIGH_PANTS = 8;
    static constexpr std::uint32_t PIECE_TYPE_HANDS = 9;
    static constexpr std::uint32_t PIECE_TYPE_OUTERWEAR = 10;
    static constexpr std::uint32_t PIECE_TYPE_FACIAL_HAIR = 11;
    static constexpr std::uint32_t PIECE_TYPE_MOUTH = 12;
    static constexpr std::uint32_t PIECE_TYPE_EYES = 13;
    static constexpr std::uint32_t PIECE_TYPE_HAIR = 14;
    static constexpr std::uint32_t PIECE_TYPE_HOOD = 15;
    static constexpr std::uint32_t PIECE_TYPE_BACK = 16;
    static constexpr std::uint32_t PIECE_TYPE_FACE_ACCESSORY = 17;
    static constexpr std::uint32_t PIECE_TYPE_HEAD = 18;
    static constexpr std::uint32_t PIECE_TYPE_LEGS = 19;
    static constexpr std::uint32_t PIECE_TYPE_LEFT_LEG = 20;
    static constexpr std::uint32_t PIECE_TYPE_RIGHT_LEG = 21;
    static constexpr std::uint32_t PIECE_TYPE_ARMS = 22;
    static constexpr std::uint32_t PIECE_TYPE_LEFT_ARM = 23;
    static constexpr std::uint32_t PIECE_TYPE_RIGHT_ARM = 24;
    static constexpr std::uint32_t PIECE_TYPE_CAPES = 25;
    static constexpr std::uint32_t PIECE_TYPE_CLASSIC_SKIN = 26;
    static constexpr std::uint32_t PIECE_TYPE_EMOTE = 27;
    static constexpr std::uint32_t PIECE_TYPE_UNSUPPORTED = 28;

    PersonaSkinPiece(std::string pieceId, std::uint32_t pieceType, uuid::Uuid packId, bool isDefaultPiece,
                     std::string productId)
        : pieceId(std::move(pieceId)), pieceType(pieceType), packId(packId), isDefaultPiece_(isDefaultPiece),
          productId(std::move(productId)) {}

    [[nodiscard]] const std::string &getPieceId() const { return pieceId; }

    [[nodiscard]] std::uint32_t getPieceType() const { return pieceType; }

    [[nodiscard]] const uuid::Uuid &getPackId() const { return packId; }

    [[nodiscard]] bool isDefaultPiece() const { return isDefaultPiece_; }

    [[nodiscard]] const std::string &getProductId() const { return productId; }

private:
    std::string pieceId;
    std::uint32_t pieceType;
    uuid::Uuid packId;
    /** PHP field name `isDefaultPiece`, renamed to avoid colliding with the getter of the same name. */
    bool isDefaultPiece_;
    std::string productId;
};

}  // namespace bedrock_protocol::types::skin
