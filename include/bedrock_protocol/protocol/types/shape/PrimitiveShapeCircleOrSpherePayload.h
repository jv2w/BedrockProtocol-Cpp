/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeCircleOrSpherePayload.php
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
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

class PrimitiveShapeCircleOrSpherePayload final : public PrimitiveShapePayload {
public:
    static constexpr std::int32_t ID = PrimitiveShapePayloadType::PAYLOAD_TYPE_CIRCLE_OR_SPHERE;

    explicit PrimitiveShapeCircleOrSpherePayload(const std::uint8_t segments) : segments(segments) {}

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] std::uint8_t getSegments() const { return segments; }

    /**
     * @throws DataDecodeException
     */
    static PrimitiveShapeCircleOrSpherePayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<PrimitiveShapePayload> clone() const override
    {
        return std::make_unique<PrimitiveShapeCircleOrSpherePayload>(*this);
    }

private:
    std::uint8_t segments;
};

}  // namespace bedrock_protocol::types::shape
