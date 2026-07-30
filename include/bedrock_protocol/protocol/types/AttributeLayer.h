/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeEnvironment.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettings.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeUpdateLayers
 */
class AttributeLayer final {
public:
    AttributeLayer(std::string name, std::optional<std::string> noiseName, std::uint32_t dimension,
                   AttributeLayerSettings settings, std::vector<AttributeEnvironment> attributes)
        : name(std::move(name)), noiseName(std::move(noiseName)), dimension(dimension), settings(std::move(settings)),
          attributes(std::move(attributes))
    {
    }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const std::optional<std::string> &getNoiseName() const { return noiseName; }

    [[nodiscard]] std::uint32_t getDimension() const { return dimension; }

    [[nodiscard]] const AttributeLayerSettings &getSettings() const { return settings; }

    [[nodiscard]] const std::vector<AttributeEnvironment> &getAttributes() const { return attributes; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeLayer read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::optional<std::string> noiseName;
    std::uint32_t dimension;
    AttributeLayerSettings settings;
    std::vector<AttributeEnvironment> attributes;
};

}  // namespace bedrock_protocol::types
