/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeEllipsoidPayload.php
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
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

class PrimitiveShapeEllipsoidPayload final : public PrimitiveShapePayload {
public:
    static constexpr std::int32_t ID = PrimitiveShapePayloadType::PAYLOAD_TYPE_ELLIPSOID;

    PrimitiveShapeEllipsoidPayload(const math::Vector3 &radii, const std::uint8_t segmentsPerAxis)
        : radii(radii), segmentsPerAxis(segmentsPerAxis)
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const math::Vector3 &getRadii() const { return radii; }

    [[nodiscard]] std::uint8_t getSegmentsPerAxis() const { return segmentsPerAxis; }

    /**
     * @throws DataDecodeException
     */
    static PrimitiveShapeEllipsoidPayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<PrimitiveShapePayload> clone() const override
    {
        return std::make_unique<PrimitiveShapeEllipsoidPayload>(*this);
    }

private:
    math::Vector3 radii;
    std::uint8_t segmentsPerAxis;
};

}  // namespace bedrock_protocol::types::shape
