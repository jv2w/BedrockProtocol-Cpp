/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/skin/SkinImage.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/skin/SkinImage.h"

#include <stdexcept>
#include <string>
#include <utility>

namespace bedrock_protocol::types::skin {

SkinImage::SkinImage(std::uint32_t height, std::uint32_t width, std::string data)
    : height(height), width(width), data(std::move(data)) {
    //the PHP original also rejects negative height/width; both are unsigned here (they come off the wire as
    //LE::readUnsignedInt), so that check cannot fire and is omitted.
    const std::uint64_t expected = static_cast<std::uint64_t>(height) * width * 4;
    const std::uint64_t actual = this->data.size();
    if (expected != actual) {
        throw std::invalid_argument("Data should be exactly " + std::to_string(expected) + " bytes, got " +
                                    std::to_string(actual) + " bytes");
    }
}

SkinImage SkinImage::fromLegacy(const std::string &data) {
    switch (data.size()) {
    case 64 * 32 * 4:
        return SkinImage(32, 64, data);
    case 64 * 64 * 4:
        return SkinImage(64, 64, data);
    case 128 * 128 * 4:
        return SkinImage(128, 128, data);
    }

    throw std::invalid_argument("Unknown size");
}

}  // namespace bedrock_protocol::types::skin
