/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/BookEditPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/BookEditPacket.h"

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

void BookEditPacket::decodePayload(encoding::ByteBufferReader &in)
{
    inventorySlot = encoding::VarInt::readSignedInt(in);
    type = encoding::VarInt::readUnsignedInt(in);

    switch (type) {
        case TYPE_REPLACE_PAGE:
        case TYPE_ADD_PAGE:
            pageNumber = encoding::VarInt::readSignedInt(in);
            text = serializer::CommonTypes::getString(in);
            photoName = serializer::CommonTypes::getString(in);
            break;
        case TYPE_DELETE_PAGE:
            pageNumber = encoding::VarInt::readSignedInt(in);
            break;
        case TYPE_SWAP_PAGES:
            pageNumber = encoding::VarInt::readSignedInt(in);
            secondaryPageNumber = encoding::VarInt::readSignedInt(in);
            break;
        case TYPE_SIGN_BOOK:
            title = serializer::CommonTypes::getString(in);
            author = serializer::CommonTypes::getString(in);
            xuid = serializer::CommonTypes::getString(in);
            break;
        default:
            throw PacketDecodeException(std::string("Unknown book edit type ") + std::to_string(type) + "!");
    }

}

void BookEditPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeSignedInt(out, inventorySlot);
    encoding::VarInt::writeUnsignedInt(out, type);

    switch (type) {
        case TYPE_REPLACE_PAGE:
        case TYPE_ADD_PAGE:
            encoding::VarInt::writeSignedInt(out, pageNumber);
            serializer::CommonTypes::putString(out, text);
            serializer::CommonTypes::putString(out, photoName);
            break;
        case TYPE_DELETE_PAGE:
            encoding::VarInt::writeSignedInt(out, pageNumber);
            break;
        case TYPE_SWAP_PAGES:
            encoding::VarInt::writeSignedInt(out, pageNumber);
            encoding::VarInt::writeSignedInt(out, secondaryPageNumber);
            break;
        case TYPE_SIGN_BOOK:
            serializer::CommonTypes::putString(out, title);
            serializer::CommonTypes::putString(out, author);
            serializer::CommonTypes::putString(out, xuid);
            break;
        default:
            throw std::invalid_argument(std::string("Unknown book edit type ") + std::to_string(type) + "!");
    }

}

bool BookEditPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleBookEdit(*this);
}

}  // namespace bedrock_protocol
