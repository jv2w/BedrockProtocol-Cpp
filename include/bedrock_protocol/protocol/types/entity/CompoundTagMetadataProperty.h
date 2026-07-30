/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/entity/CompoundTagMetadataProperty.php
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
#include "bedrock_protocol/nbt/tag/CompoundTag.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/CacheableNbt.h"
#include "bedrock_protocol/protocol/types/entity/EntityMetadataTypes.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

namespace bedrock_protocol::types::entity {

class CompoundTagMetadataProperty final : public MetadataProperty {
public:
    static constexpr std::int32_t ID = EntityMetadataTypes::COMPOUND_TAG;

    explicit CompoundTagMetadataProperty(const CacheableNbt<nbt::tag::CompoundTag> &value) : value(value) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] CacheableNbt<nbt::tag::CompoundTag> getValue() const { return value; }

    [[nodiscard]] bool equals(const MetadataProperty &other) const override
    {
        const auto *casted = dynamic_cast<const CompoundTagMetadataProperty *>(&other);
        return casted != nullptr && casted->value.getRoot().equals(value.getRoot());
    }

    /**
     * @throws PacketDecodeException
     */
    static CompoundTagMetadataProperty read(encoding::ByteBufferReader &in)
    {
        return CompoundTagMetadataProperty(
            CacheableNbt<nbt::tag::CompoundTag>(serializer::CommonTypes::getNbtCompoundRoot(in)));
    }

    void write(encoding::ByteBufferWriter &out) const override { out.writeByteArray(value.getEncodedNbt()); }

    [[nodiscard]] std::unique_ptr<MetadataProperty> clone() const override
    {
        return std::make_unique<CompoundTagMetadataProperty>(*this);
    }

private:
    CacheableNbt<nbt::tag::CompoundTag> value;
};

}  // namespace bedrock_protocol::types::entity
