/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/clientdata/ClientDataToSkinDataHelper.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/login/clientdata/ClientDataToSkinDataHelper.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/PersonaSkinPiece.h"
#include "bedrock_protocol/protocol/types/skin/SkinAnimation.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::login::clientdata {

namespace {

/** Decoding table for standard base64; -1 = invalid, -2 = padding. */
constexpr std::array<std::int8_t, 256> makeBase64DecodeTable() {
    std::array<std::int8_t, 256> table{};
    for (std::size_t i = 0; i < table.size(); ++i) {
        table[i] = -1;
    }
    const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for (std::int8_t i = 0; i < 64; ++i) {
        table[static_cast<unsigned char>(alphabet[i])] = i;
    }
    table[static_cast<unsigned char>('=')] = -2;
    return table;
}

constexpr std::array<std::int8_t, 256> BASE64_DECODE_TABLE = makeBase64DecodeTable();

/**
 * Parses the hex colour string that login data carries in SkinColor and PieceTintColors. The reference
 * documents the format as '#rrggbb', including the hash (gophertunnel minecraft/protocol/login/data.go:205);
 * '#aarrggbb' is accepted as well because the wire form carries an alpha channel. Anything else yields a
 * fully transparent colour.
 */
color::Color parseHexColor(std::string_view hex) {
    if (!hex.empty() && hex.front() == '#') {
        hex.remove_prefix(1);
    }
    if (hex.size() != 6 && hex.size() != 8) {
        return {0, 0, 0, 0};
    }
    std::uint32_t value = 0;
    for (const char c : hex) {
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        }
        else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        }
        else {
            return {0, 0, 0, 0};
        }
        value = (value << 4) | static_cast<std::uint32_t>(digit);
    }
    return hex.size() == 6 ? color::Color::fromRGB(value) : color::Color::fromARGB(value);
}

skin::PersonaPieceTintColor::Colors parseTintColors(const std::vector<std::string> &colors) {
    skin::PersonaPieceTintColor::Colors result{};
    for (std::size_t i = 0; i < result.size(); ++i) {
        result[i] = i < colors.size() ? parseHexColor(colors[i]) : color::Color(0, 0, 0, 0);
    }
    return result;
}

/**
 * Maps the persona_* piece type names used in login data onto the v2168 wire enum
 * (gophertunnel minecraft/protocol/skin.go:183-213). The reference never performs this mapping itself - its
 * login-side PieceType stays a string - so the pairing of each name to its enum member is derived from the
 * enum member names.
 */
