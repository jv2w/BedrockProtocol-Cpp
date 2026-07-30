/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/StringIdMetaItemDescriptor.php
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
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptor.h"
#include "bedrock_protocol/protocol/types/recipe/ItemDescriptorType.h"

namespace bedrock_protocol::types::recipe {

class StringIdMetaItemDescriptor final : public ItemDescriptor {
public:
    static constexpr std::uint8_t ID = ItemDescriptorType::STRING_ID_META;

    /**
     * The PHP original rejects a negative meta here; meta is decoded as an unsigned short, so the C++
     * field type makes that state unrepresentable and the check is omitted.
     */
    StringIdMetaItemDescriptor(std::string id, std::uint16_t meta) : id(std::move(id)), meta(meta) {}

    [[nodiscard]] std::uint8_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getId() const { return id; }

    [[nodiscard]] std::uint16_t getMeta() const { return meta; }

    /** @throws DataDecodeException */
    static StringIdMetaItemDescriptor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<ItemDescriptor> clone() const override
    {
        return std::make_unique<StringIdMetaItemDescriptor>(*this);
    }

private:
    std::string id;
    std::uint16_t meta;
};

}  // namespace bedrock_protocol::types::recipe
