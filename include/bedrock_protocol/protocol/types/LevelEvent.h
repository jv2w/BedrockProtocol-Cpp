/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/LevelEvent.php
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

class LevelEvent final {
public:
    LevelEvent() = delete;

    static constexpr std::int32_t SOUND_CLICK = 1000;
    static constexpr std::int32_t SOUND_CLICK_FAIL = 1001;
    static constexpr std::int32_t SOUND_SHOOT = 1002;
    static constexpr std::int32_t SOUND_DOOR = 1003;
    static constexpr std::int32_t SOUND_FIZZ = 1004;
    static constexpr std::int32_t SOUND_IGNITE = 1005;
    static constexpr std::int32_t SOUND_PLAY_RECORDING = 1006;
    static constexpr std::int32_t SOUND_GHAST = 1007;
    static constexpr std::int32_t SOUND_GHAST_SHOOT = 1008;
    static constexpr std::int32_t SOUND_BLAZE_SHOOT = 1009;
    static constexpr std::int32_t SOUND_DOOR_BUMP = 1010;

    static constexpr std::int32_t SOUND_DOOR_CRASH = 1012;

    static constexpr std::int32_t SOUND_ZOMBIE_INFECTED = 1016;
    static constexpr std::int32_t SOUND_ZOMBIE_CONVERT = 1017;
    static constexpr std::int32_t SOUND_ENDERMAN_TELEPORT = 1018;

    static constexpr std::int32_t SOUND_ANVIL_BREAK = 1020;
    static constexpr std::int32_t SOUND_ANVIL_USE = 1021;
    static constexpr std::int32_t SOUND_ANVIL_FALL = 1022;

    static constexpr std::int32_t SOUND_POP = 1030;

    static constexpr std::int32_t SOUND_PORTAL = 1032;

    static constexpr std::int32_t SOUND_ITEMFRAME_ADD_ITEM = 1040;
    static constexpr std::int32_t SOUND_ITEMFRAME_REMOVE = 1041;
    static constexpr std::int32_t SOUND_ITEMFRAME_PLACE = 1042;
    static constexpr std::int32_t SOUND_ITEMFRAME_REMOVE_ITEM = 1043;
    static constexpr std::int32_t SOUND_ITEMFRAME_ROTATE_ITEM = 1044;

    static constexpr std::int32_t SOUND_CAMERA = 1050;
    static constexpr std::int32_t SOUND_ORB = 1051;
    static constexpr std::int32_t SOUND_TOTEM = 1052;

    static constexpr std::int32_t SOUND_ARMOR_STAND_BREAK = 1060;
    static constexpr std::int32_t SOUND_ARMOR_STAND_HIT = 1061;
    static constexpr std::int32_t SOUND_ARMOR_STAND_FALL = 1062;
    static constexpr std::int32_t SOUND_ARMOR_STAND_PLACE = 1063;
    static constexpr std::int32_t SOUND_POINTED_DRIPSTONE_FALL = 1064;
    static constexpr std::int32_t SOUND_DYE_USED = 1065;
    static constexpr std::int32_t SOUND_INK_SAC_USED = 1066;
    static constexpr std::int32_t SOUND_AMETHYST_RESONATE = 1067;

    static constexpr std::int32_t QUEUE_CUSTOM_MUSIC = 1900;
    static constexpr std::int32_t PLAY_CUSTOM_MUSIC = 1901;
    static constexpr std::int32_t STOP_CUSTOM_MUSIC = 1902;
    static constexpr std::int32_t SET_MUSIC_VOLUME = 1903;

