/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/login/legacy/LegacyAuthIdentityData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <optional>
#include <string>

namespace bedrock_protocol::types::login::legacy {

/**
 * Model class for LoginPacket JSON data for JsonMapper
 */
class LegacyAuthIdentityData final {
public:
    /** @required */
    std::string displayName;

    /** @required */
    std::string identity;

    std::string sandboxId = "RETAIL";  //TODO: what are the other possible values?

    std::optional<std::string> titleId = "";  //TODO: find out what this is for

    /** @required */
    std::string XUID;
};

}  // namespace bedrock_protocol::types::login::legacy
