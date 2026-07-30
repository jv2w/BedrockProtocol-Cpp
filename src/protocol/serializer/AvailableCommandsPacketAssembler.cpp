/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/serializer/AvailableCommandsPacketAssembler.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/serializer/AvailableCommandsPacketAssembler.h"

#include <stdexcept>
#include <utility>
#include <variant>

#include "bedrock_protocol/protocol/types/command/CommandOverload.h"
#include "bedrock_protocol/protocol/types/command/CommandParameter.h"
#include "bedrock_protocol/protocol/types/command/ConstrainedEnumValue.h"
#include "bedrock_protocol/protocol/types/command/raw/ChainedSubCommandValueRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandOverloadRawData.h"
#include "bedrock_protocol/protocol/types/command/raw/CommandParameterRawData.h"

namespace bedrock_protocol::serializer {

using types::command::ChainedSubCommandData;
using types::command::CommandData;
using types::command::CommandHardEnum;
using types::command::CommandSoftEnum;
using types::command::ConstrainedEnumValue;
using types::command::raw::ChainedSubCommandRawData;
using types::command::raw::ChainedSubCommandValueRawData;
using types::command::raw::CommandEnumConstraintRawData;
using types::command::raw::CommandEnumRawData;
using types::command::raw::CommandOverloadRawData;
using types::command::raw::CommandParameterRawData;
using types::command::raw::CommandRawData;

namespace {

/** Appends a length-prefixed field to a content signature, so that no two fields can run together. */
void appendField(std::string &key, std::string_view field)
{
    key += std::to_string(field.size());
    key += ':';
    key += field;
}

} // namespace

std::string AvailableCommandsPacketAssembler::identityKey(const CommandHardEnum &enum_)
{
    std::string key = "hard";
    appendField(key, enum_.getName());
    for (const auto &value : enum_.getValues()) {
        if (std::holds_alternative<ConstrainedEnumValue>(value)) {
            const auto &constrained = std::get<ConstrainedEnumValue>(value);
            key += 'c';
            appendField(key, constrained.getValue());
            for (const auto constraint : constrained.getConstraints()) {
                key += ',';
                key += std::to_string(constraint);
            }
            key += ';';
        }
        else {
            key += 'p';
            appendField(key, std::get<std::string>(value));
        }
    }
    return key;
}

std::string AvailableCommandsPacketAssembler::identityKey(const CommandSoftEnum &enum_)
{
    std::string key = "soft";
    appendField(key, enum_.getName());
    for (const auto &value : enum_.getValues()) {
        appendField(key, value);
    }
    return key;
}

std::string AvailableCommandsPacketAssembler::identityKey(const ChainedSubCommandData &data)
{
    std::string key = "chained";
    appendField(key, data.getName());
    for (const auto &value : data.getValues()) {
        appendField(key, value.getName());
        key += '=';
        key += std::to_string(value.getType());
        key += ';';
    }
    return key;
}

AvailableCommandsPacket AvailableCommandsPacketAssembler::assemble(
    const std::vector<CommandData> &commandData,
    const std::vector<CommandHardEnum> &hardcodedEnums,
    const std::vector<CommandSoftEnum> &hardcodedSoftEnums)
{
    AvailableCommandsPacketAssembler builder;
    for (const auto &data : commandData) {
        builder.addCommandData(data);
    }
    for (const auto &enum_ : hardcodedEnums) {
        builder.addHardEnum(enum_);
    }
    for (const auto &enum_ : hardcodedSoftEnums) {
        builder.addSoftEnum(enum_);
    }

    // PHP calls AvailableCommandsPacket::create() with named arguments. C++ has no equivalent, and
    // the positional form would be eight same-shaped vectors in a row - trivially easy to transpose.
    // Assigning the fields by name keeps the mapping checkable at a glance.
    AvailableCommandsPacket packet;
    packet.enumValues = std::move(builder.enumValues);
    packet.chainedSubCommandValues = std::move(builder.chainedSubCommandValues);
    packet.postfixes = std::move(builder.postfixes);
    packet.enums = std::move(builder.enums);
    packet.chainedSubCommandData = std::move(builder.chainedSubCommandData);
    packet.commandData = std::move(builder.commandData);
    packet.softEnums = std::move(builder.softEnums);
    packet.enumConstraints = std::move(builder.enumConstraints);
    return packet;
}

std::uint32_t AvailableCommandsPacketAssembler::addEnumValue(const std::string &str)
{
    const auto it = enumValueIndexes.find(str);
    if (it == enumValueIndexes.end()) {
        const auto index = static_cast<std::uint32_t>(enumValues.size());
        enumValueIndexes[str] = index;
        enumValues.push_back(str);
        return index;
    }
    return it->second;
}

std::uint32_t AvailableCommandsPacketAssembler::addHardEnum(const CommandHardEnum &enum_)
{
    const auto key = identityKey(enum_);
    const auto existing = enumIndexes.find(key);
    if (existing == enumIndexes.end()) {
        std::vector<std::uint32_t> valueIndexes;

        const auto enumIndex = static_cast<std::uint32_t>(enums.size());
        enumIndexes[key] = enumIndex;

        std::vector<CommandEnumConstraintRawData> constraints;
        for (const auto &value : enum_.getValues()) {
            std::uint32_t valueIndex;
            if (std::holds_alternative<ConstrainedEnumValue>(value)) {
                const auto &constrained = std::get<ConstrainedEnumValue>(value);
                valueIndex = addEnumValue(constrained.getValue());
                constraints.emplace_back(valueIndex, enumIndex, constrained.getConstraints());
            }
            else {
                valueIndex = addEnumValue(std::get<std::string>(value));
            }
            valueIndexes.push_back(valueIndex);
        }
        if (static_cast<std::uint32_t>(enums.size()) != enumIndex) {
            throw std::logic_error("Didn't expect enum list to be modified while compiling values");
        }
        enums.emplace_back(enum_.getName(), std::move(valueIndexes));
        for (auto &constraint : constraints) {
            enumConstraints.push_back(std::move(constraint));
        }

        return enumIndex;
    }

    return existing->second;
}

std::uint32_t AvailableCommandsPacketAssembler::addSoftEnum(const CommandSoftEnum &enum_)
{
    const auto key = identityKey(enum_);

    const auto it = softEnumIndexes.find(key);
    if (it == softEnumIndexes.end()) {
        const auto index = static_cast<std::uint32_t>(softEnums.size());
        softEnumIndexes[key] = index;
        softEnums.push_back(enum_);
        return index;
    }

    return it->second;
}

std::uint32_t AvailableCommandsPacketAssembler::addPostfix(const std::string &postfix)
{
    const auto it = postfixIndexes.find(postfix);
    if (it == postfixIndexes.end()) {
        const auto index = static_cast<std::uint32_t>(postfixes.size());
        postfixIndexes[postfix] = index;
        postfixes.push_back(postfix);
        return index;
    }
    return it->second;
}

std::uint32_t AvailableCommandsPacketAssembler::addChainedSubCommandValueName(const std::string &valueName)
{
    const auto it = chainedSubCommandValueIndexes.find(valueName);
    if (it == chainedSubCommandValueIndexes.end()) {
        const auto index = static_cast<std::uint32_t>(chainedSubCommandValues.size());
        chainedSubCommandValueIndexes[valueName] = index;
        chainedSubCommandValues.push_back(valueName);
        return index;
    }
    return it->second;
}

std::uint32_t AvailableCommandsPacketAssembler::addChainedSubCommandData(const ChainedSubCommandData &data)
{
    const auto key = identityKey(data);

    const auto existing = chainedSubCommandDataIndexes.find(key);
    if (existing == chainedSubCommandDataIndexes.end()) {
        std::vector<ChainedSubCommandValueRawData> rawValueData;
        for (const auto &value : data.getValues()) {
            const auto valueNameIndex = addChainedSubCommandValueName(value.getName());
            rawValueData.emplace_back(valueNameIndex, value.getType());
        }

        const auto index = static_cast<std::uint32_t>(chainedSubCommandData.size());
        chainedSubCommandDataIndexes[key] = index;
        chainedSubCommandData.emplace_back(data.getName(), std::move(rawValueData));
        return index;
    }

    return existing->second;
}

void AvailableCommandsPacketAssembler::addCommandData(const CommandData &commandData)
{
    const auto aliasesIndex = commandData.aliases.has_value()
                                  ? static_cast<std::int32_t>(addHardEnum(*commandData.aliases))
                                  : -1;

    std::vector<std::uint32_t> chainedSubCommandDataIndexes;
    for (const auto &chainedSubCommandData : commandData.getChainedSubCommandData()) {
        chainedSubCommandDataIndexes.push_back(addChainedSubCommandData(chainedSubCommandData));
    }

    std::vector<CommandOverloadRawData> rawOverloadData;
    for (const auto &overload : commandData.getOverloads()) {
        std::vector<CommandParameterRawData> rawParameterData;

        for (const auto &parameter : overload.getParameters()) {
            std::uint32_t typeInfo;
            if (!std::holds_alternative<std::monostate>(parameter.enum_)) {
                if (std::holds_alternative<CommandSoftEnum>(parameter.enum_)) {
                    const auto enumIndex = addSoftEnum(std::get<CommandSoftEnum>(parameter.enum_));
                    typeInfo = static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_SOFT_ENUM) |
                               static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) | enumIndex;
                }
                else {
                    const auto enumIndex = addHardEnum(std::get<CommandHardEnum>(parameter.enum_));
                    typeInfo = static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_ENUM) |
                               static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) | enumIndex;
                }
            }
            else if (parameter.postfix.has_value()) {
                const auto postfixIndex = addPostfix(*parameter.postfix);
                typeInfo = static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_POSTFIX) | postfixIndex;
            }
            else {
                //mask this to prevent unwanted flags sneaking in
                typeInfo = static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) |
                           (parameter.paramType & (static_cast<std::uint32_t>(AvailableCommandsPacket::ARG_FLAG_VALID) - 1));
            }

            rawParameterData.emplace_back(parameter.paramName, typeInfo, parameter.isOptional, parameter.flags);
        }

        rawOverloadData.emplace_back(overload.isChaining(), std::move(rawParameterData));
    }

    this->commandData.emplace_back(
        commandData.getName(),
        commandData.getDescription(),
        commandData.getFlags(),
        commandData.getPermission(),
        aliasesIndex,
        std::move(chainedSubCommandDataIndexes),
        std::move(rawOverloadData)
    );
}

}  // namespace bedrock_protocol::serializer
