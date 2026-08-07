/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/StructureEditorData.php
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

#include "bedrock_protocol/protocol/types/StructureSettings.h"

namespace bedrock_protocol::types {

class StructureEditorData {
public:
    static constexpr std::int32_t TYPE_DATA = 0;
    static constexpr std::int32_t TYPE_SAVE = 1;
    static constexpr std::int32_t TYPE_LOAD = 2;
    static constexpr std::int32_t TYPE_CORNER = 3;
    static constexpr std::int32_t TYPE_INVALID = 4;
    static constexpr std::int32_t TYPE_EXPORT = 5;

    std::string structureName;
    std::string filteredStructureName;
    std::string structureDataField;
    bool includePlayers = false;
    bool showBoundingBox = false;
    std::int32_t structureBlockType = 0;
    StructureSettings structureSettings;
    //gophertunnel v1.58.0 minecraft/protocol/packet/structure_block_update.go:53,75 - a plain uint8.
    std::uint8_t structureRedstoneSaveMode = 0;
};

}  // namespace bedrock_protocol::types
