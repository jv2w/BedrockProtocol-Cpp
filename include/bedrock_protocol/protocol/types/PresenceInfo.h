/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/PresenceInfo.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

/**
 * @see ServerPresenceInfoPacket&ServerJoinInformation
 */
class PresenceInfo final {
public:
    PresenceInfo(std::optional<std::string> experienceName, std::optional<std::string> worldName,
                 std::string richPresenceId)
        : experienceName(std::move(experienceName)), worldName(std::move(worldName)),
          richPresenceId(std::move(richPresenceId))
    {
    }

    [[nodiscard]] const std::optional<std::string> &getExperienceName() const { return experienceName; }

    [[nodiscard]] const std::optional<std::string> &getWorldName() const { return worldName; }

    [[nodiscard]] const std::string &getRichPresenceId() const { return richPresenceId; }

    static PresenceInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::optional<std::string> experienceName;
    std::optional<std::string> worldName;
    std::string richPresenceId;
};

}  // namespace bedrock_protocol::types
