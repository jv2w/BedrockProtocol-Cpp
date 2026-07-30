/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/AvailableCommandsPacketAssembler.php
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
#include <unordered_map>
#include <vector>

#include "bedrock_protocol/protocol/AvailableCommandsPacket.h"
#include "bedrock_protocol/protocol/types/command/ChainedSubCommandData.h"
#include "bedrock_protocol/protocol/types/command/CommandData.h"
#include "bedrock_protocol/protocol/types/command/CommandHardEnum.h"
#include "bedrock_protocol/protocol/types/command/CommandSoftEnum.h"
#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandEnumConstraintRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandEnumRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandRawData.h"

namespace bedrock_protocol::serializer {

/**
 * Assembles high-level commands data into low-level AvailableCommandsPacket structures
 *
 * @note DELIBERATE DEVIATION: PHP deduplicates enums, soft enums and chained subcommand data by
 * object identity (spl_object_id). C++ holds these as values, so there is no object identity to key
 * on; this port keys on a content signature instead. Two distinct-but-identical inputs therefore
 * collapse into a single wire entry where PHP would have emitted two. The resulting packet is still
 * valid, and inputs that differ in any way are never merged.
 */
class AvailableCommandsPacketAssembler final {
public:
    /**
     * @param commandData
     * @param hardcodedEnums
     * @param hardcodedSoftEnums
     */
    static AvailableCommandsPacket assemble(const std::vector<types::command::CommandData> &commandData,
                                            const std::vector<types::command::CommandHardEnum> &hardcodedEnums,
                                            const std::vector<types::command::CommandSoftEnum> &hardcodedSoftEnums);

private:
    AvailableCommandsPacketAssembler() = default;

    std::uint32_t addEnumValue(const std::string &str);

    std::uint32_t addHardEnum(const types::command::CommandHardEnum &enum_);

    std::uint32_t addSoftEnum(const types::command::CommandSoftEnum &enum_);

    std::uint32_t addPostfix(const std::string &postfix);

    std::uint32_t addChainedSubCommandValueName(const std::string &valueName);

    std::uint32_t addChainedSubCommandData(const types::command::ChainedSubCommandData &data);

    void addCommandData(const types::command::CommandData &commandData);

    /** Content signature standing in for PHP's spl_object_id(). @see class doc */
    static std::string identityKey(const types::command::CommandHardEnum &enum_);
    static std::string identityKey(const types::command::CommandSoftEnum &enum_);
    static std::string identityKey(const types::command::ChainedSubCommandData &data);

    std::unordered_map<std::string, std::uint32_t> enumIndexes;
    std::unordered_map<std::string, std::uint32_t> enumValueIndexes;
    std::unordered_map<std::string, std::uint32_t> softEnumIndexes;
    std::unordered_map<std::string, std::uint32_t> postfixIndexes;
    std::unordered_map<std::string, std::uint32_t> chainedSubCommandDataIndexes;
    std::unordered_map<std::string, std::uint32_t> chainedSubCommandValueIndexes;

    std::vector<std::string> enumValues;
    std::vector<std::string> chainedSubCommandValues;
    std::vector<std::string> postfixes;
    std::vector<types::command::raw::CommandEnumRawData> enums;
    std::vector<types::command::raw::ChainedSubCommandRawData> chainedSubCommandData;
    std::vector<types::command::raw::CommandRawData> commandData;
    std::vector<types::command::CommandSoftEnum> softEnums;
    std::vector<types::command::raw::CommandEnumConstraintRawData> enumConstraints;
};

}  // namespace bedrock_protocol::serializer
