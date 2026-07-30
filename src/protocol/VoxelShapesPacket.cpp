/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/VoxelShapesPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/VoxelShapesPacket.h"

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

VoxelShapesPacket VoxelShapesPacket::create(std::vector<types::SerializableVoxelShape> shapes, std::vector<std::pair<std::string, std::uint16_t>> nameMap, std::uint16_t customShapeCount)
{
    VoxelShapesPacket result;
    result.shapes = std::move(shapes);
    result.nameMap = std::move(nameMap);
    result.customShapeCount = customShapeCount;
    return result;
}

void VoxelShapesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    shapes.clear();
    for (std::uint32_t i = 0, shapesCount = encoding::VarInt::readUnsignedInt(in); i < shapesCount; ++i) {
        shapes.push_back(types::SerializableVoxelShape::read(in));
    }

    nameMap.clear();
    for (std::uint32_t i = 0, namesCount = encoding::VarInt::readUnsignedInt(in); i < namesCount; ++i) {
        const auto name = serializer::CommonTypes::getString(in);
        const auto id = encoding::LE::readUnsignedShort(in);
        nameMap.emplace_back(name, id);
    }

    customShapeCount = encoding::LE::readUnsignedShort(in);

}

void VoxelShapesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(shapes.size()));
    for (const auto &shape : shapes) {
        shape.write(out);
    }

    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(nameMap.size()));
    for (const auto &[name, id] : nameMap) {
        serializer::CommonTypes::putString(out, name);
        encoding::LE::writeUnsignedShort(out, id);
    }

    encoding::LE::writeUnsignedShort(out, customShapeCount);

}

bool VoxelShapesPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleVoxelShapes(*this);
}

}  // namespace bedrock_protocol
