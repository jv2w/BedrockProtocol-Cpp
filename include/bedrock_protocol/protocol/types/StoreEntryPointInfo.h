/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/StoreEntryPointInfo.php
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

class StoreEntryPointInfo final {
public:
    StoreEntryPointInfo(std::string id, std::string name) : id(std::move(id)), name(std::move(name)) {}

    [[nodiscard]] const std::string &getId() const { return id; }

    [[nodiscard]] const std::string &getName() const { return name; }

    static StoreEntryPointInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string id;
    std::string name;
};

}  // namespace bedrock_protocol::types
