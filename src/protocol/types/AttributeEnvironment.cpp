/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/AttributeEnvironment.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/AttributeEnvironment.h"

#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"

namespace bedrock_protocol::types {

using encoding::LE;
using serializer::CommonTypes;

AttributeEnvironment AttributeEnvironment::read(encoding::ByteBufferReader &in)
{
    auto name = CommonTypes::getString(in);
    //readOptional expanded by hand: AttributeValue is polymorphic and is therefore held by pointer, not optional
    std::unique_ptr<AttributeValue> fromAttribute;
    if (CommonTypes::getBool(in)) {
        fromAttribute = AttributeValue::read(in);
    }
    auto attribute = AttributeValue::read(in);
    std::unique_ptr<AttributeValue> toAttribute;
    if (CommonTypes::getBool(in)) {
        toAttribute = AttributeValue::read(in);
    }
    const auto currentTransitionTicks = LE::readUnsignedInt(in);
    const auto totalTransitionTicks = LE::readUnsignedInt(in);
    auto easeType = CommonTypes::getString(in);
    const auto localTransitionTicks = LE::readUnsignedInt(in);
    const auto noiseTransition = CommonTypes::getBool(in);

    return AttributeEnvironment(std::move(name), std::move(fromAttribute), std::move(attribute),
                                std::move(toAttribute), currentTransitionTicks, totalTransitionTicks,
                                std::move(easeType), localTransitionTicks, noiseTransition);
}

void AttributeEnvironment::write(encoding::ByteBufferWriter &out) const
{
    CommonTypes::putString(out, name);
    //writeOptional expanded by hand: AttributeValue is polymorphic and is therefore held by pointer, not optional
    CommonTypes::putBool(out, fromAttribute != nullptr);
    if (fromAttribute != nullptr) {
        fromAttribute->write(out);
    }
    attribute->write(out);
    CommonTypes::putBool(out, toAttribute != nullptr);
    if (toAttribute != nullptr) {
        toAttribute->write(out);
    }
    LE::writeUnsignedInt(out, currentTransitionTicks);
    LE::writeUnsignedInt(out, totalTransitionTicks);
    CommonTypes::putString(out, easeType);
    LE::writeUnsignedInt(out, localTransitionTicks);
    CommonTypes::putBool(out, noiseTransition);
}

}  // namespace bedrock_protocol::types
