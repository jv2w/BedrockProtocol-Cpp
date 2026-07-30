/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/SelfSignedJwtHeader.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>

namespace bedrock_protocol::types::login {

/**
 * JsonMapper model for headers of JWTs used in self-signed authentication and for the client data JWT header.
 */
class SelfSignedJwtHeader final {
public:
    /** @required */
    std::string alg;
    /** @required */
    std::string x5u;

    /**
     * As of 2023-03-29, this field suddenly started appearing in JWTs returned by the Mojang authentication API.
     * It's unclear whether this was intended, but it is part of the JWT spec, so it's not a problem to accept it.
     */
    std::string x5t;
};

}  // namespace bedrock_protocol::types::login
