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
#include <vector>

#include "bedrock_protocol/protocol/types/skin/PersonaPieceTintColor.h"
#include "bedrock_protocol/protocol/types/skin/PersonaSkinPiece.h"
#include "bedrock_protocol/protocol/types/skin/SkinAnimation.h"
#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

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
 * Equivalent of PHP base64_decode($base64, true) - returns nullopt instead of false on malformed input.
 */
std::optional<std::string> strictBase64Decode(std::string_view base64) {
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

}  // namespace

std::string ClientDataToSkinDataHelper::safeB64Decode(std::string_view base64, std::string_view context) {
    const auto result = strictBase64Decode(base64);
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
        personaPieces.emplace_back(piece.PieceId, piece.PieceType, piece.PackId, piece.IsDefault, piece.ProductId);
    }
    std::vector<skin::PersonaPieceTintColor> pieceTintColors;
    for (const auto &tint : clientData.PieceTintColors) {
        pieceTintColors.emplace_back(tint.PieceType, tint.Colors);
    }

    return skin::SkinData(
        clientData.SkinId, "", safeB64Decode(clientData.SkinResourcePatch, "SkinResourcePatch"),
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
        clientData.ArmSize, clientData.SkinColor, personaPieces, pieceTintColors, true, clientData.PremiumSkin,
        clientData.PersonaSkin, clientData.CapeOnClassicSkin,
        true,  //assume this is true? there's no field for it ...
        clientData.OverrideSkin.value_or(true));
}

}  // namespace bedrock_protocol::types::login::clientdata
