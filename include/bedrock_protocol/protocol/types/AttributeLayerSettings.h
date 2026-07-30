/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/AttributeLayerSettings.php
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
#include <utility>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/AttributeLayerSettingsWeight.h"

namespace bedrock_protocol::types {

/**
 * @see AttributeLayer&AttributeUpdateLayerSettings
 */
class AttributeLayerSettings final {
public:
    AttributeLayerSettings(std::int32_t priority, std::unique_ptr<AttributeLayerSettingsWeight> weight, bool enabled,
                           bool transitionsPaused)
        : priority(priority), weight(std::move(weight)), enabled(enabled), transitionsPaused(transitionsPaused)
    {
    }

    /**
     * Not present in the PHP original; deep-copies the polymorphic weight so that this type keeps PHP's value
     * semantics and can be stored in copyable containers.
     */
    AttributeLayerSettings(const AttributeLayerSettings &other)
        : priority(other.priority), weight(other.weight != nullptr ? other.weight->clone() : nullptr),
          enabled(other.enabled), transitionsPaused(other.transitionsPaused)
    {
    }

    AttributeLayerSettings &operator=(const AttributeLayerSettings &other)
    {
        if (this != &other) {
            AttributeLayerSettings copy(other);
            *this = std::move(copy);
        }
        return *this;
    }

    AttributeLayerSettings(AttributeLayerSettings &&) noexcept = default;
    AttributeLayerSettings &operator=(AttributeLayerSettings &&) noexcept = default;
    ~AttributeLayerSettings() = default;

    [[nodiscard]] std::int32_t getPriority() const { return priority; }

    [[nodiscard]] const AttributeLayerSettingsWeight &getWeight() const { return *weight; }

    [[nodiscard]] bool isEnabled() const { return enabled; }

    [[nodiscard]] bool isTransitionsPaused() const { return transitionsPaused; }

    /**
     * @throws PacketDecodeException
     */
    static AttributeLayerSettings read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::int32_t priority;
    std::unique_ptr<AttributeLayerSettingsWeight> weight;
    bool enabled;
    bool transitionsPaused;
};

}  // namespace bedrock_protocol::types
