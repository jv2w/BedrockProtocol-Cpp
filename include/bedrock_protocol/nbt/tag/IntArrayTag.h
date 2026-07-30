/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/IntArrayTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/nbt/tag/ImmutableTag.h"

namespace bedrock_protocol::nbt {
class NbtStreamReader;
class NbtStreamWriter;
}  // namespace bedrock_protocol::nbt

namespace bedrock_protocol::nbt::tag {

class IntArrayTag final : public ImmutableTag {
public:
    explicit IntArrayTag(std::vector<std::int32_t> value) : value(std::move(value)) {}

    [[nodiscard]] const std::vector<std::int32_t> &getValue() const { return value; }

    [[nodiscard]] std::int32_t getType() const override;

    static IntArrayTag read(NbtStreamReader &reader);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    std::vector<std::int32_t> value;
};

}  // namespace bedrock_protocol::nbt::tag
