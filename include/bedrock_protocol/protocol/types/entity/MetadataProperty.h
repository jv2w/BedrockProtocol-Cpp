/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/MetadataProperty.php
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

namespace bedrock_protocol::types::entity {

class MetadataProperty {
public:
    virtual ~MetadataProperty() = default;

    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    [[nodiscard]] virtual bool equals(const MetadataProperty &other) const = 0;

    /** Not present in the PHP original; needed to preserve value semantics for owning containers. */
    [[nodiscard]] virtual std::unique_ptr<MetadataProperty> clone() const = 0;

    /**
     * Not present in the PHP original; this is the dispatch of CommonTypes::readMetadataProperty(),
     * moved here because the polymorphic hierarchy owns its own discriminator switch in this port.
     *
     * @throws PacketDecodeException
     */
    static std::unique_ptr<MetadataProperty> read(encoding::ByteBufferReader &in, std::int32_t type);

protected:
    MetadataProperty() = default;
    MetadataProperty(const MetadataProperty &) = default;
    MetadataProperty &operator=(const MetadataProperty &) = default;
};

}  // namespace bedrock_protocol::types::entity
