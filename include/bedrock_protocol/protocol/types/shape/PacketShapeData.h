/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PacketShapeData.php
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
#include <string>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/math/Vector2.h"
#include "bedrock_protocol/math/Vector3.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

/**
 * @see PrimitiveShapesPacket
 */
class PacketShapeData final {
public:
    PacketShapeData(std::uint64_t networkId, std::optional<PrimitiveShapeType> type,
                    std::optional<math::Vector3> location, std::optional<float> scale,
                    std::optional<math::Vector3> rotation, std::optional<float> totalTimeLeft,
                    std::optional<float> maximumRenderDistance, std::optional<color::Color> color,
                    std::optional<std::int32_t> dimensionId, std::optional<std::uint64_t> attachedToEntityId,
                    std::unique_ptr<PrimitiveShapePayload> payload);

    static PacketShapeData remove(std::uint64_t networkId, std::optional<std::int32_t> dimensionId = std::nullopt);

    static PacketShapeData line(std::uint64_t networkId, const math::Vector3 &location,
                                const math::Vector3 &lineEndLocation,
                                const std::optional<color::Color> &color = std::nullopt,
                                std::optional<std::int32_t> dimensionId = std::nullopt,
                                std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData box(std::uint64_t networkId, const math::Vector3 &location, const math::Vector3 &boxBound,
                               std::optional<float> scale = std::nullopt,
                               const std::optional<color::Color> &color = std::nullopt,
                               std::optional<std::int32_t> dimensionId = std::nullopt,
                               std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData sphere(std::uint64_t networkId, const math::Vector3 &location, std::uint8_t segments,
                                  std::optional<float> scale = std::nullopt,
                                  const std::optional<color::Color> &color = std::nullopt,
                                  std::optional<std::int32_t> dimensionId = std::nullopt,
                                  std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData circle(std::uint64_t networkId, const math::Vector3 &location, std::uint8_t segments,
                                  std::optional<float> scale = std::nullopt,
                                  const std::optional<color::Color> &color = std::nullopt,
                                  std::optional<std::int32_t> dimensionId = std::nullopt,
                                  std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData text(std::uint64_t networkId, const math::Vector3 &location, std::string text,
                                bool useRotation = false,
                                const std::optional<color::Color> &backgroundColor = std::nullopt,
                                bool depthTest = true, bool showBackface = true, bool showTextBackface = true,
                                const std::optional<color::Color> &color = std::nullopt,
                                std::optional<std::int32_t> dimensionId = std::nullopt,
                                std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData arrow(std::uint64_t networkId, const math::Vector3 &location,
                                 const math::Vector3 &lineEndLocation, std::optional<float> scale = std::nullopt,
                                 const std::optional<color::Color> &color = std::nullopt,
                                 std::optional<float> arrowHeadLength = std::nullopt,
                                 std::optional<float> arrowHeadRadius = std::nullopt,
                                 std::optional<std::uint8_t> segments = std::nullopt,
                                 std::optional<std::int32_t> dimensionId = std::nullopt,
                                 std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData cylinder(std::uint64_t networkId, const math::Vector3 &location,
                                    const math::Vector2 &radiusX, const math::Vector2 &radiusZ, float height,
                                    std::uint8_t segments, const std::optional<color::Color> &color = std::nullopt,
                                    std::optional<std::int32_t> dimensionId = std::nullopt,
                                    std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData pyramid(std::uint64_t networkId, const math::Vector3 &location, float width, float height,
                                   std::optional<float> depth = std::nullopt,
                                   const std::optional<color::Color> &color = std::nullopt,
                                   std::optional<std::int32_t> dimensionId = std::nullopt,
                                   std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData ellipsoid(std::uint64_t networkId, const math::Vector3 &location, const math::Vector3 &radii,
                                     std::uint8_t segmentsPerAxis,
                                     const std::optional<color::Color> &color = std::nullopt,
                                     std::optional<std::int32_t> dimensionId = std::nullopt,
                                     std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    static PacketShapeData cone(std::uint64_t networkId, const math::Vector3 &location, const math::Vector2 &radii,
                                float height, std::uint8_t segments,
                                const std::optional<color::Color> &color = std::nullopt,
                                std::optional<std::int32_t> dimensionId = std::nullopt,
                                std::optional<std::uint64_t> attachedToEntityId = std::nullopt);

    [[nodiscard]] std::uint64_t getNetworkId() const { return networkId; }

    [[nodiscard]] std::optional<PrimitiveShapeType> getType() const { return type; }

    [[nodiscard]] const std::optional<math::Vector3> &getLocation() const { return location; }

    [[nodiscard]] std::optional<float> getScale() const { return scale; }

    [[nodiscard]] const std::optional<math::Vector3> &getRotation() const { return rotation; }

    [[nodiscard]] std::optional<float> getTotalTimeLeft() const { return totalTimeLeft; }

    [[nodiscard]] std::optional<float> getMaximumRenderDistance() const { return maximumRenderDistance; }

    [[nodiscard]] const std::optional<color::Color> &getColor() const { return color; }

    [[nodiscard]] std::optional<std::int32_t> getDimensionId() const { return dimensionId; }

    [[nodiscard]] std::optional<std::uint64_t> getAttachedToEntityId() const { return attachedToEntityId; }

    [[nodiscard]] const PrimitiveShapePayload *getPayload() const { return payload.get(); }

    /**
     * @throws PacketDecodeException
     * @throws DataDecodeException
     */
    static PacketShapeData read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::uint64_t networkId;
    std::optional<PrimitiveShapeType> type;
    std::optional<math::Vector3> location;
    std::optional<float> scale;
    std::optional<math::Vector3> rotation;
    std::optional<float> totalTimeLeft;
    std::optional<float> maximumRenderDistance;
    std::optional<color::Color> color;
    std::optional<std::int32_t> dimensionId;
    std::optional<std::uint64_t> attachedToEntityId;
    std::unique_ptr<PrimitiveShapePayload> payload;
};

}  // namespace bedrock_protocol::types::shape
