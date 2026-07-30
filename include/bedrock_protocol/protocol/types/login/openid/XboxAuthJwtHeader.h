/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/openid/XboxAuthJwtHeader.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>

namespace bedrock_protocol::types::login::openid {

class XboxAuthJwtHeader final {
public:
    /** @required */
    std::string alg;

    /** @required */
    std::string kid;

    /** @required */
    std::string typ;
};

}  // namespace bedrock_protocol::types::login::openid
