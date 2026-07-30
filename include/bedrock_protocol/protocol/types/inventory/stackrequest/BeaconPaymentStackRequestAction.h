/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/inventory/stackrequest/BeaconPaymentStackRequestAction.php
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
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestAction.h"
#include "bedrock_protocol/protocol/types/inventory/stackrequest/ItemStackRequestActionType.h"

namespace bedrock_protocol::types::inventory::stackrequest {

/**
 * Completes a transaction involving a beacon consuming input to produce effects.
 */
class BeaconPaymentStackRequestAction final : public ItemStackRequestAction {
public:
    static constexpr std::int32_t ID = ItemStackRequestActionType::BEACON_PAYMENT;

    BeaconPaymentStackRequestAction(std::int32_t primaryEffectId, std::int32_t secondaryEffectId) :
        primaryEffectId(primaryEffectId), secondaryEffectId(secondaryEffectId) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::int32_t getPrimaryEffectId() const { return primaryEffectId; }

    [[nodiscard]] std::int32_t getSecondaryEffectId() const { return secondaryEffectId; }

    static BeaconPaymentStackRequestAction read(encoding::ByteBufferReader &in) {
        const auto primary = encoding::VarInt::readSignedInt(in);
        const auto secondary = encoding::VarInt::readSignedInt(in);
        return BeaconPaymentStackRequestAction(primary, secondary);
    }

    void write(encoding::ByteBufferWriter &out) const override {
        encoding::VarInt::writeSignedInt(out, primaryEffectId);
        encoding::VarInt::writeSignedInt(out, secondaryEffectId);
    }

    [[nodiscard]] std::unique_ptr<ItemStackRequestAction> clone() const override {
        return std::make_unique<BeaconPaymentStackRequestAction>(*this);
    }

private:
    std::int32_t primaryEffectId;
    std::int32_t secondaryEffectId;
};

}  // namespace bedrock_protocol::types::inventory::stackrequest
