/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerAuthInputFlags.php
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

/**
 * These flags are used in PlayerAuthInputPacket's inputFlags field.
 * The flags should be written as
 * `flags |= (1 << flag)`
 * and read as
 * `(flags & (1 << flag)) !== 0`
 *
 * @see PlayerAuthInputPacket
 */
class PlayerAuthInputFlags final {
    PlayerAuthInputFlags() = delete;

public:
    /** Pressing the "fly up" key when using touch. */
    static constexpr std::int32_t ASCEND = 0;
    /** Pressing the "fly down" key when using touch. */
    static constexpr std::int32_t DESCEND = 1;
    /** Pressing (and optionally holding) the jump key (while not flying). */
    static constexpr std::int32_t NORTH_JUMP = 2;
    /** Pressing (and optionally holding) the jump key (including while flying). */
    static constexpr std::int32_t JUMP_DOWN = 3;
    /** Pressing (and optionally holding) the sprint key (typically the CTRL key). Does not include double-pressing the forward key. */
    static constexpr std::int32_t SPRINT_DOWN = 4;
    /** Pressing (and optionally holding) the fly button ONCE when in flight mode when using touch. This has no obvious use. */
    static constexpr std::int32_t CHANGE_HEIGHT = 5;
    /** Pressing (and optionally holding) the jump key (including while flying), and also auto-jumping. */
    static constexpr std::int32_t JUMPING = 6;
    /** Auto-swimming upwards while pressing forwards with auto-jump enabled. */
    static constexpr std::int32_t AUTO_JUMPING_IN_WATER = 7;
    /** Sneaking, and pressing the "fly down" key or "sneak" key (including while flying). */
    static constexpr std::int32_t SNEAKING = 8;
    /** Pressing (and optionally holding) the sneak key (including while flying). This includes when the sneak button is toggled ON with touch controls. */
    static constexpr std::int32_t SNEAK_DOWN = 9;
    /** Pressing the forward key (typically W on keyboard). */
    static constexpr std::int32_t UP = 10;
    /** Pressing the backward key (typically S on keyboard). */
    static constexpr std::int32_t DOWN = 11;
    /** Pressing the left key (typically A on keyboard). */
    static constexpr std::int32_t LEFT = 12;
    /** Pressing the right key (typically D on keyboard). */
    static constexpr std::int32_t RIGHT = 13;
    /** Pressing the ↖ key on touch. */
    static constexpr std::int32_t UP_LEFT = 14;
    /** Pressing the ↗ key on touch. */
    static constexpr std::int32_t UP_RIGHT = 15;
    /** Client wants to go upwards. Sent when Ascend or Jump is pressed, irrespective of whether flight is enabled. */
    static constexpr std::int32_t WANT_UP = 16;
    /** Client wants to go downwards. Sent when Descend or Sneak is pressed, irrespective of whether flight is enabled. */
    static constexpr std::int32_t WANT_DOWN = 17;
    /** Same as "want up" but slow. Only usable with controllers at the time of writing. Triggered by pressing the right joystick by default. */
    static constexpr std::int32_t WANT_DOWN_SLOW = 18;
    /** Same as "want down" but slow. Only usable with controllers at the time of writing. Not bound to any control by default. */
    static constexpr std::int32_t WANT_UP_SLOW = 19;
    /** Unclear usage, during testing it was only seen in conjunction with SPRINT_DOWN. NOT sent while actually sprinting. */
    static constexpr std::int32_t SPRINTING = 20;
    /** Ascending scaffolding. Note that this is NOT sent when climbing ladders. */
    static constexpr std::int32_t ASCEND_BLOCK = 21;
    /** Descending scaffolding. */
    static constexpr std::int32_t DESCEND_BLOCK = 22;
    /** Toggling the sneak button on touch when the button enters the "enabled" state. */
    static constexpr std::int32_t SNEAK_TOGGLE_DOWN = 23;
    /** Unclear use. Sent continually on touch controls, irrespective of whether the player is actually sneaking or not. */
    static constexpr std::int32_t PERSIST_SNEAK = 24;
    static constexpr std::int32_t START_SPRINTING = 25;
    static constexpr std::int32_t STOP_SPRINTING = 26;
    static constexpr std::int32_t START_SNEAKING = 27;
    static constexpr std::int32_t STOP_SNEAKING = 28;
    static constexpr std::int32_t START_SWIMMING = 29;
    static constexpr std::int32_t STOP_SWIMMING = 30;
    /** Initiating a new jump. Sent every time the client leaves the ground due to jumping, including auto jumps. */
    static constexpr std::int32_t START_JUMPING = 31;
    static constexpr std::int32_t START_GLIDING = 32;
    static constexpr std::int32_t STOP_GLIDING = 33;
    static constexpr std::int32_t PERFORM_ITEM_INTERACTION = 34;
    static constexpr std::int32_t PERFORM_BLOCK_ACTIONS = 35;
    static constexpr std::int32_t PERFORM_ITEM_STACK_REQUEST = 36;
    static constexpr std::int32_t HANDLED_TELEPORT = 37;
    static constexpr std::int32_t EMOTING = 38;
    /** Left-clicking the air. In vanilla, this generates an ATTACK_NODAMAGE sound and does nothing else. */
    static constexpr std::int32_t MISSED_SWING = 39;
    static constexpr std::int32_t START_CRAWLING = 40;
    static constexpr std::int32_t STOP_CRAWLING = 41;
    static constexpr std::int32_t START_FLYING = 42;
    static constexpr std::int32_t STOP_FLYING = 43;
    static constexpr std::int32_t ACK_ACTOR_DATA = 44;
    static constexpr std::int32_t IN_CLIENT_PREDICTED_VEHICLE = 45;
    static constexpr std::int32_t PADDLING_LEFT = 46;
    static constexpr std::int32_t PADDLING_RIGHT = 47;
    static constexpr std::int32_t BLOCK_BREAKING_DELAY_ENABLED = 48;
    static constexpr std::int32_t HORIZONTAL_COLLISION = 49;
    static constexpr std::int32_t VERTICAL_COLLISION = 50;
    static constexpr std::int32_t DOWN_LEFT = 51;
    static constexpr std::int32_t DOWN_RIGHT = 52;
    static constexpr std::int32_t START_USING_ITEM = 53;
    static constexpr std::int32_t IS_CAMERA_RELATIVE_MOVEMENT_ENABLED = 54;
    static constexpr std::int32_t IS_ROT_CONTROLLED_BY_MOVE_DIRECTION = 55;
    static constexpr std::int32_t START_SPIN_ATTACK = 56;
    static constexpr std::int32_t STOP_SPIN_ATTACK = 57;
    static constexpr std::int32_t IS_HOTBAR_ONLY_TOUCH = 58;
    static constexpr std::int32_t JUMP_RELEASED_RAW = 59;
    static constexpr std::int32_t JUMP_PRESSED_RAW = 60;
    static constexpr std::int32_t JUMP_CURRENT_RAW = 61;
    static constexpr std::int32_t SNEAK_RELEASED_RAW = 62;
    static constexpr std::int32_t SNEAK_PRESSED_RAW = 63;
    static constexpr std::int32_t SNEAK_CURRENT_RAW = 64;

    static constexpr std::int32_t NUMBER_OF_FLAGS = 65;
};

}  // namespace bedrock_protocol::types
