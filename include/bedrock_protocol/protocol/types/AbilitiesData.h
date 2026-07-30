/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AbilitiesData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AbilitiesLayer.h"

namespace bedrock_protocol::types {

class AbilitiesData final {
public:
    AbilitiesData(std::uint8_t commandPermission, std::uint8_t playerPermission, std::int64_t targetActorUniqueId,
                  std::vector<AbilitiesLayer> abilityLayers)
        : commandPermission(commandPermission), playerPermission(playerPermission),
          targetActorUniqueId(targetActorUniqueId), abilityLayers(std::move(abilityLayers))
    {
    }

    [[nodiscard]] std::uint8_t getCommandPermission() const { return commandPermission; }

    [[nodiscard]] std::uint8_t getPlayerPermission() const { return playerPermission; }

    [[nodiscard]] std::int64_t getTargetActorUniqueId() const { return targetActorUniqueId; }

    [[nodiscard]] const std::vector<AbilitiesLayer> &getAbilityLayers() const { return abilityLayers; }

    /**
     * @throws PacketDecodeException
     */
    static AbilitiesData decode(encoding::ByteBufferReader &in);

    void encode(encoding::ByteBufferWriter &out) const;

private:
    std::uint8_t commandPermission;
    std::uint8_t playerPermission;
    std::int64_t targetActorUniqueId;  //This is a little-endian long, NOT a var-long. (WTF Mojang)
    std::vector<AbilitiesLayer> abilityLayers;
};

}  // namespace bedrock_protocol::types
