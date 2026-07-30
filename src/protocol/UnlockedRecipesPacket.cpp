/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/UnlockedRecipesPacket.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/UnlockedRecipesPacket.h"

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

UnlockedRecipesPacket UnlockedRecipesPacket::create(std::uint32_t type, std::vector<std::string> recipes)
{
    UnlockedRecipesPacket result;
    result.type = type;
    result.recipes = std::move(recipes);
    return result;
}

void UnlockedRecipesPacket::decodePayload(encoding::ByteBufferReader &in)
{
    type = encoding::LE::readUnsignedInt(in);
    recipes.clear();
    for (std::uint32_t i = 0, count = encoding::VarInt::readUnsignedInt(in); i < count; ++i) {
        recipes.push_back(serializer::CommonTypes::getString(in));
    }

}

void UnlockedRecipesPacket::encodePayload(encoding::ByteBufferWriter &out) const
{
    encoding::LE::writeUnsignedInt(out, type);
    encoding::VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(recipes.size()));
    for (const auto &recipe : recipes) {
        serializer::CommonTypes::putString(out, recipe);
    }

}

bool UnlockedRecipesPacket::handle(PacketHandlerInterface &handler)
{
    return handler.handleUnlockedRecipes(*this);
}

}  // namespace bedrock_protocol
