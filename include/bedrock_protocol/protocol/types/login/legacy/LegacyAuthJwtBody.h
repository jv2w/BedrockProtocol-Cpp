/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/legacy/LegacyAuthJwtBody.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>

#include "bedrock_protocol/protocol/types/login/JwtBodyRfc7519.h"

namespace bedrock_protocol::types::login::legacy {

/**
 * Model for JsonMapper exposing the data in the login JWT chain links.
 * TODO: extend this with more complete models
 */
class LegacyAuthJwtBody final : public JwtBodyRfc7519 {
public:
    std::string identityPublicKey;
};

}  // namespace bedrock_protocol::types::login::legacy
