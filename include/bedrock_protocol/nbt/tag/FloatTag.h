/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/FloatTag.php
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

#include "bedrock_protocol/nbt/tag/ImmutableTag.h"

namespace bedrock_protocol::nbt {
class NbtStreamReader;
class NbtStreamWriter;
}  // namespace bedrock_protocol::nbt

namespace bedrock_protocol::nbt::tag {

class FloatTag final : public ImmutableTag {
public:
    explicit FloatTag(float value) : value(value) {}

    [[nodiscard]] float getValue() const { return value; }

    [[nodiscard]] std::int32_t getType() const override;

    static FloatTag read(NbtStreamReader &reader);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    float value;
};

}  // namespace bedrock_protocol::nbt::tag
