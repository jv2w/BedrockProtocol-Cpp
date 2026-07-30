/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/ShowStoreOfferRedirectType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types {

enum class ShowStoreOfferRedirectType : std::int32_t {
    MARKETPLACE = 0,
    DRESSING_ROOM = 1,
    THIRD_PARTY_SERVER_PAGE = 2,
};

/**
 * @throws PacketDecodeException
 */
ShowStoreOfferRedirectType ShowStoreOfferRedirectTypeFromPacket(std::int32_t value);

}  // namespace bedrock_protocol::types
