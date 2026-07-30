/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/EnchantOption.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/Enchant.h"

namespace bedrock_protocol::types {

class EnchantOption final {
public:
    EnchantOption(std::uint8_t cost, std::uint32_t slotFlags, std::vector<Enchant> equipActivatedEnchantments,
                  std::vector<Enchant> heldActivatedEnchantments, std::vector<Enchant> selfActivatedEnchantments,
                  std::string name, std::uint32_t optionId)
        : cost(cost), slotFlags(slotFlags), equipActivatedEnchantments(std::move(equipActivatedEnchantments)),
          heldActivatedEnchantments(std::move(heldActivatedEnchantments)),
          selfActivatedEnchantments(std::move(selfActivatedEnchantments)), name(std::move(name)), optionId(optionId)
    {
    }

    [[nodiscard]] std::uint8_t getCost() const { return cost; }

    [[nodiscard]] std::uint32_t getSlotFlags() const { return slotFlags; }

    [[nodiscard]] const std::vector<Enchant> &getEquipActivatedEnchantments() const
    {
        return equipActivatedEnchantments;
    }

    [[nodiscard]] const std::vector<Enchant> &getHeldActivatedEnchantments() const { return heldActivatedEnchantments; }

    [[nodiscard]] const std::vector<Enchant> &getSelfActivatedEnchantments() const { return selfActivatedEnchantments; }

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] std::uint32_t getOptionId() const { return optionId; }

    /** @throws DataDecodeException */
    static EnchantOption read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    /** @throws DataDecodeException */
    static std::vector<Enchant> readEnchantList(encoding::ByteBufferReader &in);

    static void writeEnchantList(encoding::ByteBufferWriter &out, const std::vector<Enchant> &list);

    std::uint8_t cost;
    std::uint32_t slotFlags;
    std::vector<Enchant> equipActivatedEnchantments;
    std::vector<Enchant> heldActivatedEnchantments;
    std::vector<Enchant> selfActivatedEnchantments;
    std::string name;
    std::uint32_t optionId;
};

}  // namespace bedrock_protocol::types
