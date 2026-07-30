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

#include "bedrock_protocol/protocol/serializer/AvailableCommandsPacketDisassembler.h"

#include <utility>
#include <vector>

#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/types/command/ChainedSubCommandValue.h"
#include "bedrock_protocol/protocol/types/command/CommandOverload.h"
#include "bedrock_protocol/protocol/types/command/CommandParameter.h"
#include "bedrock_protocol/protocol/types/command/ConstrainedEnumValue.h"

namespace bedrock_protocol::serializer {

using types::command::ChainedSubCommandData;
using types::command::ChainedSubCommandValue;
using types::command::CommandData;
using types::command::CommandHardEnum;
using types::command::CommandOverload;
using types::command::CommandParameter;
using types::command::CommandSoftEnum;
using types::command::ConstrainedEnumValue;
using types::command::raw::CommandRawData;

namespace {

/** Rebuilds PHP's int-keyed array from a C++ list, preserving the original indexes as keys. */
template <typename T>
std::map<std::uint32_t, T> indexed(const std::vector<T> &list)
{
    std::map<std::uint32_t, T> result;
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(list.size()); ++i) {
        result.emplace(i, list[i]);
    }
    return result;
}

} // namespace

AvailableCommandsPacketDisassembler::AvailableCommandsPacketDisassembler(const AvailableCommandsPacket &packet)
    : packet(packet)
{
    unusedHardEnumValues = indexed(packet.enumValues);
    unusedHardEnumRawData = indexed(packet.enums);
    unusedSoftEnums = indexed(packet.softEnums);
    unusedPostfixes = indexed(packet.postfixes);
    unusedChainedSubCommandRawData = indexed(packet.chainedSubCommandData);
    unusedChainedSubCommandValues = indexed(packet.chainedSubCommandValues);
}

DisassembledAvailableCommandsData AvailableCommandsPacketDisassembler::disassemble(
    const AvailableCommandsPacket &packet)
{
    AvailableCommandsPacketDisassembler result(packet);

    //this lets us put the data for the constraints inside the CommandEnum objects directly
    std::map<std::uint32_t, types::command::raw::CommandEnumConstraintRawData> repeatedEnumConstraints;
    for (std::uint32_t index = 0; index < static_cast<std::uint32_t>(packet.enumConstraints.size()); ++index) {
        const auto &rawConstraintData = packet.enumConstraints[index];
        const auto enumIndex = rawConstraintData.getEnumIndex();
        const auto affectedValueIndex = rawConstraintData.getAffectedValueIndex();
        const auto enumIt = result.enumConstraintIndex.find(enumIndex);
        if (enumIt != result.enumConstraintIndex.end() &&
            enumIt->second.find(affectedValueIndex) != enumIt->second.end()) {
            repeatedEnumConstraints.emplace(index, rawConstraintData);
        }
        else {
            result.enumConstraintIndex[rawConstraintData.getEnumIndex()][rawConstraintData.getAffectedValueIndex()] =
                &rawConstraintData;
        }
    }

    std::vector<CommandData> unusedCommandData;
    for (const auto &rawData : packet.commandData) {
        unusedCommandData.push_back(result.processCommandData(rawData));
    }
    //PHP's foreach iterates over a snapshot of the array; the lookups below remove entries from it,
    //so the keys must be collected up front here.
    std::map<std::uint32_t, CommandHardEnum> unusedHardEnums;
    {
        std::vector<std::uint32_t> indexes;
        for (const auto &entry : result.unusedHardEnumRawData) {
            indexes.push_back(entry.first);
        }
        for (const auto index : indexes) {
            unusedHardEnums.emplace(index, result.lookupHardEnum(index));
        }
    }
    std::map<std::uint32_t, ChainedSubCommandData> unusedChainedSubCommandData;
    {
        std::vector<std::uint32_t> indexes;
        for (const auto &entry : result.unusedChainedSubCommandRawData) {
            indexes.push_back(entry.first);
        }
        for (const auto index : indexes) {
            unusedChainedSubCommandData.emplace(index, result.lookupChainedSubCommandData(index));
        }
    }

    return DisassembledAvailableCommandsData(
        std::move(unusedCommandData),
        std::move(result.unusedHardEnumValues),
        std::move(result.unusedPostfixes),
        std::move(unusedHardEnums),
        std::move(result.unusedSoftEnums),
        std::move(unusedChainedSubCommandData),
        std::move(result.unusedChainedSubCommandValues),
        std::move(repeatedEnumConstraints)
    );
}

