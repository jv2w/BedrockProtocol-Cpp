/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/skin/SkinAnimation.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>

#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

namespace bedrock_protocol::types::skin {

class SkinAnimation {
public:
    static constexpr std::uint32_t TYPE_HEAD = 1;
    static constexpr std::uint32_t TYPE_BODY_32 = 2;
    static constexpr std::uint32_t TYPE_BODY_64 = 3;

    static constexpr std::uint32_t EXPRESSION_LINEAR = 0;  //???
    static constexpr std::uint32_t EXPRESSION_BLINKING = 1;

    SkinAnimation(SkinImage image, std::uint32_t type, float frames, std::uint32_t expressionType)
        : image(std::move(image)), type(type), frames(frames), expressionType(expressionType) {}

    /**
     * Image of the animation.
     */
    [[nodiscard]] const SkinImage &getImage() const { return image; }

    /**
     * The type of animation you are applying.
     */
    [[nodiscard]] std::uint32_t getType() const { return type; }

    /**
     * The total amount of frames in an animation.
     */
    [[nodiscard]] float getFrames() const { return frames; }

    [[nodiscard]] std::uint32_t getExpressionType() const { return expressionType; }

private:
    SkinImage image;
    std::uint32_t type;
    float frames;
    std::uint32_t expressionType;
};

}  // namespace bedrock_protocol::types::skin
