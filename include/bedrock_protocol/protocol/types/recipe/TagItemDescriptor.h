/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/recipe/TagItemDescriptor.php
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

class TagItemDescriptor final : public ItemDescriptor {
public:
    static constexpr std::uint8_t ID = ItemDescriptorType::TAG;

    explicit TagItemDescriptor(std::string tag) : tag(std::move(tag)) {}

    [[nodiscard]] std::uint8_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getTag() const { return tag; }

    /** @throws DataDecodeException */
    static TagItemDescriptor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<ItemDescriptor> clone() const override
    {
        return std::make_unique<TagItemDescriptor>(*this);
    }

private:
    std::string tag;
};

}  // namespace bedrock_protocol::types::recipe
