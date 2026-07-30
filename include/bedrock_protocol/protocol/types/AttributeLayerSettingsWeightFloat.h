/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeLayerSettingsWeightFloat.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeight.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeightType.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeLayerSettings
 */
class AttributeLayerSettingsWeightFloat final : public AttributeLayerSettingsWeight {
public:
    static constexpr std::int32_t ID = AttributeLayerSettingsWeightType::FLOAT;

    explicit AttributeLayerSettingsWeightFloat(float value) : value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] std::unique_ptr<AttributeLayerSettingsWeight> clone() const override
    {
        return std::make_unique<AttributeLayerSettingsWeightFloat>(*this);
    }

    static AttributeLayerSettingsWeightFloat read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

private:
    float value;
};

}  // namespace bedrock_protocol::types
