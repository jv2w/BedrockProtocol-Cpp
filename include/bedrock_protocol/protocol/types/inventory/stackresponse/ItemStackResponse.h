/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackresponse/ItemStackResponse.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.h"

namespace bedrock_protocol::types::inventory::stackresponse {

class ItemStackResponse final {
public:
    static constexpr std::uint8_t RESULT_OK = 0;
    static constexpr std::uint8_t RESULT_ERROR = 1;
    //TODO: there are a ton more possible result types but we don't need them yet and they are wayyyyyy too many for me
    //to waste my time on right now...

    /**
     * @note containerInfos contains ItemStackResponseContainerInfo elements
     */
    ItemStackResponse(std::uint8_t result, std::int32_t requestId,
                      std::vector<ItemStackResponseContainerInfo> containerInfos = {});

    [[nodiscard]] std::uint8_t getResult() const { return result; }

    [[nodiscard]] std::int32_t getRequestId() const { return requestId; }

    [[nodiscard]] const std::vector<ItemStackResponseContainerInfo> &getContainerInfos() const
    {
        return containerInfos;
    }

    static ItemStackResponse read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t result;
    std::int32_t requestId;
    std::vector<ItemStackResponseContainerInfo> containerInfos;
};

}  // namespace bedrock_protocol::types::inventory::stackresponse
