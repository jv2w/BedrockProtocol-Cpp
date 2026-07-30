/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/CommandOutputPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/CommandOutputPacket.h"

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
namespace {

using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

/**
 * PHP declares this as a protected instance method, but it never touches $this, so the port keeps it
 * file-local rather than adding it to the packet's surface.
 *
 * @throws DataDecodeException
 */
types::command::CommandOutputMessage getCommandMessage(encoding::ByteBufferReader &in)
{
    types::command::CommandOutputMessage message;

    message.messageId = CommonTypes::getString(in);
    message.isInternal = CommonTypes::getBool(in);

    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; ++i) {
        message.parameters.push_back(CommonTypes::getString(in));
    }

    return message;
}

/** Counterpart of getCommandMessage(); see the note there. */
void putCommandMessage(const types::command::CommandOutputMessage &message, encoding::ByteBufferWriter &out)
{
    CommonTypes::putString(out, message.messageId);
    CommonTypes::putBool(out, message.isInternal);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(message.parameters.size()));
    for (const auto &parameter : message.parameters) {
        CommonTypes::putString(out, parameter);
    }
}

}  // namespace

void CommandOutputPacket::decodePayload(encoding::ByteBufferReader &in)
{
    originData = CommonTypes::getCommandOriginData(in);
    outputType = CommonTypes::getString(in);
    successCount = LE::readUnsignedInt(in);

    for (std::uint32_t i = 0, size = VarInt::readUnsignedInt(in); i < size; ++i) {
        messages.push_back(getCommandMessage(in));
    }

    data = CommonTypes::readOptional(
        in, [](encoding::ByteBufferReader &reader) { return CommonTypes::getString(reader); });
}

void CommandOutputPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putCommandOriginData(out, originData);
    CommonTypes::putString(out, outputType);
    LE::writeUnsignedInt(out, successCount);

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(messages.size()));
    for (const auto &message : messages) {
        putCommandMessage(message, out);
    }

    CommonTypes::writeOptional(out, data, [](encoding::ByteBufferWriter &writer, const std::string &value) {
        CommonTypes::putString(writer, value);
    });
}

bool CommandOutputPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleCommandOutput(*this);
}

}  // namespace bedrock_protocol
