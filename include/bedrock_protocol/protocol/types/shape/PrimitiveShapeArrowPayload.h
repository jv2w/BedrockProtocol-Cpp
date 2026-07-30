/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeArrowPayload.php
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
#include <optional>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

class PrimitiveShapeArrowPayload final : public PrimitiveShapePayload {
public:
    static constexpr std::int32_t ID = PrimitiveShapePayloadType::PAYLOAD_TYPE_ARROW;

    PrimitiveShapeArrowPayload(std::optional<math::Vector3> lineEndLocation, std::optional<float> arrowHeadLength,
                               std::optional<float> arrowHeadRadius, std::optional<std::uint8_t> segments)
        : lineEndLocation(lineEndLocation), arrowHeadLength(arrowHeadLength), arrowHeadRadius(arrowHeadRadius),
          segments(segments)
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::optional<math::Vector3> &getLineEndLocation() const { return lineEndLocation; }

    [[nodiscard]] std::optional<float> getArrowHeadLength() const { return arrowHeadLength; }

    [[nodiscard]] std::optional<float> getArrowHeadRadius() const { return arrowHeadRadius; }

    [[nodiscard]] std::optional<std::uint8_t> getSegments() const { return segments; }

    /**
     * @throws DataDecodeException
     */
    static PrimitiveShapeArrowPayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<PrimitiveShapePayload> clone() const override
    {
        return std::make_unique<PrimitiveShapeArrowPayload>(*this);
    }

private:
    std::optional<math::Vector3> lineEndLocation;
    std::optional<float> arrowHeadLength;
    std::optional<float> arrowHeadRadius;
    std::optional<std::uint8_t> segments;
};

}  // namespace bedrock_protocol::types::shape
