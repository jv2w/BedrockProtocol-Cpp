/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/ClientboundDataStorePacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/ClientboundDataStorePacket.h"

#include <memory>
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

ClientboundDataStorePacket ClientboundDataStorePacket::create(std::vector<std::unique_ptr<types::ddui::DataStoreOperation>> values)
{
    ClientboundDataStorePacket result;
    result.values = std::move(values);
    return result;
}

void ClientboundDataStorePacket::decodePayload(encoding::ByteBufferReader &in)
{
    values.clear();
    for (std::uint32_t i = 0, len = encoding::VarInt::readUnsignedInt(in); i < len; ++i) {
        switch (encoding::VarInt::readUnsignedInt(in)) {
        case types::ddui::DataStoreOperationType::UPDATE:
            values.push_back(std::make_unique<types::ddui::DataStoreUpdate>(types::ddui::DataStoreUpdate::read(in)));
            break;
        case types::ddui::DataStoreOperationType::CHANGE:
            values.push_back(std::make_unique<types::ddui::DataStoreChange>(types::ddui::DataStoreChange::read(in)));
            break;
        case types::ddui::DataStoreOperationType::REMOVAL:
            values.push_back(std::make_unique<types::ddui::DataStoreRemoval>(types::ddui::DataStoreRemoval::read(in)));
            break;
        default:
            throw PacketDecodeException("Unknown DataStore type");
        }
    }

}

void ClientboundDataStorePacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(values.size()));
    for (const auto &value : values) {
        encoding::VarInt::writeUnsignedInt(out, value->getTypeId());
        value->write(out);
    }

}

bool ClientboundDataStorePacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleClientboundDataStore(*this);
}

}  // namespace bedrock_protocol
