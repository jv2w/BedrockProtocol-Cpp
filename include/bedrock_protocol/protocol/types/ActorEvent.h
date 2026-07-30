/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ActorEvent.php
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

class ActorEvent final {
public:
    ActorEvent() = delete;  //NOOP

    static constexpr std::int32_t JUMP = 1;
    static constexpr std::int32_t HURT_ANIMATION = 2;
    static constexpr std::int32_t DEATH_ANIMATION = 3;
    static constexpr std::int32_t ARM_SWING = 4;
    static constexpr std::int32_t STOP_ATTACK = 5;
    static constexpr std::int32_t TAME_FAIL = 6;
    static constexpr std::int32_t TAME_SUCCESS = 7;
    static constexpr std::int32_t SHAKE_WET = 8;
    static constexpr std::int32_t USE_ITEM = 9;
    static constexpr std::int32_t EAT_GRASS_ANIMATION = 10;
    static constexpr std::int32_t FISH_HOOK_BUBBLE = 11;
    static constexpr std::int32_t FISH_HOOK_POSITION = 12;
    static constexpr std::int32_t FISH_HOOK_HOOK = 13;
    static constexpr std::int32_t FISH_HOOK_TEASE = 14;
    static constexpr std::int32_t SQUID_INK_CLOUD = 15;
    static constexpr std::int32_t ZOMBIE_VILLAGER_CURE = 16;
    static constexpr std::int32_t PLAY_AMBIENT_SOUND = 17;
    static constexpr std::int32_t RESPAWN = 18;
    static constexpr std::int32_t IRON_GOLEM_OFFER_FLOWER = 19;
    static constexpr std::int32_t IRON_GOLEM_WITHDRAW_FLOWER = 20;
    static constexpr std::int32_t LOVE_PARTICLES = 21;  //breeding
    static constexpr std::int32_t VILLAGER_ANGRY = 22;
    static constexpr std::int32_t VILLAGER_HAPPY = 23;
    static constexpr std::int32_t WITCH_SPELL_PARTICLES = 24;
    static constexpr std::int32_t FIREWORK_PARTICLES = 25;
    static constexpr std::int32_t IN_LOVE_PARTICLES = 26;
    static constexpr std::int32_t SILVERFISH_SPAWN_ANIMATION = 27;
    static constexpr std::int32_t GUARDIAN_ATTACK = 28;
    static constexpr std::int32_t WITCH_DRINK_POTION = 29;
    static constexpr std::int32_t WITCH_THROW_POTION = 30;
    static constexpr std::int32_t MINECART_TNT_PRIME_FUSE = 31;
    static constexpr std::int32_t CREEPER_PRIME_FUSE = 32;
    static constexpr std::int32_t AIR_SUPPLY_EXPIRED = 33;
    static constexpr std::int32_t PLAYER_ADD_XP_LEVELS = 34;
    static constexpr std::int32_t ELDER_GUARDIAN_CURSE = 35;
    static constexpr std::int32_t AGENT_ARM_SWING = 36;
    static constexpr std::int32_t ENDER_DRAGON_DEATH = 37;
    static constexpr std::int32_t DUST_PARTICLES = 38;  //not sure what this is
    static constexpr std::int32_t ARROW_SHAKE = 39;

    static constexpr std::int32_t EATING_ITEM = 57;

    static constexpr std::int32_t BABY_ANIMAL_FEED = 60;  //green particles, like bonemeal on crops
    static constexpr std::int32_t DEATH_SMOKE_CLOUD = 61;
    static constexpr std::int32_t COMPLETE_TRADE = 62;
    static constexpr std::int32_t REMOVE_LEASH = 63;  //data 1 = cut leash
    static constexpr std::int32_t CARAVAN_UPDATED = 64;
    static constexpr std::int32_t CONSUME_TOTEM = 65;
    static constexpr std::int32_t DEPRECATED_UPDATE_STRUCTURE_FEATURE = 66;  //mojang...
    static constexpr std::int32_t ENTITY_SPAWN = 67;  //used for MinecraftEventing stuff, not needed
    static constexpr std::int32_t DRAGON_PUKE = 68;  //they call this puke particles
    static constexpr std::int32_t ITEM_ENTITY_MERGE = 69;
    static constexpr std::int32_t START_SWIM = 70;
    static constexpr std::int32_t BALLOON_POP = 71;
    static constexpr std::int32_t TREASURE_HUNT = 72;
    static constexpr std::int32_t AGENT_SUMMON = 73;
    static constexpr std::int32_t CHARGED_ITEM = 74;
    static constexpr std::int32_t FALL = 75;
    static constexpr std::int32_t GROW_UP = 76;
    static constexpr std::int32_t VIBRATION_DETECTED = 77;
    static constexpr std::int32_t DRINK_MILK = 78;
    static constexpr std::int32_t SHAKE_WETNESS_STOP = 79;
    static constexpr std::int32_t KINETIC_DAMAGE_DEALT = 80;
    static constexpr std::int32_t HURT_WITHOUT_RECEIVING_DAMAGE = 81;

    //TODO: add more events
};

}  // namespace bedrock_protocol::types
