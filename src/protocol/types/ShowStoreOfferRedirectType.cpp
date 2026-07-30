/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ShowStoreOfferRedirectType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/ShowStoreOfferRedirectType.h"

#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types {

ShowStoreOfferRedirectType ShowStoreOfferRedirectTypeFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return ShowStoreOfferRedirectType::MARKETPLACE;
    case 1:
        return ShowStoreOfferRedirectType::DRESSING_ROOM;
    case 2:
        return ShowStoreOfferRedirectType::THIRD_PARTY_SERVER_PAGE;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for ShowStoreOfferRedirectType");
    }
}

}  // namespace bedrock_protocol::types
