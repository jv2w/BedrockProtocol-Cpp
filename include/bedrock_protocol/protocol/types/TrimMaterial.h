/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/TrimMaterial.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class TrimMaterial final {
public:
    TrimMaterial(std::string materialId, std::string color, std::string itemId)
        : materialId(std::move(materialId)), color(std::move(color)), itemId(std::move(itemId))
    {
    }

    [[nodiscard]] const std::string &getMaterialId() const { return materialId; }

    [[nodiscard]] const std::string &getColor() const { return color; }

    [[nodiscard]] const std::string &getItemId() const { return itemId; }

    static TrimMaterial read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string materialId;
    std::string color;
    std::string itemId;
};

}  // namespace bedrock_protocol::types
