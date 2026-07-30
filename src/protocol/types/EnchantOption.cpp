/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/EnchantOption.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/EnchantOption.h"

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

std::vector<Enchant> EnchantOption::readEnchantList(encoding::ByteBufferReader &in)
{
    std::vector<Enchant> result;
    for (std::uint32_t i = 0, len = VarInt::readUnsignedInt(in); i < len; ++i) {
        result.push_back(Enchant::read(in));
    }
    return result;
}

void EnchantOption::writeEnchantList(encoding::ByteBufferWriter &out, const std::vector<Enchant> &list)
{
    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(list.size()));
    for (const auto &item : list) {
        item.write(out);
    }
}

EnchantOption EnchantOption::read(encoding::ByteBufferReader &in)
{
    const auto cost = Byte::readUnsigned(in);

    const auto slotFlags = LE::readUnsignedInt(in);
    auto equipActivatedEnchants = readEnchantList(in);
    auto heldActivatedEnchants = readEnchantList(in);
    auto selfActivatedEnchants = readEnchantList(in);

    auto name = CommonTypes::getString(in);
    const auto optionId = CommonTypes::readRecipeNetId(in);

    return EnchantOption(cost, slotFlags, std::move(equipActivatedEnchants), std::move(heldActivatedEnchants),
                         std::move(selfActivatedEnchants), std::move(name), optionId);
}

void EnchantOption::write(encoding::ByteBufferWriter &out) const
{
    Byte::writeUnsigned(out, cost);

    LE::writeUnsignedInt(out, slotFlags);
    writeEnchantList(out, equipActivatedEnchantments);
    writeEnchantList(out, heldActivatedEnchantments);
    writeEnchantList(out, selfActivatedEnchantments);

    CommonTypes::putString(out, name);
    CommonTypes::writeRecipeNetId(out, optionId);
}

}  // namespace bedrock_protocol::types
