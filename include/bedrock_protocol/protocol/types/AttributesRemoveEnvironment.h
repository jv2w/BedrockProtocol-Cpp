/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributesRemoveEnvironment.php
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
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncPayload.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncType.h"

namespace bedrock_protocol::types {

/**
 * @see ClientboundAttributeLayerSyncPacket
 */
class AttributesRemoveEnvironment final : public AttributeLayerSyncPayload {
public:
    static constexpr std::int32_t ID = AttributeLayerSyncType::REMOVE_ENVIRONMENT;

    AttributesRemoveEnvironment(std::string name, std::uint32_t dimension, std::vector<std::string> attributes)
        : name(std::move(name)), dimension(dimension), attributes(std::move(attributes))
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] std::uint32_t getDimension() const { return dimension; }

    [[nodiscard]] const std::vector<std::string> &getAttributes() const { return attributes; }

    static AttributesRemoveEnvironment read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] std::unique_ptr<AttributeLayerSyncPayload> clone() const override
    {
        return std::make_unique<AttributesRemoveEnvironment>(*this);
    }

private:
    std::string name;
    std::uint32_t dimension;
    std::vector<std::string> attributes;
};

}  // namespace bedrock_protocol::types
