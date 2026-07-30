/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/ModalFormResponsePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ModalFormResponsePacket.h"

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

ModalFormResponsePacket ModalFormResponsePacket::create(std::uint32_t formId, std::optional<std::string> formData, std::optional<std::uint8_t> cancelReason)
{
    ModalFormResponsePacket result;
    result.formId = formId;
    result.formData = std::move(formData);
    result.cancelReason = std::move(cancelReason);
    return result;
}

ModalFormResponsePacket ModalFormResponsePacket::response(std::uint32_t formId, std::optional<std::string> formData)
{
    return create(formId, formData, std::nullopt);
}

ModalFormResponsePacket ModalFormResponsePacket::cancel(std::uint32_t formId, std::optional<std::uint8_t> cancelReason)
{
    return create(formId, std::nullopt, cancelReason);
}

void ModalFormResponsePacket::decodePayload(encoding::ByteBufferReader &in)
{
    formId = encoding::VarInt::readUnsignedInt(in);
    formData = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return serializer::CommonTypes::getString(reader); });
    cancelReason = serializer::CommonTypes::readOptional(in, [](encoding::ByteBufferReader &reader) { return encoding::Byte::readUnsigned(reader); });

}

void ModalFormResponsePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, formId);

    serializer::CommonTypes::writeOptional(out, formData, [](encoding::ByteBufferWriter &writer, const auto &value) { serializer::CommonTypes::putString(writer, value); });
    serializer::CommonTypes::writeOptional(out, cancelReason, [](encoding::ByteBufferWriter &writer, const auto &value) { encoding::Byte::writeUnsigned(writer, value); });

}

bool ModalFormResponsePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleModalFormResponse(*this);
}

}  // namespace bedrock_protocol
