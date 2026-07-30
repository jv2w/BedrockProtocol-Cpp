/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/login/clientdata/ClientDataToSkinDataHelper.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <string_view>

#include "bedrock_protocol/protocol/types/login/clientdata/ClientData.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"

namespace bedrock_protocol::types::login::clientdata {

class ClientDataToSkinDataHelper final {
public:
    /**
     * @throws std::invalid_argument
     */
    static skin::SkinData fromClientData(const ClientData &clientData);

private:
    /**
     * @throws std::invalid_argument
     */
    static std::string safeB64Decode(std::string_view base64, std::string_view context);
};

}  // namespace bedrock_protocol::types::login::clientdata
