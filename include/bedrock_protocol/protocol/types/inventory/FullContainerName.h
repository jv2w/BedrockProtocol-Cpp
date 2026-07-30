/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/FullContainerName.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::inventory {

class FullContainerName final {
public:
    explicit FullContainerName(std::uint8_t containerId, std::optional<std::uint32_t> dynamicId = std::nullopt)
        : containerId(containerId), dynamicId(dynamicId)
    {
    }

    [[nodiscard]] std::uint8_t getContainerId() const { return containerId; }

    [[nodiscard]] const std::optional<std::uint32_t> &getDynamicId() const { return dynamicId; }

    static FullContainerName read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t containerId;
    std::optional<std::uint32_t> dynamicId;
};

}  // namespace bedrock_protocol::types::inventory
