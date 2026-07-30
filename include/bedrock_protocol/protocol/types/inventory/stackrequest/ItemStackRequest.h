/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/inventory/stackrequest/ItemStackRequest.php
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
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"

namespace bedrock_protocol::types::inventory::stackrequest {

class ItemStackRequest final {
public:
    ItemStackRequest(std::int32_t requestId, std::vector<std::unique_ptr<ItemStackRequestAction>> actions,
                     std::vector<std::string> filterStrings, std::int32_t filterStringCause) :
        requestId(requestId),
        actions(std::move(actions)),
        filterStrings(std::move(filterStrings)),
        filterStringCause(filterStringCause) {}

    [[nodiscard]] std::int32_t getRequestId() const { return requestId; }

    [[nodiscard]] const std::vector<std::unique_ptr<ItemStackRequestAction>> &getActions() const { return actions; }

    [[nodiscard]] const std::vector<std::string> &getFilterStrings() const { return filterStrings; }

    [[nodiscard]] std::int32_t getFilterStringCause() const { return filterStringCause; }

    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static ItemStackRequest read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

    /**
     * Deep-copies this request. Needed because the actions are held by unique_ptr, which makes this type move-only.
     */
    [[nodiscard]] ItemStackRequest clone() const;

private:
    /**
     * @throws DataDecodeException
     * @throws PacketDecodeException
     */
    static std::unique_ptr<ItemStackRequestAction> readAction(encoding::ByteBufferReader &in, std::int32_t typeId);

    std::int32_t requestId;
    std::vector<std::unique_ptr<ItemStackRequestAction>> actions;
    std::vector<std::string> filterStrings;
    std::int32_t filterStringCause;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
