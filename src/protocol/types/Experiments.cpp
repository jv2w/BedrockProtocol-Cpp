/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/Experiments.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/Experiments.h"

#include <cstdint>

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

Experiments Experiments::read(encoding::ByteBufferReader &in)
{
    std::vector<std::pair<std::string, bool>> experiments;
    for (std::uint32_t i = 0, len = LE::readUnsignedInt(in); i < len; ++i) {
        auto experimentName = CommonTypes::getString(in);
        const auto enabled = CommonTypes::getBool(in);
        experiments.emplace_back(std::move(experimentName), enabled);
    }
    const auto hasPreviouslyUsedExperiments = CommonTypes::getBool(in);
    return Experiments(std::move(experiments), hasPreviouslyUsedExperiments);
}

void Experiments::write(encoding::ByteBufferWriter &out) const
{
    LE::writeUnsignedInt(out, static_cast<std::uint32_t>(experiments.size()));
    for (const auto &[experimentName, enabled] : experiments) {
        CommonTypes::putString(out, experimentName);
        CommonTypes::putBool(out, enabled);
    }
    CommonTypes::putBool(out, hasPreviouslyUsedExperiments_);
}

}  // namespace bedrock_protocol::types
