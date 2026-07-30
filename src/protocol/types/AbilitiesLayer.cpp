/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AbilitiesLayer.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AbilitiesLayer.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

using encoding::LE;

AbilitiesLayer AbilitiesLayer::decode(encoding::ByteBufferReader &in)
{
    const auto layerId = LE::readUnsignedShort(in);
    const auto setAbilities = LE::readUnsignedInt(in);
    const auto setAbilityValues = LE::readUnsignedInt(in);
    auto flySpeedValue = LE::readFloat(in);
    auto verticalFlySpeedValue = LE::readFloat(in);
    auto walkSpeedValue = LE::readFloat(in);

    std::unordered_map<std::int32_t, bool> boolAbilities;
    for (std::int32_t i = 0; i < NUMBER_OF_ABILITIES; i++) {
        if (i == ABILITY_FLY_SPEED || i == ABILITY_WALK_SPEED) {
            continue;
        }
        if ((setAbilities & (1U << i)) != 0) {
            boolAbilities[i] = (setAbilityValues & (1U << i)) != 0;
        }
    }
    std::optional<float> flySpeed = flySpeedValue;
    std::optional<float> verticalFlySpeed = verticalFlySpeedValue;
    std::optional<float> walkSpeed = walkSpeedValue;
    if ((setAbilities & (1U << ABILITY_FLY_SPEED)) == 0) {
        if (flySpeedValue != 0.0F) {
            throw PacketDecodeException("Fly speed should be zero if the layer does not set it");
        }
        flySpeed = std::nullopt;
    }
    if ((setAbilities & (1U << ABILITY_VERTICAL_FLY_SPEED)) == 0) {
        if (verticalFlySpeedValue != 0.0F) {
            throw PacketDecodeException("Vertical fly speed should be zero if the layer does not set it");
        }
        verticalFlySpeed = std::nullopt;
    }
    if ((setAbilities & (1U << ABILITY_WALK_SPEED)) == 0) {
        if (walkSpeedValue != 0.0F) {
            throw PacketDecodeException("Walk speed should be zero if the layer does not set it");
        }
        walkSpeed = std::nullopt;
    }

    return AbilitiesLayer(layerId, std::move(boolAbilities), flySpeed, verticalFlySpeed, walkSpeed);
}

void AbilitiesLayer::encode(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedShort(out, layerId);

    std::uint32_t setAbilities = 0;
    std::uint32_t setAbilityValues = 0;
    for (const auto &[ability, value] : boolAbilities) {
        setAbilities |= (1U << ability);
        setAbilityValues |= (value ? 1U << ability : 0U);
    }
    if (flySpeed.has_value()) {
        setAbilities |= (1U << ABILITY_FLY_SPEED);
    }
    if (verticalFlySpeed.has_value()) {
        setAbilities |= (1U << ABILITY_VERTICAL_FLY_SPEED);
    }
    if (walkSpeed.has_value()) {
        setAbilities |= (1U << ABILITY_WALK_SPEED);
    }

    LE::writeUnsignedInt(out, setAbilities);
    LE::writeUnsignedInt(out, setAbilityValues);
    LE::writeFloat(out, flySpeed.value_or(0));
    LE::writeFloat(out, verticalFlySpeed.value_or(0));
    LE::writeFloat(out, walkSpeed.value_or(0));
}

}  // namespace bedrock_protocol::types
