/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/FloatPackSetting.php
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
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/types/PackSetting.h"
#include "bedrock_protocol/protocol/types/PackSettingType.h"

namespace bedrock_protocol::types {

class FloatPackSetting final : public PackSetting {
public:
    static constexpr PackSettingType ID = PackSettingType::FLOAT;

    FloatPackSetting(std::string name, float value) : PackSetting(std::move(name)), value(value) {}

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] PackSettingType getTypeId() const override { return ID; }

    void write(encoding::ByteBufferWriter &out) const override { encoding::LE::writeFloat(out, value); }

    /** @throws DataDecodeException */
    static FloatPackSetting read(encoding::ByteBufferReader &in, std::string name)
    {
        return FloatPackSetting(std::move(name), encoding::LE::readFloat(in));
    }

    [[nodiscard]] std::unique_ptr<PackSetting> clone() const override
    {
        return std::make_unique<FloatPackSetting>(*this);
    }

private:
    float value;
};

}  // namespace bedrock_protocol::types
