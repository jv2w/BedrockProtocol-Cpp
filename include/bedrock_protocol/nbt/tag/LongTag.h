/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: pocketmine/nbt src/tag/LongTag.php
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

class LongTag final : public ImmutableTag {
public:
    /**
     * Deviation from PHP: the PHP constructor range-checks the value against min()/max(). Here the
     * parameter is already exactly 64 bits wide, so no value can ever fall outside the range and
     * the check is omitted.
     */
    explicit LongTag(std::int64_t value);

    static constexpr std::int64_t min() { return -0x7fffffffffffffffLL - 1; }

    static constexpr std::int64_t max() { return 0x7fffffffffffffffLL; }

    [[nodiscard]] std::int64_t getValue() const { return value; }

    [[nodiscard]] std::int32_t getType() const override;

    static LongTag read(NbtStreamReader &reader);

    void write(NbtStreamWriter &writer) const override;

    [[nodiscard]] std::unique_ptr<Tag> clone() const override;

    [[nodiscard]] bool equals(const Tag &that) const override;

protected:
    [[nodiscard]] std::string getTypeName() const override;

    [[nodiscard]] std::string stringifyValue(int indentation) const override;

private:
    std::int64_t value;
};

}  // namespace bedrock_protocol::nbt::tag
