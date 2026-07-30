/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::shape {

enum class PrimitiveShapeType : std::int32_t {
    LINE = 0,
    BOX = 1,
    SPHERE = 2,
    CIRCLE = 3,
    TEXT = 4,
    ARROW = 5,
    CYLINDER = 6,
    PYRAMID = 7,
    ELLIPSOID = 8,
    CONE = 9,
};

/**
 * @throws PacketDecodeException
 */
PrimitiveShapeType PrimitiveShapeTypeFromPacket(std::int32_t value);

/**
 * Holder for the PAYLOAD_TYPE_* constants declared on the PHP enum itself
 * (a C++ `enum class` cannot carry class constants).
 */
struct PrimitiveShapePayloadType final {
    PrimitiveShapePayloadType() = delete;

    static constexpr std::int32_t PAYLOAD_TYPE_NONE = 0;
    static constexpr std::int32_t PAYLOAD_TYPE_ARROW = 1;
    static constexpr std::int32_t PAYLOAD_TYPE_TEXT = 2;
    static constexpr std::int32_t PAYLOAD_TYPE_BOX = 3;
    static constexpr std::int32_t PAYLOAD_TYPE_LINE = 4;
    static constexpr std::int32_t PAYLOAD_TYPE_CIRCLE_OR_SPHERE = 5;
    static constexpr std::int32_t PAYLOAD_TYPE_CYLINDER = 6;
    static constexpr std::int32_t PAYLOAD_TYPE_PYRAMID = 7;
    static constexpr std::int32_t PAYLOAD_TYPE_ELLIPSOID = 8;
    static constexpr std::int32_t PAYLOAD_TYPE_CONE = 9;
};

/**
 * UGH
 */
std::int32_t getPayloadType(PrimitiveShapeType type);

}  // namespace bedrock_protocol::types::shape
