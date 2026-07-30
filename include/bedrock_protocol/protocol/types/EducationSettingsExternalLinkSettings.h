/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EducationSettingsExternalLinkSettings.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class EducationSettingsExternalLinkSettings final {
public:
    EducationSettingsExternalLinkSettings(std::string url, std::string displayName)
        : url(std::move(url)), displayName(std::move(displayName))
    {
    }

    [[nodiscard]] const std::string &getUrl() const { return url; }

    [[nodiscard]] const std::string &getDisplayName() const { return displayName; }

    /** @throws DataDecodeException */
    static EducationSettingsExternalLinkSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string url;
    std::string displayName;
};

}  // namespace bedrock_protocol::types
