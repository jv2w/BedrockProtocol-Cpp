/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/entity/EntityLink.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::entity {

class EntityLink {
public:
    static constexpr std::uint8_t TYPE_REMOVE = 0;
    static constexpr std::uint8_t TYPE_RIDER = 1;
    static constexpr std::uint8_t TYPE_PASSENGER = 2;

    /** Not present in the PHP original; packets must be default-constructible for PacketPool. */
    EntityLink() = default;

    EntityLink(std::int64_t fromActorUniqueId, std::int64_t toActorUniqueId, std::uint8_t type, bool immediate,
               bool causedByRider, float vehicleAngularVelocity)
        : fromActorUniqueId(fromActorUniqueId), toActorUniqueId(toActorUniqueId), type(type), immediate(immediate),
          causedByRider(causedByRider), vehicleAngularVelocity(vehicleAngularVelocity)
    {
    }

    std::int64_t fromActorUniqueId = 0;
    std::int64_t toActorUniqueId = 0;
    std::uint8_t type = 0;
    bool immediate = false;
    bool causedByRider = false;
    float vehicleAngularVelocity = 0.0f;
};

}  // namespace bedrock_protocol::types::entity
