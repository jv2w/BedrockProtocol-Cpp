/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/openid/SelfSignedJwtBody.php
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

#include "bedrock_protocol/protocol/types/login/JwtBodyRfc7519.h"

namespace bedrock_protocol::types::login::openid {

/**
 * Mostly the same as the xbox token but with some weird differences
 */
class SelfSignedJwtBody final : public JwtBodyRfc7519 {
public:
    /** @required */
    std::string cpk;  // the public key that was used to sign the "client properties" token

    /** @required */
    std::string leguuid;  // the client's chosen UUID

    /** @required */
    std::string xname;  // the player's chosen name, nothing to do with Xbox but shares the same property name

    /** @required */
    std::string mid;  // the player's Minecraft ID, identifying the player in Minecraft's PlayFab namespace

    std::int64_t ap = 0;  // ??

    //The following are not required for self-signed authentication, but seem to be present as empty strings in a
    //self-signed token for some reason

    std::string nid;
    std::string nname;

    std::string pid;
    std::string pname;

    std::string xid;
};

}  // namespace bedrock_protocol::types::login::openid
