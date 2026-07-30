/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/AuthenticationInfo.php
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

namespace bedrock_protocol::types::login {

/**
 * Model class for LoginPacket JSON data for JsonMapper
 */
class AuthenticationInfo final {
public:
    /** @required */
    std::int32_t AuthenticationType = 0;

    std::string Certificate;

    /** @required */
    std::string Token;
};

}  // namespace bedrock_protocol::types::login
