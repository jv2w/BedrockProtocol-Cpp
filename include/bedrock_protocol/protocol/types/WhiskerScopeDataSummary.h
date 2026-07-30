/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/WhiskerScopeDataSummary.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class WhiskerScopeDataSummary final {
public:
    WhiskerScopeDataSummary(std::string label, std::string indentation, std::uint64_t totalHighCostNS,
                            std::uint64_t totalMidCostNS, std::uint64_t totalLowCostNS)
        : label(std::move(label)), indentation(std::move(indentation)), totalHighCostNS(totalHighCostNS),
          totalMidCostNS(totalMidCostNS), totalLowCostNS(totalLowCostNS)
    {
    }

    [[nodiscard]] const std::string &getLabel() const { return label; }

    [[nodiscard]] const std::string &getIndentation() const { return indentation; }

    [[nodiscard]] std::uint64_t getTotalHighCostNS() const { return totalHighCostNS; }

    [[nodiscard]] std::uint64_t getTotalMidCostNS() const { return totalMidCostNS; }

    [[nodiscard]] std::uint64_t getTotalLowCostNS() const { return totalLowCostNS; }

    static WhiskerScopeDataSummary read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string label;
    std::string indentation;
    std::uint64_t totalHighCostNS;
    std::uint64_t totalMidCostNS;
    std::uint64_t totalLowCostNS;
};

}  // namespace bedrock_protocol::types
