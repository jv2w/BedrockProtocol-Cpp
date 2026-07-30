/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/LocatorBarWaypoint.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/protocol/types/WorldPosition.h"

namespace bedrock_protocol::types {

/**
 * @see LocatorBarWaypointPayload
 */
class LocatorBarWaypoint final {
public:
    LocatorBarWaypoint(std::uint32_t updateFlag, std::optional<bool> visible,
                       std::optional<WorldPosition> worldPosition, std::optional<std::string> texturePath,
                       std::optional<math::Vector2> iconSize, std::optional<color::Color> color,
                       std::optional<bool> clientPositionAuthority, std::optional<std::int64_t> actorUniqueId)
        : updateFlag(updateFlag), visible(visible), worldPosition(std::move(worldPosition)),
          texturePath(std::move(texturePath)), iconSize(iconSize), color(color),
          clientPositionAuthority(clientPositionAuthority), actorUniqueId(actorUniqueId)
    {
    }

    [[nodiscard]] std::uint32_t getUpdateFlag() const { return updateFlag; }

    [[nodiscard]] std::optional<bool> getVisible() const { return visible; }

    [[nodiscard]] const std::optional<WorldPosition> &getWorldPosition() const { return worldPosition; }

    [[nodiscard]] const std::optional<std::string> &getTexturePath() const { return texturePath; }

    [[nodiscard]] std::optional<math::Vector2> getIconSize() const { return iconSize; }

    [[nodiscard]] std::optional<color::Color> getColor() const { return color; }

    [[nodiscard]] std::optional<bool> getClientPositionAuthority() const { return clientPositionAuthority; }

    [[nodiscard]] std::optional<std::int64_t> getActorUniqueId() const { return actorUniqueId; }

    /**
     * @throws DataDecodeException
     */
    static LocatorBarWaypoint read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint32_t updateFlag;
    std::optional<bool> visible;
    std::optional<WorldPosition> worldPosition;
    std::optional<std::string> texturePath;
    std::optional<math::Vector2> iconSize;
    std::optional<color::Color> color;
    std::optional<bool> clientPositionAuthority;
    std::optional<std::int64_t> actorUniqueId;
};

}  // namespace bedrock_protocol::types
