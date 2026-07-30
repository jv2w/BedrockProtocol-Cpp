/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeUpdateLayerSettings.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettings.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncPayload.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncType.h"

namespace bedrock_protocol::types {

/**
 * @see ClientboundAttributeLayerSyncPacket
 */
class AttributeUpdateLayerSettings final : public AttributeLayerSyncPayload {
public:
    static constexpr std::int32_t ID = AttributeLayerSyncType::UPDATE_LAYER_SETTINGS;

    AttributeUpdateLayerSettings(std::string name, std::uint32_t dimension, AttributeLayerSettings settings)
        : name(std::move(name)), dimension(dimension), settings(std::move(settings))
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] std::uint32_t getDimension() const { return dimension; }

    [[nodiscard]] const AttributeLayerSettings &getSettings() const { return settings; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeUpdateLayerSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<AttributeLayerSyncPayload> clone() const override
    {
        return std::make_unique<AttributeUpdateLayerSettings>(*this);
    }

private:
    std::string name;
    std::uint32_t dimension;
    AttributeLayerSettings settings;
};

}  // namespace bedrock_protocol::types
