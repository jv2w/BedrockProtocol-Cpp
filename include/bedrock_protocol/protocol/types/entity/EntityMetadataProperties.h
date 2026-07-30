/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/EntityMetadataProperties.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::entity {

class EntityMetadataProperties final {
    EntityMetadataProperties() = delete;

public:
    /*
     * Readers beware: this isn't a nice list. Some of the properties have different types for different entities, and
     * are used for entirely different things.
     */
    static constexpr std::int32_t FLAGS = 0;
    static constexpr std::int32_t HEALTH = 1; //int (minecart/boat)
    static constexpr std::int32_t VARIANT = 2; //int
    static constexpr std::int32_t COLOR = 3; //byte
    static constexpr std::int32_t NAMETAG = 4; //string
    static constexpr std::int32_t OWNER_EID = 5; //long
    static constexpr std::int32_t TARGET_EID = 6; //long
    static constexpr std::int32_t AIR = 7; //short
    static constexpr std::int32_t POTION_COLOR = 8; //int (ARGB!)
    static constexpr std::int32_t POTION_AMBIENT = 9; //byte
    static constexpr std::int32_t JUMP_DURATION = 10; //byte
    static constexpr std::int32_t HURT_TIME = 11; //int (minecart/boat)
    static constexpr std::int32_t HURT_DIRECTION = 12; //int (minecart/boat)
    static constexpr std::int32_t PADDLE_TIME_LEFT = 13; //float
    static constexpr std::int32_t PADDLE_TIME_RIGHT = 14; //float
    static constexpr std::int32_t EXPERIENCE_VALUE = 15; //int (xp orb)
    static constexpr std::int32_t MINECART_DISPLAY_BLOCK = 16; //int (block runtime ID)
    static constexpr std::int32_t HORSE_FLAGS = 16; //int
    static constexpr std::int32_t FIREWORK_ITEM = 16; //compoundtag
    /* 16 (byte) used by wither skull */
    static constexpr std::int32_t MINECART_DISPLAY_OFFSET = 17; //int
    static constexpr std::int32_t SHOOTER_ID = 17; //long (used by arrows)
    static constexpr std::int32_t MINECART_HAS_DISPLAY = 18; //byte (must be 1 for minecart to show block inside)
    static constexpr std::int32_t HORSE_TYPE = 19; //byte
    static constexpr std::int32_t CREEPER_SWELL = 19; //int
    static constexpr std::int32_t CREEPER_SWELL_PREVIOUS = 20; //int
    static constexpr std::int32_t CREEPER_SWELL_DIRECTION = 21; //byte
    static constexpr std::int32_t CHARGE_AMOUNT = 22; //int8, used for ghasts and also crossbow charging
    static constexpr std::int32_t ENDERMAN_HELD_ITEM_ID = 23; //short
    static constexpr std::int32_t ENTITY_AGE = 24; //short
    /* 25 (int) used by horse, (byte) used by witch */
    static constexpr std::int32_t PLAYER_FLAGS = 26; //byte
    static constexpr std::int32_t PLAYER_INDEX = 27; //int, used for marker colours and agent nametag colours
    static constexpr std::int32_t PLAYER_BED_POSITION = 28; //blockpos
    static constexpr std::int32_t FIREBALL_POWER_X = 29; //float
    static constexpr std::int32_t FIREBALL_POWER_Y = 30;
    static constexpr std::int32_t FIREBALL_POWER_Z = 31;
    /* 32 (unknown) */
    static constexpr std::int32_t FISH_X = 33; //float
    static constexpr std::int32_t FISH_Z = 34; //float
    static constexpr std::int32_t FISH_ANGLE = 35; //float
    static constexpr std::int32_t POTION_AUX_VALUE = 36; //short
    static constexpr std::int32_t LEAD_HOLDER_EID = 37; //long
    static constexpr std::int32_t SCALE = 38; //float
    static constexpr std::int32_t HAS_NPC_COMPONENT = 39; //byte (???)
    static constexpr std::int32_t NPC_SKIN_INDEX = 40; //string
    static constexpr std::int32_t NPC_ACTIONS = 41; //string (maybe JSON blob?)
    static constexpr std::int32_t MAX_AIR = 42; //short
    static constexpr std::int32_t MARK_VARIANT = 43; //int
    static constexpr std::int32_t CONTAINER_TYPE = 44; //byte (ContainerComponent)
    static constexpr std::int32_t CONTAINER_BASE_SIZE = 45; //int (ContainerComponent)
    static constexpr std::int32_t CONTAINER_EXTRA_SLOTS_PER_STRENGTH = 46; //int (used for llamas, inventory size is baseSize + thisProp * strength)
    static constexpr std::int32_t BLOCK_TARGET = 47; //block coords (ender crystal)
    static constexpr std::int32_t WITHER_INVULNERABLE_TICKS = 48; //int
    static constexpr std::int32_t WITHER_TARGET_1 = 49; //long
    static constexpr std::int32_t WITHER_TARGET_2 = 50; //long
    static constexpr std::int32_t WITHER_TARGET_3 = 51; //long
    static constexpr std::int32_t WITHER_AERIAL_ATTACK = 52; //short
    static constexpr std::int32_t BOUNDING_BOX_WIDTH = 53; //float
    static constexpr std::int32_t BOUNDING_BOX_HEIGHT = 54; //float
    static constexpr std::int32_t FUSE_LENGTH = 55; //int
    static constexpr std::int32_t RIDER_SEAT_POSITION = 56; //vector3f
    static constexpr std::int32_t RIDER_ROTATION_LOCKED = 57; //byte
    static constexpr std::int32_t RIDER_MAX_ROTATION = 58; //float
    static constexpr std::int32_t RIDER_MIN_ROTATION = 59; //float
    static constexpr std::int32_t RIDER_SEAT_ROTATION_OFFSET = 60; //TODO: find type
    static constexpr std::int32_t AREA_EFFECT_CLOUD_RADIUS = 61; //float
    static constexpr std::int32_t AREA_EFFECT_CLOUD_WAITING = 62; //int
    static constexpr std::int32_t AREA_EFFECT_CLOUD_PARTICLE_ID = 63; //int
    static constexpr std::int32_t SHULKER_PEEK_ID = 64; //int
    static constexpr std::int32_t SHULKER_ATTACH_FACE = 65; //byte
    static constexpr std::int32_t SHULKER_ATTACHED = 66; //byte (TODO: check this - comment said it was a short)
    static constexpr std::int32_t SHULKER_ATTACH_POS = 67; //block coords
    static constexpr std::int32_t TRADING_PLAYER_EID = 68; //long
    static constexpr std::int32_t CAREER = 69; //int
    static constexpr std::int32_t HAS_COMMAND_BLOCK = 70; //byte
    static constexpr std::int32_t COMMAND_BLOCK_COMMAND = 71; //string
    static constexpr std::int32_t COMMAND_BLOCK_LAST_OUTPUT = 72; //string
    static constexpr std::int32_t COMMAND_BLOCK_TRACK_OUTPUT = 73; //byte
    static constexpr std::int32_t CONTROLLING_RIDER_SEAT_NUMBER = 74; //byte
    static constexpr std::int32_t STRENGTH = 75; //int
    static constexpr std::int32_t MAX_STRENGTH = 76; //int
    static constexpr std::int32_t EVOKER_SPELL_CASTING_COLOR = 77; //int
    static constexpr std::int32_t LIMITED_LIFE = 78;
    static constexpr std::int32_t ARMOR_STAND_POSE_INDEX = 79; //int
    static constexpr std::int32_t ENDER_CRYSTAL_TIME_OFFSET = 80; //int
    static constexpr std::int32_t ALWAYS_SHOW_NAMETAG = 81; //byte: -1 = default, 0 = only when looked at, 1 = always
    static constexpr std::int32_t COLOR_2 = 82; //byte
    static constexpr std::int32_t NAME_AUTHOR = 83; //string
    static constexpr std::int32_t SCORE_TAG = 84; //string
    static constexpr std::int32_t BALLOON_ATTACHED_ENTITY = 85; //int64, entity unique ID of owner
    static constexpr std::int32_t PUFFERFISH_SIZE = 86; //byte
    static constexpr std::int32_t BOAT_BUBBLE_TIME = 87; //int (time in bubble column)
    static constexpr std::int32_t PLAYER_AGENT_EID = 88; //long
    static constexpr std::int32_t SITTING_AMOUNT = 89; //float
    static constexpr std::int32_t SITTING_AMOUNT_PREVIOUS = 90; //float
    static constexpr std::int32_t EAT_COUNTER = 91; //int (used by pandas)
    static constexpr std::int32_t FLAGS2 = 92; //long (extended data flags)
    static constexpr std::int32_t LAYING_AMOUNT = 93; //float (used by pandas)
    static constexpr std::int32_t LAYING_AMOUNT_PREVIOUS = 94; //float (used by pandas)
    static constexpr std::int32_t AREA_EFFECT_CLOUD_DURATION = 95; //int
    static constexpr std::int32_t AREA_EFFECT_CLOUD_SPAWN_TIME = 96; //int
    static constexpr std::int32_t AREA_EFFECT_CLOUD_RADIUS_PER_TICK = 97; //float, usually negative
    static constexpr std::int32_t AREA_EFFECT_CLOUD_RADIUS_CHANGE_ON_PICKUP = 98; //float
    static constexpr std::int32_t AREA_EFFECT_CLOUD_PICKUP_COUNT = 99; //int
    static constexpr std::int32_t INTERACTIVE_TAG = 100; //string (button text)
    static constexpr std::int32_t TRADE_TIER = 101; //int
    static constexpr std::int32_t MAX_TRADE_TIER = 102; //int
    static constexpr std::int32_t TRADE_XP = 103; //int
    static constexpr std::int32_t SKIN_ID = 104; //int ???
    static constexpr std::int32_t SPAWNING_FRAMES = 105; //int - related to wither
    static constexpr std::int32_t COMMAND_BLOCK_TICK_DELAY = 106; //int
    static constexpr std::int32_t COMMAND_BLOCK_EXECUTE_ON_FIRST_TICK = 107; //byte
    static constexpr std::int32_t AMBIENT_SOUND_INTERVAL_MIN = 108; //float
    static constexpr std::int32_t AMBIENT_SOUND_INTERVAL_RANGE = 109; //float
    static constexpr std::int32_t AMBIENT_SOUND_EVENT = 110; //string
    static constexpr std::int32_t FALL_DAMAGE_MULTIPLIER = 111; //float
    static constexpr std::int32_t NAME_RAW_TEXT = 112; //string
    static constexpr std::int32_t CAN_RIDE_TARGET = 113; //byte
    static constexpr std::int32_t LOW_TIER_CURED_TRADE_DISCOUNT = 114; //int
    static constexpr std::int32_t HIGH_TIER_CURED_TRADE_DISCOUNT = 115; //int
    static constexpr std::int32_t NEARBY_CURED_TRADE_DISCOUNT = 116; //int
    static constexpr std::int32_t NEARBY_CURED_DISCOUNT_TIME_STAMP = 117; //int
    static constexpr std::int32_t HITBOX = 118; //compound
    static constexpr std::int32_t IS_BUOYANT = 119; //byte
    static constexpr std::int32_t FREEZING_EFFECT_STRENGTH = 120; //float
    static constexpr std::int32_t BUOYANCY_DATA = 121; //string
    static constexpr std::int32_t GOAT_HORN_COUNT = 122; //int
    static constexpr std::int32_t BASE_RUNTIME_ID = 123; //string
    static constexpr std::int32_t MOVEMENT_SOUND_DISTANCE_OFFSET = 124;
    static constexpr std::int32_t HEARTBEAT_INTERVAL_TICKS = 125; //int
    static constexpr std::int32_t HEARTBEAT_LEVEL_SOUND_EVENT = 126; //int
    static constexpr std::int32_t PLAYER_DEATH_POSITION = 127; //blockpos
    static constexpr std::int32_t PLAYER_DEATH_DIMENSION = 128; //int
    static constexpr std::int32_t PLAYER_HAS_DIED = 129; //byte
    static constexpr std::int32_t COLLISION_BOX = 130; //compound
    static constexpr std::int32_t VISIBLE_MOB_EFFECTS = 131; //long (first 8 visible effects packed, 6 bits for effectId + 1 bit for ambient indicator)
    static constexpr std::int32_t FILTERED_NAME = 132; //string
    static constexpr std::int32_t BED_ENTER_POSITION = 133; //blockpos
    static constexpr std::int32_t SEAT_THIRD_PERSON_CAMERA_RADIUS = 134;
    static constexpr std::int32_t SEAT_CAMERA_RELAX_DISTANCE_SMOOTHING = 135;
    static constexpr std::int32_t AIM_ASSIST_PRIORITY_PRESET_ID = 136; //int
    static constexpr std::int32_t AIM_ASSIST_PRIORITY_CATEGORY_ID = 137; //int
    static constexpr std::int32_t AIM_ASSIST_PRIORITY_ACTOR_ID = 138; //int
    static constexpr std::int32_t RESERVED_139 = 139; //long
    static constexpr std::int32_t NAMEPLATE_RENDER_DISTANCE_MAX = 140; //float
};

}  // namespace bedrock_protocol::types::entity
