/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionEaseType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace bedrock_protocol::types::camera {

class CameraSetInstructionEaseType final {
private:
    CameraSetInstructionEaseType() = delete;
    //NOOP

public:
    static constexpr std::uint8_t LINEAR = 0;
    static constexpr std::uint8_t SPRING = 1;
    static constexpr std::uint8_t IN_QUAD = 2;
    static constexpr std::uint8_t OUT_QUAD = 3;
    static constexpr std::uint8_t IN_OUT_QUAD = 4;
    static constexpr std::uint8_t IN_CUBIC = 5;
    static constexpr std::uint8_t OUT_CUBIC = 6;
    static constexpr std::uint8_t IN_OUT_CUBIC = 7;
    static constexpr std::uint8_t IN_QUART = 8;
    static constexpr std::uint8_t OUT_QUART = 9;
    static constexpr std::uint8_t IN_OUT_QUART = 10;
    static constexpr std::uint8_t IN_QUINT = 11;
    static constexpr std::uint8_t OUT_QUINT = 12;
    static constexpr std::uint8_t IN_OUT_QUINT = 13;
    static constexpr std::uint8_t IN_SINE = 14;
    static constexpr std::uint8_t OUT_SINE = 15;
    static constexpr std::uint8_t IN_OUT_SINE = 16;
    static constexpr std::uint8_t IN_EXPO = 17;
    static constexpr std::uint8_t OUT_EXPO = 18;
    static constexpr std::uint8_t IN_OUT_EXPO = 19;
    static constexpr std::uint8_t IN_CIRC = 20;
    static constexpr std::uint8_t OUT_CIRC = 21;
    static constexpr std::uint8_t IN_OUT_CIRC = 22;
    static constexpr std::uint8_t IN_BOUNCE = 23;
    static constexpr std::uint8_t OUT_BOUNCE = 24;
    static constexpr std::uint8_t IN_OUT_BOUNCE = 25;
    static constexpr std::uint8_t IN_BACK = 26;
    static constexpr std::uint8_t OUT_BACK = 27;
    static constexpr std::uint8_t IN_OUT_BACK = 28;
    static constexpr std::uint8_t IN_ELASTIC = 29;
    static constexpr std::uint8_t OUT_ELASTIC = 30;
    static constexpr std::uint8_t IN_OUT_ELASTIC = 31;

    /**
     * @throws std::invalid_argument
     */
    static std::string toName(std::uint8_t value);

    /**
     * @throws std::invalid_argument
     */
    static std::uint8_t fromName(std::string_view name);
};

}  // namespace bedrock_protocol::types::camera
