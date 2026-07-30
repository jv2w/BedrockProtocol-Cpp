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

#include <optional>
#include <string>
#include <string_view>

#include "bedrock_protocol/protocol/types/login/clientdata/ClientData.h"
#include "bedrock_protocol/protocol/types/skin/SkinData.h"

namespace bedrock_protocol::types::login::clientdata {

namespace detail {
/**
 * Equivalent of PHP's base64_decode($base64, true): nullopt where PHP returns false.
 *
 * Declared here rather than kept internal to the .cpp so that the parity test can put it side by
 * side with PHP's own decoder. This is the only hand-written parser in the port that the packet
 * suites cannot reach, because it runs on the JWT payload rather than on the wire.
 */
std::optional<std::string> strictBase64Decode(std::string_view base64);
}  // namespace detail

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
