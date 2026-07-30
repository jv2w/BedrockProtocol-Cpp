/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/ShortMetadataProperty.php
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
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/types/entity/EntityMetadataTypes.h"
#include "bedrock_protocol/protocol/types/entity/MetadataProperty.h"

namespace bedrock_protocol::types::entity {

class ShortMetadataProperty final : public MetadataProperty {
public:
    static constexpr std::int32_t ID = EntityMetadataTypes::SHORT;

    /**
     * @note The parameter is widened to 64 bits so that the PHP range check below stays observable;
     *       the value itself is stored in the width the wire format uses.
     */
    explicit ShortMetadataProperty(std::int64_t value)
    {
        if (value < min() || value > max()) {
            throw std::invalid_argument("Value is out of range " + std::to_string(min()) + " - " + std::to_string(max()));
        }
        this->value = static_cast<std::int16_t>(value);
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::int16_t getValue() const { return value; }

    [[nodiscard]] bool equals(const MetadataProperty &other) const override
    {
        const auto *casted = dynamic_cast<const ShortMetadataProperty *>(&other);
        return casted != nullptr && casted->value == value;
    }

    /**
     * @note flags maps flag index to whether the flag is set.
     */
    static ShortMetadataProperty buildFromFlags(const std::unordered_map<std::int32_t, bool> &flags)
    {
        std::int64_t value = 0;
        for (const auto &[flag, v] : flags) {
            if (v) {
                value |= static_cast<std::int64_t>(1) << flag;
            }
        }
        return ShortMetadataProperty(value);
    }

    static ShortMetadataProperty read(encoding::ByteBufferReader &in)
    {
        return ShortMetadataProperty(encoding::LE::readSignedShort(in));
    }

    void write(encoding::ByteBufferWriter &out) const override { encoding::LE::writeSignedShort(out, value); }

    [[nodiscard]] std::unique_ptr<MetadataProperty> clone() const override
    {
        return std::make_unique<ShortMetadataProperty>(*this);
    }

protected:
    static constexpr std::int64_t min() { return -0x8000; }

    static constexpr std::int64_t max() { return 0x7fff; }

private:
    std::int16_t value = 0;
};

}  // namespace bedrock_protocol::types::entity