    static constexpr std::int32_t PARTICLE_SHOOT = 2000;
    static constexpr std::int32_t PARTICLE_DESTROY = 2001; //sound + particles
    static constexpr std::int32_t PARTICLE_SPLASH = 2002;
    static constexpr std::int32_t PARTICLE_EYE_DESPAWN = 2003;
    static constexpr std::int32_t PARTICLE_SPAWN = 2004;
    static constexpr std::int32_t BONE_MEAL_USE = 2005; //sound + green particles
    static constexpr std::int32_t GUARDIAN_CURSE = 2006;
    static constexpr std::int32_t PARTICLE_DEATH_SMOKE = 2007;
    static constexpr std::int32_t PARTICLE_BLOCK_FORCE_FIELD = 2008;
    static constexpr std::int32_t PARTICLE_PROJECTILE_HIT = 2009;
    static constexpr std::int32_t PARTICLE_DRAGON_EGG_TELEPORT = 2010;
    static constexpr std::int32_t PARTICLE_CROP_EATEN = 2011;
    static constexpr std::int32_t PARTICLE_CRITICAL_HIT = 2012;
    static constexpr std::int32_t PARTICLE_ENDERMAN_TELEPORT = 2013;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK = 2014;
    static constexpr std::int32_t PARTICLE_BUBBLE = 2015;
    static constexpr std::int32_t PARTICLE_EVAPORATE = 2016;
    static constexpr std::int32_t PARTICLE_ARMOR_STAND_DESTROY = 2017;
    static constexpr std::int32_t PARTICLE_EGG_PUNCH = 2018;
    static constexpr std::int32_t PARTICLE_EGG_BREAK = 2019;
    static constexpr std::int32_t PARTICLE_ICE_EVAPORATE = 2020;
    static constexpr std::int32_t PARTICLE_DESTROY_NO_SOUND = 2021;
    static constexpr std::int32_t PARTICLE_KNOCKBACK_ROAR = 2022; //spews out tons of white particles
    static constexpr std::int32_t PARTICLE_TELEPORT_TRAIL = 2023;
    static constexpr std::int32_t PARTICLE_POINT_CLOUD = 2024;
    static constexpr std::int32_t PARTICLE_EXPLODE = 2025; //data >= 2 = huge explode seed, otherwise huge explode
    static constexpr std::int32_t PARTICLE_BLOCK_EXPLODE = 2026;
    static constexpr std::int32_t PARTICLE_VIBRATION_SIGNAL = 2027;
    static constexpr std::int32_t PARTICLE_DRIPSTONE_DRIP = 2028;
    static constexpr std::int32_t PARTICLE_FIZZ = 2029;
    static constexpr std::int32_t COPPER_WAX_ON = 2030; //sound + particles
    static constexpr std::int32_t COPPER_WAX_OFF = 2031; //sound + particles
    static constexpr std::int32_t COPPER_SCRAPE = 2032; //sound + particles
    static constexpr std::int32_t PARTICLE_ELECTRIC_SPARK = 2033; //lightning rod
    static constexpr std::int32_t PARTICLE_TURTLE_EGG_GROW = 2034;
    static constexpr std::int32_t PARTICLE_SCULK_SHRIEK = 2035;
    static constexpr std::int32_t PARTICLE_SCULK_CATALYST_BLOOM = 2036;
    static constexpr std::int32_t SCULK_CHARGE = 2037;
    static constexpr std::int32_t SCULK_CHARGE_POP = 2038;
    static constexpr std::int32_t SONIC_EXPLOSION = 2039;

    static constexpr std::int32_t PARTICLE_DUST_PLUME = 2040;

    static constexpr std::int32_t START_RAIN = 3001;
    static constexpr std::int32_t START_THUNDER = 3002;
    static constexpr std::int32_t STOP_RAIN = 3003;
    static constexpr std::int32_t STOP_THUNDER = 3004;
    static constexpr std::int32_t PAUSE_GAME = 3005; //data: 1 to pause, 0 to resume
    static constexpr std::int32_t PAUSE_GAME_NO_SCREEN = 3006; //data: 1 to pause, 0 to resume - same effect as normal pause but without screen
    static constexpr std::int32_t SET_GAME_SPEED = 3007; //x coordinate of pos = scale factor (default 1.0)

