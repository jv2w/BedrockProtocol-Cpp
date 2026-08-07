/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/MultiRecipe.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string_view>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/uuid/Uuid.h"

namespace bedrock_protocol::types::recipe {

class MultiRecipe final {
public:
    static constexpr std::string_view TYPE_REPAIR_ITEM = "00000000-0000-0000-0000-000000000001";
    static constexpr std::string_view TYPE_MAP_EXTENDING = "D392B075-4BA1-40AE-8789-AF868D56F6CE";
    static constexpr std::string_view TYPE_MAP_EXTENDING_CARTOGRAPHY = "8B36268C-1829-483C-A0F1-993B7156A8F2";
    static constexpr std::string_view TYPE_MAP_CLONING = "85939755-BA10-4D9D-A4CC-EFB7A8E943C4";
    static constexpr std::string_view TYPE_MAP_CLONING_CARTOGRAPHY = "442D85ED-8272-4543-A6F1-418F90DED05D";
    static constexpr std::string_view TYPE_MAP_UPGRADING = "AECD2294-4B94-434B-8667-4499BB2C9327";
    static constexpr std::string_view TYPE_MAP_UPGRADING_CARTOGRAPHY = "98C84B38-1085-46BD-B1CE-DD38C159E6CC";
    static constexpr std::string_view TYPE_BOOK_CLONING = "D1CA6B84-338E-4F2F-9C6B-76CC8B4BD98D";
    static constexpr std::string_view TYPE_BANNER_DUPLICATE = "B5C5D105-75A2-4076-AF2B-923EA2BF4BF0";
    static constexpr std::string_view TYPE_BANNER_ADD_PATTERN = "D81AAEAF-E172-4440-9225-868DF030D27B";
    static constexpr std::string_view TYPE_FIREWORKS = "00000000-0000-0000-0000-000000000002";
    static constexpr std::string_view TYPE_MAP_LOCKING_CARTOGRAPHY = "602234E4-CAC1-4353-8BB7-B1EBFF70024B";

    MultiRecipe(uuid::Uuid recipeId, std::uint32_t recipeNetId) : recipeId(recipeId), recipeNetId(recipeNetId) {}

    [[nodiscard]] const uuid::Uuid &getRecipeId() const { return recipeId; }

    [[nodiscard]] std::uint32_t getRecipeNetId() const { return recipeNetId; }

    /** @throws DataDecodeException */
    static MultiRecipe decode(encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const;

private:
    uuid::Uuid recipeId;
    std::uint32_t recipeNetId;
};

}  // namespace bedrock_protocol::types::recipe
