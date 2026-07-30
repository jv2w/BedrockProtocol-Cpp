/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ScreenCloseReason.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string_view>

namespace bedrock_protocol::types {

/**
 * @see ServerboundDataDrivenScreenClosedPacket
 */
class ScreenCloseReason final {
    ScreenCloseReason() = delete;

public:
    static constexpr std::string_view PROGRAMMATIC_CLOSE = "programmaticclose";
    static constexpr std::string_view PROGRAMMATIC_CLOSE_ALL = "programmaticcloseall";
    static constexpr std::string_view CLIENT_CANCELED = "clientcanceled";
    static constexpr std::string_view USER_BUSY = "userbusy";
    static constexpr std::string_view INVALID_FORM = "invalidform";
};

}  // namespace bedrock_protocol::types
