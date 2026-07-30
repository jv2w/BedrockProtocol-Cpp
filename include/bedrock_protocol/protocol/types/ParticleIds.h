/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ParticleIds.php
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

class ParticleIds final {
    ParticleIds()
    {
        //NOOP
    }

public:
    static constexpr std::int32_t BUBBLE = 1;
    static constexpr std::int32_t BUBBLE_MANUAL = 2;
    static constexpr std::int32_t CRITICAL = 3;
    static constexpr std::int32_t BLOCK_FORCE_FIELD = 4;
    static constexpr std::int32_t SMOKE = 5;
    static constexpr std::int32_t EXPLODE = 6;
    static constexpr std::int32_t EVAPORATION = 7;
    static constexpr std::int32_t FLAME = 8;
    static constexpr std::int32_t CANDLE_FLAME = 9;
    static constexpr std::int32_t LAVA = 10;
    static constexpr std::int32_t LARGE_SMOKE = 11;
    static constexpr std::int32_t REDSTONE = 12;
    static constexpr std::int32_t RISING_RED_DUST = 13;
    static constexpr std::int32_t ITEM_BREAK = 14;
    static constexpr std::int32_t SNOWBALL_POOF = 15;
    static constexpr std::int32_t HUGE_EXPLODE = 16;
    static constexpr std::int32_t HUGE_EXPLODE_SEED = 17;
    static constexpr std::int32_t BREEZE_WIND_EXPLOSION = 18;
    static constexpr std::int32_t MOB_FLAME = 19;
    static constexpr std::int32_t HEART = 20;
    static constexpr std::int32_t TERRAIN = 21;
    static constexpr std::int32_t SUSPENDED_TOWN = 22, TOWN_AURA = 22;
    static constexpr std::int32_t PORTAL = 23;
    //24 same as 23
    static constexpr std::int32_t SPLASH = 25, WATER_SPLASH = 25;
    static constexpr std::int32_t WATER_SPLASH_MANUAL = 26;
    static constexpr std::int32_t WATER_WAKE = 27;
    static constexpr std::int32_t DRIP_WATER = 28;
    static constexpr std::int32_t DRIP_LAVA = 29;
    static constexpr std::int32_t DRIP_HONEY = 30;
    static constexpr std::int32_t STALACTITE_DRIP_WATER = 31;
    static constexpr std::int32_t STALACTITE_DRIP_LAVA = 32;
    static constexpr std::int32_t FALLING_DUST = 33, DUST = 33;
    static constexpr std::int32_t MOB_SPELL = 34;
    static constexpr std::int32_t MOB_SPELL_AMBIENT = 35;
    static constexpr std::int32_t MOB_SPELL_INSTANTANEOUS = 36;
    static constexpr std::int32_t INK = 37;
    static constexpr std::int32_t SLIME = 38;
    static constexpr std::int32_t RAIN_SPLASH = 39;
    static constexpr std::int32_t VILLAGER_ANGRY = 40;
    static constexpr std::int32_t VILLAGER_HAPPY = 41;
    static constexpr std::int32_t ENCHANTMENT_TABLE = 42;
    static constexpr std::int32_t TRACKING_EMITTER = 43;
    static constexpr std::int32_t NOTE = 44;
    static constexpr std::int32_t WITCH_SPELL = 45;
    static constexpr std::int32_t CARROT = 46;
    static constexpr std::int32_t MOB_APPEARANCE = 47;
    static constexpr std::int32_t END_ROD = 48;
    static constexpr std::int32_t DRAGONS_BREATH = 49;
    static constexpr std::int32_t SPIT = 50;
    static constexpr std::int32_t TOTEM = 51;
    static constexpr std::int32_t FOOD = 52;
    static constexpr std::int32_t FIREWORKS_STARTER = 53;
    static constexpr std::int32_t FIREWORKS_SPARK = 54;
    static constexpr std::int32_t FIREWORKS_OVERLAY = 55;
    static constexpr std::int32_t BALLOON_GAS = 56;
    static constexpr std::int32_t COLORED_FLAME = 57;
    static constexpr std::int32_t SPARKLER = 58;
    static constexpr std::int32_t CONDUIT = 59;
    static constexpr std::int32_t BUBBLE_COLUMN_UP = 60;
    static constexpr std::int32_t BUBBLE_COLUMN_DOWN = 61;
    static constexpr std::int32_t SNEEZE = 62;
    static constexpr std::int32_t SHULKER_BULLET = 63;
    static constexpr std::int32_t BLEACH = 64;
    static constexpr std::int32_t DRAGON_DESTROY_BLOCK = 65;
    static constexpr std::int32_t MYCELIUM_DUST = 66;
    static constexpr std::int32_t FALLING_RED_DUST = 67;
    static constexpr std::int32_t CAMPFIRE_SMOKE = 68;
    static constexpr std::int32_t TALL_CAMPFIRE_SMOKE = 69;
    static constexpr std::int32_t DRAGON_BREATH_FIRE = 70;
    static constexpr std::int32_t DRAGON_BREATH_TRAIL = 71;
    static constexpr std::int32_t BLUE_FLAME = 72;
    static constexpr std::int32_t SOUL = 73;
    static constexpr std::int32_t OBSIDIAN_TEAR = 74;
    static constexpr std::int32_t PORTAL_REVERSE = 75;
    static constexpr std::int32_t SNOWFLAKE = 76;
    static constexpr std::int32_t VIBRATION_SIGNAL = 77;
    static constexpr std::int32_t SCULK_SENSOR_REDSTONE = 78;
    static constexpr std::int32_t SPORE_BLOSSOM_SHOWER = 79;
    static constexpr std::int32_t SPORE_BLOSSOM_AMBIENT = 80;
    static constexpr std::int32_t WAX = 81;
    static constexpr std::int32_t ELECTRIC_SPARK = 82;
    static constexpr std::int32_t SHRIEK = 83;
    static constexpr std::int32_t SCULK_SOUL = 84;
    static constexpr std::int32_t SONIC_EXPLOSION = 85;
    static constexpr std::int32_t BRUSH_DUST = 86;
    static constexpr std::int32_t CHERRY_LEAVES = 87;
    static constexpr std::int32_t DUST_PLUME = 88;
    static constexpr std::int32_t WHITE_SMOKE = 89;
    static constexpr std::int32_t VAULT_CONNECTION = 90;
    static constexpr std::int32_t WIND_EXPLOSION = 91;
    static constexpr std::int32_t WOLF_ARMOR_CRACK = 92;
    static constexpr std::int32_t OMINOUS_ITEM_SPAWNER = 93;
    static constexpr std::int32_t CREAKING_CRUMBLE = 94;
    static constexpr std::int32_t PALE_OAK_LEAVES = 95;
    static constexpr std::int32_t EYEBLOSSOM_OPEN = 96;
    static constexpr std::int32_t EYEBLOSSOM_CLOSE = 97;
    static constexpr std::int32_t GREEN_FLAME = 98;
    static constexpr std::int32_t PAUSE_MOB_GROWTH = 99;
    static constexpr std::int32_t RESET_MOB_GROWTH = 100;
};

}  // namespace bedrock_protocol::types
