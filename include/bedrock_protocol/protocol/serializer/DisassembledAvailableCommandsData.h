/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/DisassembledAvailableCommandsData.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/types/command/ChainedSubCommandData.h"
#include "bedrock_protocol/protocol/types/command/CommandData.h"
#include "bedrock_protocol/protocol/types/command/CommandHardEnum.h"
#include "bedrock_protocol/protocol/types/command/CommandSoftEnum.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandEnumConstraintRawData.h"

namespace bedrock_protocol::serializer {

/**
 * High-level commands data with all the info properly linked (no nasty offsets).
 *
 * @note PHP represents the `unused*` collections as int-keyed arrays whose keys are the original
 * indexes in the packet (entries are removed with unset() as they get used, so the keys are sparse).
 * The C++ port uses std::map keyed by that same index; std::unordered_map is deliberately avoided so
 * that iteration stays in ascending index order, as it is in PHP.
 */
class DisassembledAvailableCommandsData final {
public:
    /**
     * @param commandData
     * @param unusedHardEnumValues
     * @param unusedPostfixes
     * @param unusedHardEnums
     * @param unusedSoftEnums
     * @param unusedChainedSubCommandData
     * @param unusedChainedSubCommandValues
     * @param repeatedEnumConstraints
     */
    DisassembledAvailableCommandsData(
        std::vector<types::command::CommandData> commandData,
        std::map<std::uint32_t, std::string> unusedHardEnumValues,
        std::map<std::uint32_t, std::string> unusedPostfixes,
        std::map<std::uint32_t, types::command::CommandHardEnum> unusedHardEnums,
        std::map<std::uint32_t, types::command::CommandSoftEnum> unusedSoftEnums,
        std::map<std::uint32_t, types::command::ChainedSubCommandData> unusedChainedSubCommandData,
        std::map<std::uint32_t, std::string> unusedChainedSubCommandValues,
        std::map<std::uint32_t, types::command::raw::CommandEnumConstraintRawData> repeatedEnumConstraints)
        : commandData(std::move(commandData)), unusedHardEnumValues(std::move(unusedHardEnumValues)),
          unusedPostfixes(std::move(unusedPostfixes)), unusedHardEnums(std::move(unusedHardEnums)),
          unusedSoftEnums(std::move(unusedSoftEnums)),
          unusedChainedSubCommandData(std::move(unusedChainedSubCommandData)),
          unusedChainedSubCommandValues(std::move(unusedChainedSubCommandValues)),
          repeatedEnumConstraints(std::move(repeatedEnumConstraints))
    {
    }

    const std::vector<types::command::CommandData> commandData;
    const std::map<std::uint32_t, std::string> unusedHardEnumValues;
    const std::map<std::uint32_t, std::string> unusedPostfixes;
    const std::map<std::uint32_t, types::command::CommandHardEnum> unusedHardEnums;
    const std::map<std::uint32_t, types::command::CommandSoftEnum> unusedSoftEnums;
    const std::map<std::uint32_t, types::command::ChainedSubCommandData> unusedChainedSubCommandData;
    const std::map<std::uint32_t, std::string> unusedChainedSubCommandValues;
    const std::map<std::uint32_t, types::command::raw::CommandEnumConstraintRawData> repeatedEnumConstraints;
};

}  // namespace bedrock_protocol::serializer
