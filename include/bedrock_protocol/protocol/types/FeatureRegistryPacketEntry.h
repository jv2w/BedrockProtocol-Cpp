/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/FeatureRegistryPacketEntry.php
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

class FeatureRegistryPacketEntry final {
public:
    FeatureRegistryPacketEntry(std::string featureName, std::string featureJson)
        : featureName(std::move(featureName)), featureJson(std::move(featureJson))
    {
    }

    [[nodiscard]] const std::string &getFeatureName() const { return featureName; }

    [[nodiscard]] const std::string &getFeatureJson() const { return featureJson; }

    /** @throws DataDecodeException */
    static FeatureRegistryPacketEntry read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string featureName;
    std::string featureJson;
};

}  // namespace bedrock_protocol::types
