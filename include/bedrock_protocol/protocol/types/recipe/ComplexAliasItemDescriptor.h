/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/recipe/ComplexAliasItemDescriptor.php
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

class ComplexAliasItemDescriptor final : public ItemDescriptor {
public:
    static constexpr std::uint8_t ID = ItemDescriptorType::COMPLEX_ALIAS;

    explicit ComplexAliasItemDescriptor(std::string alias) : alias(std::move(alias)) {}

    [[nodiscard]] std::uint8_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getAlias() const { return alias; }

    /** @throws DataDecodeException */
    static ComplexAliasItemDescriptor read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<ItemDescriptor> clone() const override
    {
        return std::make_unique<ComplexAliasItemDescriptor>(*this);
    }

private:
    std::string alias;
};

}  // namespace bedrock_protocol::types::recipe
