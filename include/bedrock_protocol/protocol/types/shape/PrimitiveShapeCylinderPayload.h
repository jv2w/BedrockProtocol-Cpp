/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeCylinderPayload.php
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

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

class PrimitiveShapeCylinderPayload final : public PrimitiveShapePayload {
public:
    static constexpr std::int32_t ID = PrimitiveShapePayloadType::PAYLOAD_TYPE_CYLINDER;

    PrimitiveShapeCylinderPayload(const math::Vector2 &radiusX, const math::Vector2 &radiusZ, const float height,
                                  const std::uint8_t segments)
        : radiusX(radiusX), radiusZ(radiusZ), height(height), segments(segments)
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const math::Vector2 &getRadiusX() const { return radiusX; }

    [[nodiscard]] const math::Vector2 &getRadiusZ() const { return radiusZ; }

    [[nodiscard]] float getHeight() const { return height; }

    [[nodiscard]] std::uint8_t getSegments() const { return segments; }

    /**
     * @throws DataDecodeException
     */
    static PrimitiveShapeCylinderPayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<PrimitiveShapePayload> clone() const override
    {
        return std::make_unique<PrimitiveShapeCylinderPayload>(*this);
    }

private:
    math::Vector2 radiusX;
    math::Vector2 radiusZ;
    float height;
    std::uint8_t segments;
};

}  // namespace bedrock_protocol::types::shape
