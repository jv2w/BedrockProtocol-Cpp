/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SerializableVoxelCells.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class SerializableVoxelCells final {
public:
    SerializableVoxelCells(std::uint8_t xSize, std::uint8_t ySize, std::uint8_t zSize, std::vector<std::uint8_t> storage)
        : xSize(xSize), ySize(ySize), zSize(zSize), storage(std::move(storage))
    {
    }

    [[nodiscard]] std::uint8_t getXSize() const { return xSize; }

    [[nodiscard]] std::uint8_t getYSize() const { return ySize; }

    [[nodiscard]] std::uint8_t getZSize() const { return zSize; }

    [[nodiscard]] const std::vector<std::uint8_t> &getStorage() const { return storage; }

    static SerializableVoxelCells read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t xSize;
    std::uint8_t ySize;
    std::uint8_t zSize;
    std::vector<std::uint8_t> storage;
};

}  // namespace bedrock_protocol::types
