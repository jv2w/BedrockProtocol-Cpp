/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/ItemStackRequestAction.php
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

#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types::inventory::stackrequest {

class ItemStackRequestAction {
public:
    virtual ~ItemStackRequestAction() = default;

    [[nodiscard]] virtual std::int32_t getTypeId() const = 0;

    virtual void write(encoding::ByteBufferWriter &out) const = 0;

    [[nodiscard]] virtual std::unique_ptr<ItemStackRequestAction> clone() const = 0;

protected:
    ItemStackRequestAction() = default;
    ItemStackRequestAction(const ItemStackRequestAction &) = default;
    ItemStackRequestAction(ItemStackRequestAction &&) = default;
    ItemStackRequestAction &operator=(const ItemStackRequestAction &) = default;
    ItemStackRequestAction &operator=(ItemStackRequestAction &&) = default;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
