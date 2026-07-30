/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/Vec3MetadataProperty.php
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
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/entity/EntityMetadataTypes.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

namespace bedrock_protocol::types::entity {

class Vec3MetadataProperty : public MetadataProperty {
public:
    static constexpr std::int32_t ID = EntityMetadataTypes::VECTOR3F;

    explicit Vec3MetadataProperty(const math::Vector3 &value) : value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] math::Vector3 getValue() const { return value; }

    static Vec3MetadataProperty read(encoding::ByteBufferReader &in)
    {
        return Vec3MetadataProperty(serializer::CommonTypes::getVector3(in));
    }

    void write(encoding::ByteBufferWriter &out) const override { serializer::CommonTypes::putVector3(out, value); }

    [[nodiscard]] bool equals(const MetadataProperty &other) const override
    {
        const auto *casted = dynamic_cast<const Vec3MetadataProperty *>(&other);
        return casted != nullptr && casted->value.equals(value);
    }

    [[nodiscard]] std::unique_ptr<MetadataProperty> clone() const override
    {
        return std::make_unique<Vec3MetadataProperty>(*this);
    }

private:
    math::Vector3 value;
};

}  // namespace bedrock_protocol::types::entity
