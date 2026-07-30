/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/AuthenticationType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::login {

/**
 * @see AuthenticationInfo
 */
enum class AuthenticationType : std::int32_t {
    FULL = 0,
    GUEST = 1,
    SELF_SIGNED = 2,
};

}  // namespace bedrock_protocol::types::login
