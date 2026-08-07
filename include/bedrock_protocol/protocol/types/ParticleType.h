/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ParticleType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

class ParticleType final {
public:
    ParticleType() = delete;

    static constexpr std::int32_t UNDEFINED = 0;
    static constexpr std::int32_t BUBBLE = 1;
    static constexpr std::int32_t BUBBLE_MANUAL = 2;
    static constexpr std::int32_t CRIT = 3;
    static constexpr std::int32_t BLOCK_FORCE_FIELD = 4;
    static constexpr std::int32_t SMOKE = 5;
    static constexpr std::int32_t EXPLODE = 6;
    static constexpr std::int32_t EVAPORATION = 7;
    static constexpr std::int32_t FLAME = 8;
    static constexpr std::int32_t LAVA = 9;
    static constexpr std::int32_t LARGE_SMOKE = 10;
    static constexpr std::int32_t RED_DUST = 11;
    static constexpr std::int32_t RISING_BORDER_DUST = 12;
    static constexpr std::int32_t ICON_CRACK = 13;
    static constexpr std::int32_t SNOWBALL_POOF = 14;
    static constexpr std::int32_t LARGE_EXPLODE = 15;
    static constexpr std::int32_t HUGE_EXPLOSION = 16;
    static constexpr std::int32_t MOB_FLAME = 17;
    static constexpr std::int32_t HEART = 18;
    static constexpr std::int32_t TERRAIN = 19;
    static constexpr std::int32_t TOWN_AURA = 20;
    static constexpr std::int32_t PORTAL = 21;
    static constexpr std::int32_t MOB_PORTAL = 22;
    static constexpr std::int32_t WATER_SPLASH = 23;
    static constexpr std::int32_t WATER_SPLASH_MANUAL = 24;
    static constexpr std::int32_t WATER_WAKE = 25;
    static constexpr std::int32_t DRIP_WATER = 26;
    static constexpr std::int32_t DRIP_LAVA = 27;
    static constexpr std::int32_t DRIP_HONEY = 28;
    static constexpr std::int32_t STALACTITE_DRIP_WATER = 29;
    static constexpr std::int32_t STALACTITE_DRIP_LAVA = 30;
    static constexpr std::int32_t FALLING_DUST = 31;
    static constexpr std::int32_t MOB_SPELL = 32;
    static constexpr std::int32_t MOB_SPELL_AMBIENT = 33;
    static constexpr std::int32_t MOB_SPELL_INSTANTANEOUS = 34;
    static constexpr std::int32_t INK = 35;
    static constexpr std::int32_t SLIME = 36;
    static constexpr std::int32_t RAIN_SPLASH = 37;
    static constexpr std::int32_t VILLAGER_ANGRY = 38;
    static constexpr std::int32_t VILLAGER_HAPPY = 39;
    static constexpr std::int32_t ENCHANTING_TABLE = 40;
    static constexpr std::int32_t TRACKER_EMITTER = 41;
    static constexpr std::int32_t NOTE = 42;
    static constexpr std::int32_t WITCH_SPELL = 43;
    static constexpr std::int32_t CARROT_BOOST = 44;
    static constexpr std::int32_t MOB_APPEARANCE = 45;
    static constexpr std::int32_t END_ROD = 46;
    static constexpr std::int32_t DRAGON_BREATH = 47;
    static constexpr std::int32_t SPIT = 48;
    static constexpr std::int32_t TOTEM = 49;
    static constexpr std::int32_t FOOD = 50;
    static constexpr std::int32_t FIREWORKS_STARTER = 51;
    static constexpr std::int32_t FIREWORKS = 52;
    static constexpr std::int32_t FIREWORKS_OVERLAY = 53;
    static constexpr std::int32_t BALLOON_GAS = 54;
    static constexpr std::int32_t COLOURED_FLAME = 55;
    static constexpr std::int32_t SPARKLER = 56;
    static constexpr std::int32_t CONDUIT = 57;
    static constexpr std::int32_t BUBBLE_COLUMN_UP = 58;
    static constexpr std::int32_t BUBBLE_COLUMN_DOWN = 59;
    static constexpr std::int32_t SNEEZE = 60;
    static constexpr std::int32_t SHULKER_BULLET = 61;
    static constexpr std::int32_t BLEACH = 62;
    static constexpr std::int32_t DRAGON_DESTROY_BLOCK = 63;
    static constexpr std::int32_t MYCELIUM_DUST = 64;
    static constexpr std::int32_t FALLING_BORDER_DUST = 65;
    static constexpr std::int32_t CAMPFIRE_SMOKE = 66;
    static constexpr std::int32_t CAMPFIRE_SMOKE_TALL = 67;
    static constexpr std::int32_t DRAGON_BREATH_FIRE = 68;
    static constexpr std::int32_t DRAGON_BREATH_TRAIL = 69;
    static constexpr std::int32_t BLUE_FLAME = 70;
    static constexpr std::int32_t SOUL = 71;
    static constexpr std::int32_t OBSIDIAN_TEAR = 72;
    static constexpr std::int32_t PORTAL_REVERSE = 73;
    static constexpr std::int32_t SNOWFLAKE = 74;
    static constexpr std::int32_t VIBRATION_SIGNAL = 75;
    static constexpr std::int32_t SCULK_SENSOR_REDSTONE = 76;
    static constexpr std::int32_t SPORE_BLOSSOM_SHOWER = 77;
    static constexpr std::int32_t SPORE_BLOSSOM_AMBIENT = 78;
    static constexpr std::int32_t WAX = 79;
    static constexpr std::int32_t ELECTRIC_SPARK = 80;
    static constexpr std::int32_t CANDLE_FLAME = 81;
    static constexpr std::int32_t SHRIEK = 82;
    static constexpr std::int32_t SCULK_SOUL = 83;
    static constexpr std::int32_t SONIC_EXPLOSION = 84;
    static constexpr std::int32_t BRUSH_DUST = 85;
    static constexpr std::int32_t CHERRY_LEAVES = 86;
    static constexpr std::int32_t DUST_PLUME = 87;
    static constexpr std::int32_t WHITE_SMOKE = 88;
    static constexpr std::int32_t WIND_EXPLOSION = 89;
    static constexpr std::int32_t BREEZE_WIND_EXPLOSION = 90;
    static constexpr std::int32_t VAULT_CONNECTION = 91;
    static constexpr std::int32_t WOLF_ARMOUR_BREAK = 92;
    static constexpr std::int32_t OMINOUS_ITEM_SPAWNER = 93;
    static constexpr std::int32_t CREAKING_CRUMBLE = 94;
    static constexpr std::int32_t PALE_OAK_LEAVES = 95;
    static constexpr std::int32_t EYEBLOSSOM_OPEN = 96;
    static constexpr std::int32_t EYEBLOSSOM_CLOSE = 97;
    static constexpr std::int32_t GREEN_FLAME = 98;
    static constexpr std::int32_t PAUSE_MOB_GROWTH = 99;
    static constexpr std::int32_t RESET_MOB_GROWTH = 100;
    static constexpr std::int32_t SULFUR_CUBE = 101;
    static constexpr std::int32_t ORANGE_POPLAR_LEAVES = 102;
    static constexpr std::int32_t RED_POPLAR_LEAVES = 103;
    static constexpr std::int32_t YELLOW_POPLAR_LEAVES = 104;
};

}  // namespace bedrock_protocol::types
