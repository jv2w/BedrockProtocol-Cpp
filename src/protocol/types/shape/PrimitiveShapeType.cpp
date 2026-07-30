/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

#include <stdexcept>
#include <string>

#include "bedrock_protocol/protocol/PacketDecodeException.h"

namespace bedrock_protocol::types::shape {

PrimitiveShapeType PrimitiveShapeTypeFromPacket(const std::int32_t value)
{
    switch (value) {
    case 0:
        return PrimitiveShapeType::LINE;
    case 1:
        return PrimitiveShapeType::BOX;
    case 2:
        return PrimitiveShapeType::SPHERE;
    case 3:
        return PrimitiveShapeType::CIRCLE;
    case 4:
        return PrimitiveShapeType::TEXT;
    case 5:
        return PrimitiveShapeType::ARROW;
    case 6:
        return PrimitiveShapeType::CYLINDER;
    case 7:
        return PrimitiveShapeType::PYRAMID;
    case 8:
        return PrimitiveShapeType::ELLIPSOID;
    case 9:
        return PrimitiveShapeType::CONE;
    default:
        throw PacketDecodeException("Invalid raw value " + std::to_string(value) + " for PrimitiveShapeType");
    }
}

std::int32_t getPayloadType(const PrimitiveShapeType type)
{
    switch (type) {
    case PrimitiveShapeType::ARROW:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_ARROW;
    case PrimitiveShapeType::TEXT:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_TEXT;
    case PrimitiveShapeType::BOX:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_BOX;
    case PrimitiveShapeType::LINE:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_LINE;
    case PrimitiveShapeType::CIRCLE:
    case PrimitiveShapeType::SPHERE:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_CIRCLE_OR_SPHERE;
    case PrimitiveShapeType::CYLINDER:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_CYLINDER;
    case PrimitiveShapeType::PYRAMID:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_PYRAMID;
    case PrimitiveShapeType::ELLIPSOID:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_ELLIPSOID;
    case PrimitiveShapeType::CONE:
        return PrimitiveShapePayloadType::PAYLOAD_TYPE_CONE;
    default:
        throw std::logic_error("Unhandled PrimitiveShapeType");
    }
}

}  // namespace bedrock_protocol::types::shape