std::uint32_t personaPieceTypeFromLoginName(const std::string &pieceType) {
    static const std::vector<std::pair<std::string_view, std::uint32_t>> TABLE = {
        {"persona_skeleton", skin::PersonaSkinPiece::PIECE_TYPE_SKELETON},
        {"persona_body", skin::PersonaSkinPiece::PIECE_TYPE_BODY},
        {"persona_skin", skin::PersonaSkinPiece::PIECE_TYPE_SKIN},
        {"persona_bottom", skin::PersonaSkinPiece::PIECE_TYPE_BOTTOM},
        {"persona_feet", skin::PersonaSkinPiece::PIECE_TYPE_FEET},
        {"persona_dress", skin::PersonaSkinPiece::PIECE_TYPE_DRESS},
        {"persona_top", skin::PersonaSkinPiece::PIECE_TYPE_TOP},
        {"persona_high_pants", skin::PersonaSkinPiece::PIECE_TYPE_HIGH_PANTS},
        {"persona_hands", skin::PersonaSkinPiece::PIECE_TYPE_HANDS},
        {"persona_outerwear", skin::PersonaSkinPiece::PIECE_TYPE_OUTERWEAR},
        {"persona_facial_hair", skin::PersonaSkinPiece::PIECE_TYPE_FACIAL_HAIR},
        {"persona_mouth", skin::PersonaSkinPiece::PIECE_TYPE_MOUTH},
        {"persona_eyes", skin::PersonaSkinPiece::PIECE_TYPE_EYES},
        {"persona_hair", skin::PersonaSkinPiece::PIECE_TYPE_HAIR},
        {"persona_hood", skin::PersonaSkinPiece::PIECE_TYPE_HOOD},
        {"persona_back", skin::PersonaSkinPiece::PIECE_TYPE_BACK},
        {"persona_face_accessory", skin::PersonaSkinPiece::PIECE_TYPE_FACE_ACCESSORY},
        {"persona_head", skin::PersonaSkinPiece::PIECE_TYPE_HEAD},
        {"persona_legs", skin::PersonaSkinPiece::PIECE_TYPE_LEGS},
        {"persona_left_leg", skin::PersonaSkinPiece::PIECE_TYPE_LEFT_LEG},
        {"persona_right_leg", skin::PersonaSkinPiece::PIECE_TYPE_RIGHT_LEG},
        {"persona_arms", skin::PersonaSkinPiece::PIECE_TYPE_ARMS},
        {"persona_left_arm", skin::PersonaSkinPiece::PIECE_TYPE_LEFT_ARM},
        {"persona_right_arm", skin::PersonaSkinPiece::PIECE_TYPE_RIGHT_ARM},
        {"persona_capes", skin::PersonaSkinPiece::PIECE_TYPE_CAPES},
        {"persona_classic_skin", skin::PersonaSkinPiece::PIECE_TYPE_CLASSIC_SKIN},
        {"persona_emote", skin::PersonaSkinPiece::PIECE_TYPE_EMOTE},
        {"unsupported", skin::PersonaSkinPiece::PIECE_TYPE_UNSUPPORTED},
    };
    for (const auto &[name, value] : TABLE) {
        if (pieceType == name) {
            return value;
        }
    }
    return skin::PersonaSkinPiece::PIECE_TYPE_UNKNOWN;
}

uuid::Uuid parsePackId(const std::string &packId) {
    try {
        return uuid::Uuid::fromString(packId);
    }
    catch (const std::invalid_argument &) {
        //login data is client-supplied; a malformed pack ID must not abort the whole conversion
        return {};
    }
}

}  // namespace

/**
 * Equivalent of PHP base64_decode($base64, true) - returns nullopt instead of false on malformed input.
 */
std::optional<std::string> detail::strictBase64Decode(std::string_view base64) {
    //like PHP's strict mode: whitespace is skipped, unpadded input is accepted, anything else outside the
    //alphabet (or misplaced padding) fails
    std::vector<std::uint8_t> sextets;
    sextets.reserve(base64.size());
    std::size_t padding = 0;
    for (const char c : base64) {
        const auto uc = static_cast<unsigned char>(c);
        if (uc == ' ' || uc == '\t' || uc == '\n' || uc == '\r' || uc == '\v' || uc == '\f') {
            continue;
        }
        const std::int8_t value = BASE64_DECODE_TABLE[uc];
        if (value == -1) {
            return std::nullopt;
        }
        if (value == -2) {
            ++padding;
            continue;
        }
        if (padding != 0) {
            //data after padding
            return std::nullopt;
        }
        sextets.push_back(static_cast<std::uint8_t>(value));
    }
    if ((sextets.size() % 4) == 1) {
        return std::nullopt;
    }
    if (padding != 0) {
        if (padding > 2 || ((sextets.size() + padding) % 4) != 0) {
            return std::nullopt;
        }
    }

    std::string result;
    result.reserve((sextets.size() / 4) * 3 + 2);
    std::size_t i = 0;
    for (; i + 4 <= sextets.size(); i += 4) {
        const auto b0 = static_cast<std::uint32_t>(sextets[i]);
        const auto b1 = static_cast<std::uint32_t>(sextets[i + 1]);
        const auto b2 = static_cast<std::uint32_t>(sextets[i + 2]);
        const auto b3 = static_cast<std::uint32_t>(sextets[i + 3]);
        result.push_back(static_cast<char>(((b0 << 2) | (b1 >> 4)) & 0xff));
        result.push_back(static_cast<char>(((b1 << 4) | (b2 >> 2)) & 0xff));
        result.push_back(static_cast<char>(((b2 << 6) | b3) & 0xff));
    }
    const std::size_t remaining = sextets.size() - i;
    if (remaining >= 2) {
        const auto b0 = static_cast<std::uint32_t>(sextets[i]);
        const auto b1 = static_cast<std::uint32_t>(sextets[i + 1]);
        result.push_back(static_cast<char>(((b0 << 2) | (b1 >> 4)) & 0xff));
        if (remaining == 3) {
            const auto b2 = static_cast<std::uint32_t>(sextets[i + 2]);
            result.push_back(static_cast<char>(((b1 << 4) | (b2 >> 2)) & 0xff));
        }
    }
    return result;
}

