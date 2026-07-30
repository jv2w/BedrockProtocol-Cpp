/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/IntIdMetaItemDescriptor.php
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
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptorType.h"

namespace bedrock_protocol::types::recipe {

class IntIdMetaItemDescriptor final : public ItemDescriptor {
public:
    static constexpr std::uint8_t ID = ItemDescriptorType::INT_ID_META;

    /** @throws std::invalid_argument */
    IntIdMetaItemDescriptor(std::int16_t id, std::int16_t meta);

    [[nodiscard]] std::uint8_t getTypeId() const override { return ID; }

    [[nodiscard]] std::int16_t getId() const { return id; }

    [[nodiscard]] std::int16_t getMeta() const { return meta; }

    /** @throws DataDecodeException */
    static IntIdMetaItemDescriptor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<ItemDescriptor> clone() const override
    {
        return std::make_unique<IntIdMetaItemDescriptor>(*this);
    }

private:
    std::int16_t id;
    std::int16_t meta;
};

}  // namespace bedrock_protocol::types::recipe
