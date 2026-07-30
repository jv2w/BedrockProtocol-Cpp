/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeUpdateLayers.php
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
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeLayer.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncPayload.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSyncType.h"

namespace bedrock_protocol::types {

/**
 * @see ClientboundAttributeLayerSyncPacket
 */
class AttributeUpdateLayers final : public AttributeLayerSyncPayload {
public:
    static constexpr std::int32_t ID = AttributeLayerSyncType::UPDATE_LAYERS;

    explicit AttributeUpdateLayers(std::vector<AttributeLayer> layers) : layers(std::move(layers)) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::vector<AttributeLayer> &getLayers() const { return layers; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeUpdateLayers read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<AttributeLayerSyncPayload> clone() const override
    {
        return std::make_unique<AttributeUpdateLayers>(*this);
    }

private:
    std::vector<AttributeLayer> layers;
};

}  // namespace bedrock_protocol::types
