/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/AvailableCommandsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock_protocol/protocol/ClientboundPacket.h"
#include "bedrock_protocol/protocol/DataPacket.h"
#include "bedrock_protocol/protocol/ProtocolInfo.h"
#include "bedrock_protocol/protocol/ServerboundPacket.h"
#include "bedrock_protocol/protocol/types/command/CommandParameterTypes.h"
#include "bedrock_protocol/protocol/types/command/CommandSoftEnum.h"
#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandEnumConstraintRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandEnumRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandRawData.h"

namespace bedrock_protocol {

class PacketHandlerInterface;

/**
 * Note: It's not recommended to work with this packet directly. It's very complicated, and it's very easy to crash the
 * client if the packet data is incorrect in any way.
 *
 * To assemble a packet for sending from high-level structures, use @see AvailableCommandsPacketAssembler.
 * To disassemble a received packet into high-level structures, use @see AvailableCommandsPacketDisassembler.
 */
class AvailableCommandsPacket final : public DataPacket, public ClientboundPacket {
public:
    static constexpr std::uint32_t NETWORK_ID = ProtocolInfo::AVAILABLE_COMMANDS_PACKET;

    /**
     * This flag is set on all types EXCEPT the POSTFIX type. Not completely sure what this is for, but it is required
     * for the argtype to work correctly. VALID seems as good a name as any.
     */
    static constexpr std::int32_t ARG_FLAG_VALID = 0x100000;
    /**
     * Basic parameter types. These must be combined with the ARG_FLAG_VALID constant.
     * ARG_FLAG_VALID | (type const)
     */
    static constexpr std::int32_t ARG_TYPE_INT = types::command::CommandParameterTypes::INT;
    static constexpr std::int32_t ARG_TYPE_FLOAT = types::command::CommandParameterTypes::VAL;
    static constexpr std::int32_t ARG_TYPE_VALUE = types::command::CommandParameterTypes::RVAL;
    static constexpr std::int32_t ARG_TYPE_WILDCARD_INT = types::command::CommandParameterTypes::WILDCARDINT;
    static constexpr std::int32_t ARG_TYPE_OPERATOR = types::command::CommandParameterTypes::OPERATOR;
    static constexpr std::int32_t ARG_TYPE_COMPARE_OPERATOR = types::command::CommandParameterTypes::COMPAREOPERATOR;
    static constexpr std::int32_t ARG_TYPE_TARGET = types::command::CommandParameterTypes::SELECTION;
    static constexpr std::int32_t ARG_TYPE_WILDCARD_TARGET = types::command::CommandParameterTypes::WILDCARDSELECTION;
    static constexpr std::int32_t ARG_TYPE_FILEPATH = types::command::CommandParameterTypes::PATHCOMMAND;
    static constexpr std::int32_t ARG_TYPE_FULL_INTEGER_RANGE = types::command::CommandParameterTypes::FULLINTEGERRANGE;
    static constexpr std::int32_t ARG_TYPE_EQUIPMENT_SLOT = types::command::CommandParameterTypes::EQUIPMENTSLOTENUM;
    static constexpr std::int32_t ARG_TYPE_STRING = types::command::CommandParameterTypes::ID;
    static constexpr std::int32_t ARG_TYPE_INT_POSITION = types::command::CommandParameterTypes::POSITION;
    static constexpr std::int32_t ARG_TYPE_POSITION = types::command::CommandParameterTypes::POSITION_FLOAT;
    static constexpr std::int32_t ARG_TYPE_MESSAGE = types::command::CommandParameterTypes::MESSAGE_ROOT;
    static constexpr std::int32_t ARG_TYPE_RAWTEXT = types::command::CommandParameterTypes::RAWTEXT;
    static constexpr std::int32_t ARG_TYPE_JSON = types::command::CommandParameterTypes::JSON_OBJECT;
    static constexpr std::int32_t ARG_TYPE_BLOCK_STATES = types::command::CommandParameterTypes::BLOCK_STATE_ARRAY;
    static constexpr std::int32_t ARG_TYPE_COMMAND = types::command::CommandParameterTypes::CODEBUILDERARGS;
    /**
     * Enums are a little different: they are composed as follows:
     * ARG_FLAG_ENUM | ARG_FLAG_VALID | (enum index)
     */
    static constexpr std::int32_t ARG_FLAG_ENUM = 0x200000;
    /** This is used for /xp <level: int>L. It can only be applied to integer parameters. */
    static constexpr std::int32_t ARG_FLAG_POSTFIX = 0x1000000;
    static constexpr std::int32_t ARG_FLAG_SOFT_ENUM = 0x4000000;

    std::vector<std::string> enumValues;
    std::vector<std::string> chainedSubCommandValues;
    std::vector<std::string> postfixes;
    std::vector<types::command::raw::CommandEnumRawData> enums;
    std::vector<types::command::raw::ChainedSubCommandRawData> chainedSubCommandData;
    std::vector<types::command::raw::CommandRawData> commandData;
    std::vector<types::command::CommandSoftEnum> softEnums;
    std::vector<types::command::raw::CommandEnumConstraintRawData> enumConstraints;

    /**
     * @generate-create-func
     */
    static AvailableCommandsPacket create(std::vector<std::string> enumValues, std::vector<std::string> chainedSubCommandValues, std::vector<std::string> postfixes, std::vector<types::command::raw::CommandEnumRawData> enums, std::vector<types::command::raw::ChainedSubCommandRawData> chainedSubCommandData, std::vector<types::command::raw::CommandRawData> commandData, std::vector<types::command::CommandSoftEnum> softEnums, std::vector<types::command::raw::CommandEnumConstraintRawData> enumConstraints);

    [[nodiscard]] std::uint32_t networkId() const override { return NETWORK_ID; }
    [[nodiscard]] std::string_view getName() const override { return "AvailableCommandsPacket"; }
    bool handle(PacketHandlerInterface &handler) override;

protected:
    void decodePayload(encoding::ByteBufferReader &in) override;
    void encodePayload(encoding::ByteBufferWriter &out) const override;
};

}  // namespace bedrock_protocol