const std::string &AvailableCommandsPacketDisassembler::lookupHardEnumValue(std::uint32_t index)
{
    if (index >= static_cast<std::uint32_t>(packet.enumValues.size())) {
        throw PacketDecodeException("No such enum value index " + std::to_string(index));
    }
    const auto &value = packet.enumValues[index];
    unusedHardEnumValues.erase(index);
    return value;
}

const CommandHardEnum &AvailableCommandsPacketDisassembler::lookupHardEnum(std::uint32_t index)
{
    if (linkedEnumCache.find(index) == linkedEnumCache.end()) {
        if (index >= static_cast<std::uint32_t>(packet.enums.size())) {
            throw PacketDecodeException("No such enum index " + std::to_string(index));
        }
        const auto &rawEnum = packet.enums[index];

        std::vector<CommandHardEnum::Value> enumValues;
        for (const auto valueIndex : rawEnum.getValueIndexes()) {
            const auto &value = lookupHardEnumValue(valueIndex);

            const types::command::raw::CommandEnumConstraintRawData *rawConstraint = nullptr;
            const auto enumIt = enumConstraintIndex.find(index);
            if (enumIt != enumConstraintIndex.end()) {
                const auto valueIt = enumIt->second.find(valueIndex);
                if (valueIt != enumIt->second.end()) {
                    rawConstraint = valueIt->second;
                }
            }
            if (rawConstraint != nullptr) {
                enumValues.emplace_back(ConstrainedEnumValue(value, rawConstraint->getConstraints()));
            }
            else {
                enumValues.emplace_back(value);
            }
        }

        linkedEnumCache.emplace(index, CommandHardEnum(rawEnum.getName(), std::move(enumValues)));
        unusedHardEnumRawData.erase(index);
    }

    return linkedEnumCache.at(index);
}

const CommandSoftEnum &AvailableCommandsPacketDisassembler::lookupSoftEnum(std::uint32_t index) const
{
    //no conversion needed - these are fully self-contained
    if (index >= static_cast<std::uint32_t>(packet.softEnums.size())) {
        throw PacketDecodeException("No such soft enum index " + std::to_string(index));
    }
    return packet.softEnums[index];
}

const std::string &AvailableCommandsPacketDisassembler::lookupChainedSubCommandValue(std::uint32_t index)
{
    if (index >= static_cast<std::uint32_t>(packet.chainedSubCommandValues.size())) {
        throw PacketDecodeException("No such chained subcommand value index " + std::to_string(index));
    }
    const auto &value = packet.chainedSubCommandValues[index];
    unusedChainedSubCommandValues.erase(index);
    return value;
}

const ChainedSubCommandData &AvailableCommandsPacketDisassembler::lookupChainedSubCommandData(std::uint32_t index)
{
    if (linkedChainedSubCommandDataCache.find(index) == linkedChainedSubCommandDataCache.end()) {
        if (index >= static_cast<std::uint32_t>(packet.chainedSubCommandData.size())) {
            throw PacketDecodeException("No such chained subcommand index " + std::to_string(index));
        }
        const auto &rawData = packet.chainedSubCommandData[index];

        std::vector<ChainedSubCommandValue> values;
        for (const auto &rawValueData : rawData.getValueData()) {
            const auto &valueName = lookupChainedSubCommandValue(rawValueData.getNameIndex());
            values.emplace_back(valueName, rawValueData.getType());
        }

        linkedChainedSubCommandDataCache.emplace(index, ChainedSubCommandData(rawData.getName(), std::move(values)));
        unusedChainedSubCommandRawData.erase(index);
    }

    return linkedChainedSubCommandDataCache.at(index);
}

