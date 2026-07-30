/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/SystemDiagnosticTimingInfo.php
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

class SystemDiagnosticTimingInfo final {
public:
    SystemDiagnosticTimingInfo(std::string displayName, std::uint64_t systemIndex, std::uint64_t timeInNS,
                               std::uint8_t percentOfTotal)
        : displayName(std::move(displayName)), systemIndex(systemIndex), timeInNS(timeInNS),
          percentOfTotal(percentOfTotal)
    {
    }

    [[nodiscard]] const std::string &getDisplayName() const { return displayName; }

    [[nodiscard]] std::uint64_t getSystemIndex() const { return systemIndex; }

    [[nodiscard]] std::uint64_t getTimeInNS() const { return timeInNS; }

    [[nodiscard]] std::uint8_t getPercentOfTotal() const { return percentOfTotal; }

    static SystemDiagnosticTimingInfo read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string displayName;
    std::uint64_t systemIndex;
    std::uint64_t timeInNS;
    std::uint8_t percentOfTotal;
};

}  // namespace bedrock_protocol::types
