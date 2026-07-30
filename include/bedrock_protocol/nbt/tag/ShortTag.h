/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ShortTag.php
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

class ShortTag final : public ImmutableTag {
public:
    /**
     * @throws InvalidTagValueException if the value is outside the allowed range
     */
    explicit ShortTag(std::int32_t value);

    static constexpr std::int32_t min() { return -0x8000; }

    static constexpr std::int32_t max() { return 0x7fff; }

    [[nodiscard]] std::int16_t getValue() const { return value; }

    [[nodiscard]] std::int32_t getType() const override;

    static ShortTag read(NbtStreamReader &reader);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    std::int16_t value;
};

}  // namespace bedrock_protocol::nbt::tag
