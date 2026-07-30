/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/serializer/AvailableCommandsPacketDisassembler.php
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

#include "bedrock_protocol/protocol/AvailableCommandsPacket.h"
#include "bedrock_protocol/protocol/serializer/DisassembledAvailableCommandsData.h"
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
 * Disassembles low-level AvailableCommandsPacket structures into high-level commands data that can be operated on more
 * easily (no nasty offsets).
 *
 * @note The packet passed to disassemble() must outlive the call; the disassembler reads from it
 * directly instead of copying it, as PHP does by holding a reference.
 */
class AvailableCommandsPacketDisassembler final {
public:
    /**
     * @throws PacketDecodeException
     */
    static DisassembledAvailableCommandsData disassemble(const AvailableCommandsPacket &packet);

private:
    explicit AvailableCommandsPacketDisassembler(const AvailableCommandsPacket &packet);

    /**
     * @throws PacketDecodeException
     */
    const std::string &lookupHardEnumValue(std::uint32_t index);

    /**
     * @throws PacketDecodeException
     */
    const types::command::CommandHardEnum &lookupHardEnum(std::uint32_t index);

    /**
     * @throws PacketDecodeException
     */
    const types::command::CommandSoftEnum &lookupSoftEnum(std::uint32_t index) const;

    /**
     * @throws PacketDecodeException
     */
    const std::string &lookupChainedSubCommandValue(std::uint32_t index);

    /**
     * @throws PacketDecodeException
     */
    const types::command::ChainedSubCommandData &lookupChainedSubCommandData(std::uint32_t index);

    /**
     * @throws PacketDecodeException
     */
    const std::string &lookupPostfix(std::uint32_t index);

    /**
     * @throws PacketDecodeException
     */
    types::command::CommandData processCommandData(const types::command::raw::CommandRawData &rawData);

    const AvailableCommandsPacket &packet;

    std::map<std::uint32_t, std::map<std::uint32_t, const types::command::raw::CommandEnumConstraintRawData *>>
        enumConstraintIndex;

    std::map<std::uint32_t, types::command::CommandHardEnum> linkedEnumCache;
    std::map<std::uint32_t, types::command::ChainedSubCommandData> linkedChainedSubCommandDataCache;

    std::map<std::uint32_t, std::string> unusedHardEnumValues;
    std::map<std::uint32_t, std::string> unusedPostfixes;

    std::map<std::uint32_t, types::command::raw::CommandEnumRawData> unusedHardEnumRawData;
    std::map<std::uint32_t, types::command::CommandSoftEnum> unusedSoftEnums;
    std::map<std::uint32_t, types::command::raw::ChainedSubCommandRawData> unusedChainedSubCommandRawData;
    std::map<std::uint32_t, std::string> unusedChainedSubCommandValues;
};

}  // namespace bedrock_protocol::serializer
