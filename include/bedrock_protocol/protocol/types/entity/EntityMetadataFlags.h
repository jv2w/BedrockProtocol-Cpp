/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/EntityMetadataFlags.php
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

class EntityMetadataFlags final {
    EntityMetadataFlags() = delete;

public:
    static constexpr std::int32_t ONFIRE = 0;
    static constexpr std::int32_t SNEAKING = 1;
    static constexpr std::int32_t RIDING = 2;
    static constexpr std::int32_t SPRINTING = 3;
    static constexpr std::int32_t ACTION = 4;
    static constexpr std::int32_t INVISIBLE = 5;
    static constexpr std::int32_t TEMPTED = 6;
    static constexpr std::int32_t INLOVE = 7;
    static constexpr std::int32_t SADDLED = 8;
    static constexpr std::int32_t POWERED = 9;
    static constexpr std::int32_t IGNITED = 10;
    static constexpr std::int32_t BABY = 11;
    static constexpr std::int32_t CONVERTING = 12;
    static constexpr std::int32_t CRITICAL = 13;
    static constexpr std::int32_t CAN_SHOW_NAMETAG = 14;
    static constexpr std::int32_t ALWAYS_SHOW_NAMETAG = 15;
    static constexpr std::int32_t IMMOBILE = 16, NO_AI = 16;
    static constexpr std::int32_t SILENT = 17;
    static constexpr std::int32_t WALLCLIMBING = 18;
    static constexpr std::int32_t CAN_CLIMB = 19;
    static constexpr std::int32_t SWIMMER = 20;
    static constexpr std::int32_t CAN_FLY = 21;
    static constexpr std::int32_t WALKER = 22;
    static constexpr std::int32_t RESTING = 23;
    static constexpr std::int32_t SITTING = 24;
    static constexpr std::int32_t ANGRY = 25;
    static constexpr std::int32_t INTERESTED = 26;
    static constexpr std::int32_t CHARGED = 27;
    static constexpr std::int32_t TAMED = 28;
    static constexpr std::int32_t ORPHANED = 29;
    static constexpr std::int32_t LEASHED = 30;
    static constexpr std::int32_t SHEARED = 31;
    static constexpr std::int32_t GLIDING = 32;
    static constexpr std::int32_t ELDER = 33;
    static constexpr std::int32_t MOVING = 34;
    static constexpr std::int32_t BREATHING = 35;
    static constexpr std::int32_t CHESTED = 36;
    static constexpr std::int32_t STACKABLE = 37;
    static constexpr std::int32_t SHOWBASE = 38;
    static constexpr std::int32_t REARING = 39;
    static constexpr std::int32_t VIBRATING = 40;
    static constexpr std::int32_t IDLING = 41;
    static constexpr std::int32_t EVOKER_SPELL = 42;
    static constexpr std::int32_t CHARGE_ATTACK = 43;
    static constexpr std::int32_t WASD_CONTROLLED = 44;
    static constexpr std::int32_t CAN_POWER_JUMP = 45;
    static constexpr std::int32_t CAN_DASH = 46;
    static constexpr std::int32_t LINGER = 47;
    static constexpr std::int32_t HAS_COLLISION = 48;
    static constexpr std::int32_t AFFECTED_BY_GRAVITY = 49;
    static constexpr std::int32_t FIRE_IMMUNE = 50;
    static constexpr std::int32_t DANCING = 51;
    static constexpr std::int32_t ENCHANTED = 52;
    static constexpr std::int32_t SHOW_TRIDENT_ROPE = 53; // tridents show an animated rope when enchanted with loyalty after they are thrown and return to their owner. To be combined with DATA_OWNER_EID
    static constexpr std::int32_t CONTAINER_PRIVATE = 54; //inventory is private, doesn't drop contents when killed if true
    static constexpr std::int32_t TRANSFORMING = 55;
    static constexpr std::int32_t SPIN_ATTACK = 56;
    static constexpr std::int32_t SWIMMING = 57;
    static constexpr std::int32_t BRIBED = 58; //dolphins have this set when they go to find treasure for the player
    static constexpr std::int32_t PREGNANT = 59;
    static constexpr std::int32_t LAYING_EGG = 60;
    static constexpr std::int32_t RIDER_CAN_PICK = 61; //???
    static constexpr std::int32_t TRANSITION_SITTING = 62;
    static constexpr std::int32_t EATING = 63;
    static constexpr std::int32_t LAYING_DOWN = 64;
    static constexpr std::int32_t SNEEZING = 65;
    static constexpr std::int32_t TRUSTING = 66;
    static constexpr std::int32_t ROLLING = 67;
    static constexpr std::int32_t SCARED = 68;
    static constexpr std::int32_t IN_SCAFFOLDING = 69;
    static constexpr std::int32_t OVER_SCAFFOLDING = 70;
    static constexpr std::int32_t FALL_THROUGH_SCAFFOLDING = 71;
    static constexpr std::int32_t BLOCKING = 72; //shield
    static constexpr std::int32_t TRANSITION_BLOCKING = 73;
    static constexpr std::int32_t BLOCKED_USING_SHIELD = 74;
    static constexpr std::int32_t BLOCKED_USING_DAMAGED_SHIELD = 75;
    static constexpr std::int32_t SLEEPING = 76;
    static constexpr std::int32_t WANTS_TO_WAKE = 77;
    static constexpr std::int32_t TRADE_INTEREST = 78;
    static constexpr std::int32_t DOOR_BREAKER = 79; //...
    static constexpr std::int32_t BREAKING_OBSTRUCTION = 80;
    static constexpr std::int32_t DOOR_OPENER = 81; //...
    static constexpr std::int32_t ILLAGER_CAPTAIN = 82;
    static constexpr std::int32_t STUNNED = 83;
    static constexpr std::int32_t ROARING = 84;
    static constexpr std::int32_t DELAYED_ATTACKING = 85;
    static constexpr std::int32_t AVOIDING_MOBS = 86;
    static constexpr std::int32_t AVOIDING_BLOCK = 87;
    static constexpr std::int32_t FACING_TARGET_TO_RANGE_ATTACK = 88;
    static constexpr std::int32_t HIDDEN_WHEN_INVISIBLE = 89; //??????????????????
    static constexpr std::int32_t IS_IN_UI = 90;
    static constexpr std::int32_t STALKING = 91;
    static constexpr std::int32_t EMOTING = 92;
    static constexpr std::int32_t CELEBRATING = 93;
    static constexpr std::int32_t ADMIRING = 94;
    static constexpr std::int32_t CELEBRATING_SPECIAL = 95;
    static constexpr std::int32_t OUT_OF_CONTROL = 96;
    static constexpr std::int32_t RAM_ATTACK = 97;
    static constexpr std::int32_t PLAYING_DEAD = 98;
    static constexpr std::int32_t IN_ASCENDABLE_BLOCK = 99;
    static constexpr std::int32_t OVER_DESCENDABLE_BLOCK = 100;
    static constexpr std::int32_t CROAKING = 101;
    static constexpr std::int32_t EAT_MOB = 102;
    static constexpr std::int32_t JUMP_GOAL_JUMP = 103;
    static constexpr std::int32_t EMERGING = 104;
    static constexpr std::int32_t SNIFFING = 105;
    static constexpr std::int32_t DIGGING = 106;
    static constexpr std::int32_t SONIC_BOOM = 107;
    static constexpr std::int32_t HAS_DASH_COOLDOWN = 108;
    static constexpr std::int32_t PUSH_TOWARDS_CLOSEST_SPACE = 109;
    static constexpr std::int32_t SCENTING = 110;
    static constexpr std::int32_t RISING = 111;
    static constexpr std::int32_t HAPPY = 112;
    static constexpr std::int32_t SEARCHING = 113;
    static constexpr std::int32_t CRAWLING = 114;
    static constexpr std::int32_t TIMER_FLAG_1 = 115;
    static constexpr std::int32_t TIMER_FLAG_2 = 116;
    static constexpr std::int32_t TIMER_FLAG_3 = 117;
    static constexpr std::int32_t BODY_ROTATION_BLOCKED = 118;
    static constexpr std::int32_t RENDER_WHEN_INVISIBLE = 119;
    static constexpr std::int32_t ROTATION_AXIS_ALIGNED = 120;
    static constexpr std::int32_t COLLIDABLE = 121;
    static constexpr std::int32_t WASD_FREE_CAMERA_CONTROLLED = 122;
    static constexpr std::int32_t DOES_SERVER_AUTH_ONLY_DISMOUNT = 123;
    static constexpr std::int32_t BODY_ROTATION_ALWAYS_FOLLOWS_HEAD = 124;
    static constexpr std::int32_t CAN_USE_VERTICAL_MOVEMENT_ACTION = 125;
    static constexpr std::int32_t ROTATION_LOCKED_TO_VEHICLE = 126;
    static constexpr std::int32_t USES_LEGACY_FRICTION = 127;

    static constexpr std::int32_t NUMBER_OF_FLAGS = 128;
};

}  // namespace bedrock_protocol::types::entity
