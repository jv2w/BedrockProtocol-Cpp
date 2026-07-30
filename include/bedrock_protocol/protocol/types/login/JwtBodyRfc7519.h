/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/JwtBodyRfc7519.php
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
 * Model class for JsonMapper which describes the RFC7519 standard fields in a JWT. Any of these fields might not be
 * provided.
 */
class JwtBodyRfc7519 {
public:
    virtual ~JwtBodyRfc7519() = default;

    std::string iss;
    std::string sub;
    /**
     * @note PHP types this as `mixed` because RFC 7519 allows either a string or a list of strings.
     *       It is kept as the raw JSON text of the claim here, which is what the wire carries anyway.
     */
    std::string aud;
    std::int64_t exp = 0;
    std::int64_t nbf = 0;
    std::int64_t iat = 0;
    std::string jti;
};

}  // namespace bedrock_protocol::types::login
