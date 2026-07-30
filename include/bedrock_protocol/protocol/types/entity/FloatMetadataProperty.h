/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/FloatMetadataProperty.php
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
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/types/entity/EntityMetadataTypes.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

namespace bedrock_protocol::types::entity {

class FloatMetadataProperty final : public MetadataProperty {
public:
    static constexpr std::int32_t ID = EntityMetadataTypes::FLOAT;

    explicit FloatMetadataProperty(float value) : value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] bool equals(const MetadataProperty &other) const override
    {
        const auto *casted = dynamic_cast<const FloatMetadataProperty *>(&other);
        return casted != nullptr && casted->value == value;
    }

    static FloatMetadataProperty read(encoding::ByteBufferReader &in)
    {
        return FloatMetadataProperty(encoding::LE::readFloat(in));
    }

    void write(encoding::ByteBufferWriter &out) const override { encoding::LE::writeFloat(out, value); }

    [[nodiscard]] std::unique_ptr<MetadataProperty> clone() const override
    {
        return std::make_unique<FloatMetadataProperty>(*this);
    }

private:
    float value;
};

}  // namespace bedrock_protocol::types::entity
