/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/MapImage.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/MapImage.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/Endian.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

using encoding::VarInt;

namespace {
/** Stands in for PHP's Binary::flipIntEndianness(), which the C++ port has no utils\Binary equivalent for. */
std::uint32_t flipIntEndianness(const std::uint32_t value)
{
    return encoding::detail::byteSwap<std::uint32_t>(value);
}
}  // namespace

MapImage::MapImage(std::vector<std::vector<color::Color>> pixels)
{
    std::optional<std::size_t> rowLength = std::nullopt;
    for (const auto &row : pixels) {
        if (!rowLength.has_value()) {
            rowLength = row.size();
        }
        else if (row.size() != *rowLength) {
            throw std::invalid_argument("All rows must have the same number of pixels");
        }
    }
    if (!rowLength.has_value()) {
        throw std::invalid_argument("No pixels provided");
    }
    if (*rowLength > static_cast<std::size_t>(MAX_WIDTH)) {
        throw std::invalid_argument("Image width must be at most " + std::to_string(MAX_WIDTH) + " pixels wide");
    }
    if (pixels.size() > static_cast<std::size_t>(MAX_HEIGHT)) {
        throw std::invalid_argument("Image height must be at most " + std::to_string(MAX_HEIGHT) + " pixels tall");
    }
    this->height = static_cast<std::int32_t>(pixels.size());
    this->width = static_cast<std::int32_t>(*rowLength);
    this->pixels = std::move(pixels);
}

void MapImage::encode(encoding::ByteBufferWriter &out) const
{
    if (!encodedPixelCache.has_value()) {
        encoding::ByteBufferWriter serializer;
        for (std::int32_t y = 0; y < height; ++y) {
            for (std::int32_t x = 0; x < width; ++x) {
                //if mojang had any sense this would just be a regular LE int
                VarInt::writeUnsignedInt(
                    serializer,
                    flipIntEndianness(pixels[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)].toRGBA()));
            }
        }
        encodedPixelCache = serializer.getData();
    }

    out.writeByteArray(*encodedPixelCache);
}

MapImage MapImage::decode(encoding::ByteBufferReader &in, const std::int32_t height, const std::int32_t width)
{
    if (width > MAX_WIDTH) {
        throw PacketDecodeException("Image width must be at most " + std::to_string(MAX_WIDTH) + " pixels wide");
    }
    if (height > MAX_HEIGHT) {
        throw PacketDecodeException("Image height must be at most " + std::to_string(MAX_HEIGHT) + " pixels tall");
    }
    std::vector<std::vector<color::Color>> pixels;

    for (std::int32_t y = 0; y < height; ++y) {
        std::vector<color::Color> row;
        for (std::int32_t x = 0; x < width; ++x) {
            row.push_back(color::Color::fromRGBA(flipIntEndianness(VarInt::readUnsignedInt(in))));
        }
        pixels.push_back(std::move(row));
    }

    return MapImage(std::move(pixels));
}

}  // namespace bedrock_protocol::types
