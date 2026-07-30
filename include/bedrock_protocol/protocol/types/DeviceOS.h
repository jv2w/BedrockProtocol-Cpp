/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/DeviceOS.php
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
 * Device operating system identifiers.
 *
 * @note One deviation from PHP: the constant PHP calls WIN32 is OsWIN32 here. <windows.h> defines
 *       WIN32 as an object-like macro, so the original name would be substituted away at every use
 *       site. Renaming is preferable to #undef, which would silently leak into every header
 *       included afterwards.
 */
class DeviceOS final {
public:
    DeviceOS() = delete;

    static constexpr std::int32_t UNKNOWN = -1;
    static constexpr std::int32_t ANDROID = 1;
    static constexpr std::int32_t IOS = 2;
    static constexpr std::int32_t OSX = 3;
    static constexpr std::int32_t AMAZON = 4;
    static constexpr std::int32_t GEAR_VR = 5;
    static constexpr std::int32_t HOLOLENS = 6;
    static constexpr std::int32_t WINDOWS_10 = 7;
    static constexpr std::int32_t OsWIN32 = 8;  // PHP: WIN32
    static constexpr std::int32_t DEDICATED = 9;
    static constexpr std::int32_t TVOS = 10;
    static constexpr std::int32_t PLAYSTATION = 11;
    static constexpr std::int32_t NINTENDO = 12;
    static constexpr std::int32_t XBOX = 13;
    static constexpr std::int32_t WINDOWS_PHONE = 14;
    static constexpr std::int32_t LINUX = 15;
};

}  // namespace bedrock_protocol::types
