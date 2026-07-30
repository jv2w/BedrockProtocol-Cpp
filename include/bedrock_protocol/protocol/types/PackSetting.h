/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/PackSetting.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/PackSettingType.h"

namespace bedrock_protocol::types {

class PackSetting {
public:
    explicit PackSetting(std::string name) : name(std::move(name)) {}

    virtual ~PackSetting() = default;

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] virtual PackSettingType getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<PackSetting> clone() const = 0;

protected:
    PackSetting(const PackSetting &) = default;
    PackSetting &operator=(const PackSetting &) = default;

private:
    std::string name;
};

}  // namespace bedrock_protocol::types
