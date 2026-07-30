/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SerializableVoxelShape.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/SerializableVoxelCells.h"

namespace bedrock_protocol::types {

class SerializableVoxelShape final {
public:
    SerializableVoxelShape(SerializableVoxelCells cells, std::vector<float> xCoordinates, std::vector<float> yCoordinates, std::vector<float> zCoordinates)
        : cells(std::move(cells)), xCoordinates(std::move(xCoordinates)), yCoordinates(std::move(yCoordinates)), zCoordinates(std::move(zCoordinates))
    {
    }

    [[nodiscard]] const SerializableVoxelCells &getCells() const { return cells; }

    [[nodiscard]] const std::vector<float> &getXCoordinates() const { return xCoordinates; }

    [[nodiscard]] const std::vector<float> &getYCoordinates() const { return yCoordinates; }

    [[nodiscard]] const std::vector<float> &getZCoordinates() const { return zCoordinates; }

    static SerializableVoxelShape read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    SerializableVoxelCells cells;
    std::vector<float> xCoordinates;
    std::vector<float> yCoordinates;
    std::vector<float> zCoordinates;
};

}  // namespace bedrock_protocol::types
