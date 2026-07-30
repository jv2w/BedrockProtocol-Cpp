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

#pragma once

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class AbilitiesLayer final {
public:
    static constexpr std::int32_t LAYER_CACHE = 0;
    static constexpr std::int32_t LAYER_BASE = 1;
    static constexpr std::int32_t LAYER_SPECTATOR = 2;
    static constexpr std::int32_t LAYER_COMMANDS = 3;
    static constexpr std::int32_t LAYER_EDITOR = 4;
    static constexpr std::int32_t LAYER_LOADING_SCREEN = 5;

    static constexpr std::int32_t ABILITY_BUILD = 0;
    static constexpr std::int32_t ABILITY_MINE = 1;
    static constexpr std::int32_t ABILITY_DOORS_AND_SWITCHES = 2;  //disabling this also disables dropping items (???)
    static constexpr std::int32_t ABILITY_OPEN_CONTAINERS = 3;
    static constexpr std::int32_t ABILITY_ATTACK_PLAYERS = 4;
    static constexpr std::int32_t ABILITY_ATTACK_MOBS = 5;
    static constexpr std::int32_t ABILITY_OPERATOR = 6;
    static constexpr std::int32_t ABILITY_TELEPORT = 7;
    static constexpr std::int32_t ABILITY_INVULNERABLE = 8;
    static constexpr std::int32_t ABILITY_FLYING = 9;
    static constexpr std::int32_t ABILITY_ALLOW_FLIGHT = 10;
    static constexpr std::int32_t ABILITY_INFINITE_RESOURCES = 11;  //in vanilla they call this "instabuild", which is a bad name
    static constexpr std::int32_t ABILITY_LIGHTNING = 12;  //???

private:
    static constexpr std::int32_t ABILITY_FLY_SPEED = 13;
    static constexpr std::int32_t ABILITY_WALK_SPEED = 14;

public:
    static constexpr std::int32_t ABILITY_MUTED = 15;
    static constexpr std::int32_t ABILITY_WORLD_BUILDER = 16;
    static constexpr std::int32_t ABILITY_NO_CLIP = 17;
    static constexpr std::int32_t ABILITY_PRIVILEGED_BUILDER = 18;
    static constexpr std::int32_t ABILITY_VERTICAL_FLY_SPEED = 19;

    static constexpr std::int32_t NUMBER_OF_ABILITIES = 20;

    AbilitiesLayer(std::uint16_t layerId, std::unordered_map<std::int32_t, bool> boolAbilities,
                   std::optional<float> flySpeed, std::optional<float> verticalFlySpeed, std::optional<float> walkSpeed)
        : layerId(layerId), boolAbilities(std::move(boolAbilities)), flySpeed(flySpeed),
          verticalFlySpeed(verticalFlySpeed), walkSpeed(walkSpeed)
    {
    }

    [[nodiscard]] std::uint16_t getLayerId() const { return layerId; }

    /**
     * Returns a list of abilities set/overridden by this layer. If the ability value is not set, the index is omitted.
     * @note Keyed by one of the ABILITY_* constants.
     */
    [[nodiscard]] const std::unordered_map<std::int32_t, bool> &getBoolAbilities() const { return boolAbilities; }

    [[nodiscard]] const std::optional<float> &getFlySpeed() const { return flySpeed; }

    [[nodiscard]] const std::optional<float> &getVerticalFlySpeed() const { return verticalFlySpeed; }

    [[nodiscard]] const std::optional<float> &getWalkSpeed() const { return walkSpeed; }

    /**
     * @throws PacketDecodeException
     */
    static AbilitiesLayer decode(encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const;

private:
    std::uint16_t layerId;
    std::unordered_map<std::int32_t, bool> boolAbilities;
    std::optional<float> flySpeed;
    std::optional<float> verticalFlySpeed;
    std::optional<float> walkSpeed;
};

}  // namespace bedrock_protocol::types