const std::string &AvailableCommandsPacketDisassembler::lookupPostfix(std::uint32_t index)
{
    if (index >= static_cast<std::uint32_t>(packet.postfixes.size())) {
        throw PacketDecodeException("No such postfix index " + std::to_string(index));
    }
    const auto &value = packet.postfixes[index];
    unusedPostfixes.erase(index);
    return value;
}

CommandData AvailableCommandsPacketDisassembler::processCommandData(const CommandRawData &rawData)
{
    const auto aliasesIndex = rawData.getAliasEnumIndex();
    std::optional<CommandHardEnum> aliasesEnum =
        aliasesIndex == -1 ? std::nullopt
                           : std::optional<CommandHardEnum>(lookupHardEnum(static_cast<std::uint32_t>(aliasesIndex)));

    std::vector<ChainedSubCommandData> chainedSubCommandData;
    for (const auto dataIndex : rawData.getChainedSubCommandDataIndexes()) {
        chainedSubCommandData.push_back(lookupChainedSubCommandData(dataIndex));
    }

    std::vector<CommandOverload> overloads;
    for (std::size_t overloadIndex = 0; overloadIndex < rawData.getOverloads().size(); ++overloadIndex) {
        const auto &rawOverloadData = rawData.getOverloads()[overloadIndex];
        std::vector<CommandParameter> parameters;
        for (std::size_t parameterIndex = 0; parameterIndex < rawOverloadData.getParameters().size();
             ++parameterIndex) {
            const auto &rawParameterData = rawOverloadData.getParameters()[parameterIndex];
            const auto typeInfo = rawParameterData.getTypeInfo();
            const auto flags = typeInfo & static_cast<std::uint32_t>(
                                              AvailableCommandsPacket::ARG_FLAG_ENUM |
                                              AvailableCommandsPacket::ARG_FLAG_SOFT_ENUM |
                                              AvailableCommandsPacket::ARG_FLAG_POSTFIX |
                                              AvailableCommandsPacket::ARG_FLAG_VALID);
            //these flags are mutually exclusive - more than one is an error
            CommandParameter::EnumType enum_ = std::monostate{};
            std::optional<std::string> postfix = std::nullopt;
            std::uint32_t highLevelTypeInfo = 0;
            if (flags == static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_ENUM |
                                                    AvailableCommandsPacket::ARG_FLAG_VALID)) {
                const auto index = typeInfo & (static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) - 1);
                enum_ = lookupHardEnum(index);
            }
            else if (flags == static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_SOFT_ENUM |
                                                         AvailableCommandsPacket::ARG_FLAG_VALID)) {
                const auto index = typeInfo & (static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) - 1);
                enum_ = lookupSoftEnum(index);
            }
            else if (flags == static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_POSTFIX)) {
                const auto index =
                    typeInfo & (static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_POSTFIX) - 1);
                postfix = lookupPostfix(index);
            }
            else if (flags == static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID)) {
                highLevelTypeInfo = typeInfo & (static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) - 1);
            }
            else {
                throw PacketDecodeException("Unrecognized arg flag combination " + std::to_string(typeInfo) +
                                            " for command " + rawData.getName() + ", overload " +
                                            std::to_string(overloadIndex) + ", parameter " +
                                            std::to_string(parameterIndex));
            }

            parameters.push_back(CommandParameter::allFields(
                rawParameterData.getName(),
                highLevelTypeInfo,
                rawParameterData.isOptional(),
                rawParameterData.getFlags(),
                std::move(enum_),
                std::move(postfix)
            ));
        }

        overloads.emplace_back(rawOverloadData.isChaining(), std::move(parameters));
    }

    return CommandData(
        rawData.getName(),
        rawData.getDescription(),
        rawData.getFlags(),
        rawData.getPermission(),
        std::move(aliasesEnum),
        std::move(overloads),
        std::move(chainedSubCommandData)
    );
}

}  // namespace bedrock_protocol::serializer
