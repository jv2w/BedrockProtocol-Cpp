/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PlayerAction.php
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

class PlayerAction final {
    PlayerAction()
    {
        //NOOP
    }

public:
    static constexpr std::int32_t START_BREAK = 0;
    static constexpr std::int32_t ABORT_BREAK = 1;
    static constexpr std::int32_t STOP_BREAK = 2;
    static constexpr std::int32_t GET_UPDATED_BLOCK = 3;
    static constexpr std::int32_t DROP_ITEM = 4;
    static constexpr std::int32_t START_SLEEPING = 5;
    static constexpr std::int32_t STOP_SLEEPING = 6;
    static constexpr std::int32_t RESPAWN = 7;
    static constexpr std::int32_t JUMP = 8;
    static constexpr std::int32_t START_SPRINT = 9;
    static constexpr std::int32_t STOP_SPRINT = 10;
    static constexpr std::int32_t START_SNEAK = 11;
    static constexpr std::int32_t STOP_SNEAK = 12;
    static constexpr std::int32_t CREATIVE_PLAYER_DESTROY_BLOCK = 13;
    static constexpr std::int32_t DIMENSION_CHANGE_ACK = 14; //sent when spawning in a different dimension to tell the server we spawned
    static constexpr std::int32_t START_GLIDE = 15;
    static constexpr std::int32_t STOP_GLIDE = 16;
    static constexpr std::int32_t BUILD_DENIED = 17;
    static constexpr std::int32_t CRACK_BLOCK = 18;
    static constexpr std::int32_t CHANGE_SKIN = 19;
    static constexpr std::int32_t SET_ENCHANTMENT_SEED = 20; //no longer used
    static constexpr std::int32_t START_SWIMMING = 21;
    static constexpr std::int32_t STOP_SWIMMING = 22;
    static constexpr std::int32_t START_SPIN_ATTACK = 23;
    static constexpr std::int32_t STOP_SPIN_ATTACK = 24;
    static constexpr std::int32_t INTERACT_BLOCK = 25;
    static constexpr std::int32_t PREDICT_DESTROY_BLOCK = 26;
    static constexpr std::int32_t CONTINUE_DESTROY_BLOCK = 27;
    static constexpr std::int32_t START_ITEM_USE_ON = 28;
    static constexpr std::int32_t STOP_ITEM_USE_ON = 29;
    static constexpr std::int32_t HANDLED_TELEPORT = 30;
    static constexpr std::int32_t MISSED_SWING = 31;
    static constexpr std::int32_t START_CRAWLING = 32;
    static constexpr std::int32_t STOP_CRAWLING = 33;
    static constexpr std::int32_t START_FLYING = 34;
    static constexpr std::int32_t STOP_FLYING = 35;

    static constexpr std::int32_t START_USING_ITEM = 37;

    //Backwards compatibility (blame @dktapps)
    static constexpr std::int32_t CRACK_BREAK = 18;
};

}  // namespace bedrock_protocol::types
