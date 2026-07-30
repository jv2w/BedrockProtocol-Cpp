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

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeValue.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeLayer&AttributesUpdateEnvironment
 */
class AttributeEnvironment final {
public:
    AttributeEnvironment(std::string name, std::unique_ptr<AttributeValue> fromAttribute,
                         std::unique_ptr<AttributeValue> attribute, std::unique_ptr<AttributeValue> toAttribute,
                         std::uint32_t currentTransitionTicks, std::uint32_t totalTransitionTicks,
                         std::string easeType, std::uint32_t localTransitionTicks, bool noiseTransition)
        : name(std::move(name)), fromAttribute(std::move(fromAttribute)), attribute(std::move(attribute)),
          toAttribute(std::move(toAttribute)), currentTransitionTicks(currentTransitionTicks),
          totalTransitionTicks(totalTransitionTicks), easeType(std::move(easeType)),
          localTransitionTicks(localTransitionTicks), noiseTransition(noiseTransition)
    {
    }

    /**
     * Not present in the PHP original; deep-copies the polymorphic members so that this type keeps PHP's value
     * semantics and can be stored in copyable containers.
     */
    AttributeEnvironment(const AttributeEnvironment &other)
        : name(other.name), fromAttribute(other.fromAttribute != nullptr ? other.fromAttribute->clone() : nullptr),
          attribute(other.attribute != nullptr ? other.attribute->clone() : nullptr),
          toAttribute(other.toAttribute != nullptr ? other.toAttribute->clone() : nullptr),
          currentTransitionTicks(other.currentTransitionTicks), totalTransitionTicks(other.totalTransitionTicks),
          easeType(other.easeType), localTransitionTicks(other.localTransitionTicks),
          noiseTransition(other.noiseTransition)
    {
    }

    AttributeEnvironment &operator=(const AttributeEnvironment &other)
    {
        if (this != &other) {
            AttributeEnvironment copy(other);
            *this = std::move(copy);
        }
        return *this;
    }

    AttributeEnvironment(AttributeEnvironment &&) noexcept = default;
    AttributeEnvironment &operator=(AttributeEnvironment &&) noexcept = default;
    ~AttributeEnvironment() = default;

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] const AttributeValue *getFromAttribute() const { return fromAttribute.get(); }

    [[nodiscard]] const AttributeValue &getAttribute() const { return *attribute; }

    [[nodiscard]] const AttributeValue *getToAttribute() const { return toAttribute.get(); }

    [[nodiscard]] std::uint32_t getCurrentTransitionTicks() const { return currentTransitionTicks; }

    [[nodiscard]] std::uint32_t getTotalTransitionTicks() const { return totalTransitionTicks; }

    /**
     * @see CameraSetInstructionEaseType
     */
    [[nodiscard]] const std::string &getEaseType() const { return easeType; }

    [[nodiscard]] std::uint32_t getLocalTransitionTicks() const { return localTransitionTicks; }

    [[nodiscard]] bool isNoiseTransition() const { return noiseTransition; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeEnvironment read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::string name;
    std::unique_ptr<AttributeValue> fromAttribute;
    std::unique_ptr<AttributeValue> attribute;
    std::unique_ptr<AttributeValue> toAttribute;
    std::uint32_t currentTransitionTicks;
    std::uint32_t totalTransitionTicks;
    std::string easeType;
    std::uint32_t localTransitionTicks;
    bool noiseTransition;
};

}  // namespace bedrock_protocol::types