    static constexpr std::int32_t REDSTONE_TRIGGER = 3500;
    static constexpr std::int32_t CAULDRON_EXPLODE = 3501;
    static constexpr std::int32_t CAULDRON_DYE_ARMOR = 3502;
    static constexpr std::int32_t CAULDRON_CLEAN_ARMOR = 3503;
    static constexpr std::int32_t CAULDRON_FILL_POTION = 3504;
    static constexpr std::int32_t CAULDRON_TAKE_POTION = 3505;
    static constexpr std::int32_t CAULDRON_FILL_WATER = 3506;
    static constexpr std::int32_t CAULDRON_TAKE_WATER = 3507;
    static constexpr std::int32_t CAULDRON_ADD_DYE = 3508;
    static constexpr std::int32_t CAULDRON_CLEAN_BANNER = 3509; //particle + sound
    static constexpr std::int32_t PARTICLE_CAULDRON_FLUSH = 3510;
    static constexpr std::int32_t PARTICLE_AGENT_SPAWN = 3511;
    static constexpr std::int32_t SOUND_CAULDRON_FILL_LAVA = 3512;
    static constexpr std::int32_t SOUND_CAULDRON_TAKE_LAVA = 3513;
    static constexpr std::int32_t SOUND_CAULDRON_FILL_POWDER_SNOW = 3514;
    static constexpr std::int32_t SOUND_CAULDRON_TAKE_POWDER_SNOW = 3515;

    static constexpr std::int32_t BLOCK_START_BREAK = 3600;
    static constexpr std::int32_t BLOCK_STOP_BREAK = 3601;
    static constexpr std::int32_t BLOCK_BREAK_SPEED = 3602;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_DOWN = 3603;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_UP = 3604;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_NORTH = 3605;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_SOUTH = 3606;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_WEST = 3607;
    static constexpr std::int32_t PARTICLE_PUNCH_BLOCK_EAST = 3608;
    static constexpr std::int32_t PARTICLE_SHOOT_WHITE_SMOKE = 3609;
    static constexpr std::int32_t PARTICLE_BREEZE_WIND_EXPLOSION = 3610;
    static constexpr std::int32_t PARTICLE_TRIAL_SPAWNER_DETECTION = 3611;
    static constexpr std::int32_t PARTICLE_TRIAL_SPAWNER_SPAWNING = 3612;
    static constexpr std::int32_t PARTICLE_TRIAL_SPAWNER_EJECTING = 3613;
    static constexpr std::int32_t PARTICLE_WIND_EXPLOSION = 3614;
    static constexpr std::int32_t PARTICLE_TRIAL_SPAWNER_DETECTION_CHARGED = 3615;
    static constexpr std::int32_t PARTICLE_TRIAL_SPAWNER_BECOME_CHARGED = 3616;

    static constexpr std::int32_t SET_DATA = 4000;

    static constexpr std::int32_t PLAYERS_SLEEPING = 9800;
    static constexpr std::int32_t NUMBER_OF_SLEEPING_PLAYERS = 9801;

    static constexpr std::int32_t JUMP_PREVENTED = 9810;
    static constexpr std::int32_t ANIMATION_VAULT_ACTIVATE = 9811;
    static constexpr std::int32_t ANIMATION_VAULT_DEACTIVATE = 9812;
    static constexpr std::int32_t ANIMATION_VAULT_EJECT_ITEM = 9813;
    static constexpr std::int32_t ANIMATION_SPAWN_COBWEB = 9814;
    static constexpr std::int32_t PARTICLE_SMASH_ATTACK_GROUND_DUST = 9815;
    static constexpr std::int32_t PARTICLE_CREAKING_HEART_TRAIL = 9816;

    static constexpr std::int32_t ADD_PARTICLE_MASK = 0x4000;
};

}  // namespace bedrock_protocol::types