std::string ClientDataToSkinDataHelper::safeB64Decode(std::string_view base64, std::string_view context) {
    const auto result = detail::strictBase64Decode(base64);
    if (!result.has_value()) {
        throw std::invalid_argument(std::string(context) + ": Malformed base64, cannot be decoded");
    }
    return result.value();
}

skin::SkinData ClientDataToSkinDataHelper::fromClientData(const ClientData &clientData) {
    std::vector<skin::SkinAnimation> animations;
    std::size_t k = 0;
    for (const auto &animation : clientData.AnimatedImageData) {
        animations.emplace_back(
            skin::SkinImage(static_cast<std::uint32_t>(animation.ImageHeight),
                            static_cast<std::uint32_t>(animation.ImageWidth),
                            safeB64Decode(animation.Image, "AnimatedImageData." + std::to_string(k) + ".Image")),
            static_cast<std::uint32_t>(animation.Type), animation.Frames,
            static_cast<std::uint32_t>(animation.AnimationExpression));
        ++k;
    }

    std::vector<skin::PersonaSkinPiece> personaPieces;
    for (const auto &piece : clientData.PersonaPieces) {
        personaPieces.emplace_back(piece.PieceId, personaPieceTypeFromLoginName(piece.PieceType),
                                   parsePackId(piece.PackId), piece.IsDefault, piece.ProductId);
    }
    std::vector<skin::PersonaPieceTintColor> pieceTintColors;
    for (const auto &tint : clientData.PieceTintColors) {
        pieceTintColors.emplace_back(tint.PieceType, parseTintColors(tint.Colors));
    }

    return skin::SkinData(
        clientData.SkinId, clientData.PlayFabId, safeB64Decode(clientData.SkinResourcePatch, "SkinResourcePatch"),
        skin::SkinImage(static_cast<std::uint32_t>(clientData.SkinImageHeight),
                        static_cast<std::uint32_t>(clientData.SkinImageWidth),
                        safeB64Decode(clientData.SkinData, "SkinData")),
        animations,
        skin::SkinImage(static_cast<std::uint32_t>(clientData.CapeImageHeight),
                        static_cast<std::uint32_t>(clientData.CapeImageWidth),
                        safeB64Decode(clientData.CapeData, "CapeData")),
        safeB64Decode(clientData.SkinGeometryData, "SkinGeometryData"),
        safeB64Decode(clientData.SkinGeometryDataEngineVersion,
                      "SkinGeometryDataEngineVersion"),  //yes, they actually base64'd the version!
        safeB64Decode(clientData.SkinAnimationData, "SkinAnimationData"), clientData.CapeId, std::nullopt,
        clientData.ArmSize == "slim" ? skin::SkinData::ARM_SIZE_SLIM : skin::SkinData::ARM_SIZE_WIDE,
        parseHexColor(clientData.SkinColor), personaPieces, pieceTintColors, true, clientData.PremiumSkin,
        clientData.PersonaSkin, clientData.CapeOnClassicSkin,
        true,  //assume this is true? there's no field for it ...
        clientData.OverrideSkin.value_or(true), clientData.ProfileHash);
}

}  // namespace bedrock_protocol::types::login::clientdata
