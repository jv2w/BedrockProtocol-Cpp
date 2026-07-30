/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/BoolPackSetting.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/PackSetting.h"
#include "bedrock_protocol/protocol/types/PackSettingType.h"

namespace bedrock_protocol::types {

class BoolPackSetting final : public PackSetting {
public:
    static constexpr PackSettingType ID = PackSettingType::BOOL;

    BoolPackSetting(std::string name, bool value) : PackSetting(std::move(name)), value(value) {}

    [[nodiscard]] bool getValue() const { return value; }

    [[nodiscard]] PackSettingType getTypeId() const override { return ID; }

    void write(encoding::ByteBufferWriter &out) const override;

    static BoolPackSetting read(encoding::ByteBufferReader &in, std::string name);

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] std::unique_ptr<PackSetting> clone() const override
    {
        return std::make_unique<BoolPackSetting>(*this);
    }

private:
    bool value;
};

}  // namespace bedrock_protocol::types
