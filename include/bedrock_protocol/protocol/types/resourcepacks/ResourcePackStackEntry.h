/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/resourcepacks/ResourcePackStackEntry.php
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

namespace bedrock_protocol::types::resourcepacks {

class ResourcePackStackEntry {
public:
    ResourcePackStackEntry(std::string packId, std::string version, std::string subPackName)
        : packId(std::move(packId)), version(std::move(version)), subPackName(std::move(subPackName))
    {
    }

    virtual ~ResourcePackStackEntry() = default;

    [[nodiscard]] const std::string &getPackId() const { return packId; }

    [[nodiscard]] const std::string &getVersion() const { return version; }

    [[nodiscard]] const std::string &getSubPackName() const { return subPackName; }

    void write(encoding::ByteBufferWriter &out) const;

    static ResourcePackStackEntry read(encoding::ByteBufferReader &in);

private:
    std::string packId;
    std::string version;
    std::string subPackName;
};

}  // namespace bedrock_protocol::types::resourcepacks
