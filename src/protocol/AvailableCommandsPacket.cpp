/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/AvailableCommandsPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/AvailableCommandsPacket.h"

#include <stdexcept>
#include <utility>

#include "bedrock_protocol/encoding/BE.h"
#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/PacketHandlerInterface.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol {

AvailableCommandsPacket AvailableCommandsPacket::create(std::vector<std::string> enumValues, std::vector<std::string> chainedSubCommandValues, std::vector<std::string> postfixes, std::vector<types::command::raw::CommandEnumRawData> enums, std::vector<types::command::raw::ChainedSubCommandRawData> chainedSubCommandData, std::vector<types::command::raw::CommandRawData> commandData, std::vector<types::command::CommandSoftEnum> softEnums, std::vector<types::command::raw::CommandEnumConstraintRawData> enumConstraints)
{
    AvailableCommandsPacket result;
    result.enumValues = std::move(enumValues);
    result.chainedSubCommandValues = std::move(chainedSubCommandValues);
    result.postfixes = std::move(postfixes);
    result.enums = std::move(enums);
    result.chainedSubCommandData = std::move(chainedSubCommandData);
    result.commandData = std::move(commandData);
    result.softEnums = std::move(softEnums);
    result.enumConstraints = std::move(enumConstraints);
    return result;
}

void AvailableCommandsPacket::decodePayload(encoding::ByteBufferReader &in)
{
    enumValues.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        enumValues.push_back(serializer::CommonTypes::getString(in));
    }

    chainedSubCommandValues.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        chainedSubCommandValues.push_back(serializer::CommonTypes::getString(in));
    }

    postfixes.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        postfixes.push_back(serializer::CommonTypes::getString(in));
    }

    enums.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        enums.push_back(types::command::raw::CommandEnumRawData::read(in));
    }

    chainedSubCommandData.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        chainedSubCommandData.push_back(types::command::raw::ChainedSubCommandRawData::read(in));
    }

    commandData.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        commandData.push_back(types::command::raw::CommandRawData::read(in));
    }

    softEnums.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        softEnums.push_back(types::command::CommandSoftEnum::read(in));
    }

    enumConstraints.clear();
    for (std::uint32_t i = 0, size = encoding::VarInt::readUnsignedInt(in); i < size; ++i) {
        enumConstraints.push_back(types::command::raw::CommandEnumConstraintRawData::read(in));
    }

}

void AvailableCommandsPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(enumValues.size()));
    for (const auto &value : enumValues) {
        serializer::CommonTypes::putString(out, value);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(chainedSubCommandValues.size()));
    for (const auto &value : chainedSubCommandValues) {
        serializer::CommonTypes::putString(out, value);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(postfixes.size()));
    for (const auto &postfix : postfixes) {
        serializer::CommonTypes::putString(out, postfix);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(enums.size()));
    for (const auto &commandEnum : enums) {
        commandEnum.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(chainedSubCommandData.size()));
    for (const auto &data : chainedSubCommandData) {
        data.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(commandData.size()));
    for (const auto &data : commandData) {
        data.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(softEnums.size()));
    for (const auto &softEnum : softEnums) {
        softEnum.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(enumConstraints.size()));
    for (const auto &constraint : enumConstraints) {
        constraint.write(out);
    }

}

bool AvailableCommandsPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleAvailableCommands(*this);
}

}  // namespace bedrock_protocol
