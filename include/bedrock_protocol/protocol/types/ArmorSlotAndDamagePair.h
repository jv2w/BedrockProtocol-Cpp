/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ArmorSlotAndDamagePair.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/ArmorSlot.h"

namespace bedrock_protocol::types {

/**
 * @see PlayerArmorDamagePacket
 */
class ArmorSlotAndDamagePair final {
public:
    ArmorSlotAndDamagePair(ArmorSlot slot, std::uint16_t damage) : slot(slot), damage(damage) {}

    [[nodiscard]] ArmorSlot getSlot() const { return slot; }

    [[nodiscard]] std::uint16_t getDamage() const { return damage; }

    /**
     * @throws PacketDecodeException
     */
    static ArmorSlotAndDamagePair read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    ArmorSlot slot;
    std::uint16_t damage;
};

}  // namespace bedrock_protocol::types
