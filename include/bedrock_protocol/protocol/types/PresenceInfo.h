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
    explicit PresenceInfo(std::optional<std::string> richPresenceId)
        : richPresenceId(std::move(richPresenceId))
    {
    }

    [[nodiscard]] const std::optional<std::string> &getRichPresenceId() const { return richPresenceId; }

    static PresenceInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    /**
     * The only field this structure carries. Two further optional strings used to be read in front
     * of it, which desynchronised the stream: Endstone r26_u4 ServerConfiguration::PresenceConfiguration
     * lists richPresenceId alone, and gophertunnel v1.58.0 minecraft/protocol/server_join_information.go:59-61
     * writes nothing else. The field is optional on the wire, not a plain string.
     */
    std::optional<std::string> richPresenceId;
};

}  // namespace bedrock_protocol::types
