/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/ParameterKeyframeValue.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"

namespace bedrock_protocol::types {

class ParameterKeyframeValue final {
public:
    ParameterKeyframeValue(float time, const math::Vector3 &value) : time(time), value(value) {}

    [[nodiscard]] float getTime() const { return time; }

    [[nodiscard]] const math::Vector3 &getValue() const { return value; }

    static ParameterKeyframeValue read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    float time;
    math::Vector3 value;
};

}  // namespace bedrock_protocol::types
