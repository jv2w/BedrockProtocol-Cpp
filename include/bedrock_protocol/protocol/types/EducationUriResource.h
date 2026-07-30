/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EducationUriResource.php
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

class EducationUriResource final {
public:
    EducationUriResource(std::string buttonName, std::string linkUri)
        : buttonName(std::move(buttonName)), linkUri(std::move(linkUri))
    {
    }

    [[nodiscard]] const std::string &getButtonName() const { return buttonName; }

    [[nodiscard]] const std::string &getLinkUri() const { return linkUri; }

    /** @throws DataDecodeException */
    static EducationUriResource read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string buttonName;
    std::string linkUri;
};

}  // namespace bedrock_protocol::types
