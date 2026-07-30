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

#include "bedrock_protocol/protocol/types/shape/PacketShapeData.h"

#include <utility>

#include "bedrock_protocol/encoding/Byte.h"
#include "bedrock_protocol/encoding/LE.h"
#include "bedrock_protocol/encoding/VarInt.h"
#include "bedrock_protocol/protocol/PacketDecodeException.h"
#include "bedrock_protocol/protocol/serializer/CommonTypes.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeArrowPayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeBoxPayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeCircleOrSpherePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeConePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeCylinderPayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeEllipsoidPayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeLinePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePyramidPayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeTextPayload.h"

namespace bedrock_protocol::types::shape {

using encoding::Byte;
using encoding::LE;
using encoding::VarInt;
using serializer::CommonTypes;

namespace {

/** Stands in for PHP's enum ->name property, used to build the decode error message. */
const char *shapeTypeName(const PrimitiveShapeType type)
{
    switch (type) {
    case PrimitiveShapeType::LINE:
        return "LINE";
    case PrimitiveShapeType::BOX:
        return "BOX";
    case PrimitiveShapeType::SPHERE:
        return "SPHERE";
    case PrimitiveShapeType::CIRCLE:
        return "CIRCLE";
    case PrimitiveShapeType::TEXT:
        return "TEXT";
    case PrimitiveShapeType::ARROW:
        return "ARROW";
    case PrimitiveShapeType::CYLINDER:
        return "CYLINDER";
    case PrimitiveShapeType::PYRAMID:
        return "PYRAMID";
    case PrimitiveShapeType::ELLIPSOID:
        return "ELLIPSOID";
    case PrimitiveShapeType::CONE:
        return "CONE";
    default:
        return "(not set)";
    }
}

}  // namespace

PacketShapeData::PacketShapeData(const std::uint64_t networkId, const std::optional<PrimitiveShapeType> type,
                                 std::optional<math::Vector3> location, const std::optional<float> scale,
                                 std::optional<math::Vector3> rotation, const std::optional<float> totalTimeLeft,
                                 const std::optional<float> maximumRenderDistance, std::optional<::bedrock_protocol::color::Color> color,
                                 const std::optional<std::int32_t> dimensionId,
                                 const std::optional<std::uint64_t> attachedToEntityId,
                                 std::unique_ptr<PrimitiveShapePayload> payload)
    : networkId(networkId), type(type), location(std::move(location)), scale(scale), rotation(std::move(rotation)),
      totalTimeLeft(totalTimeLeft), maximumRenderDistance(maximumRenderDistance), color(std::move(color)),
      dimensionId(dimensionId), attachedToEntityId(attachedToEntityId), payload(std::move(payload))
{
}

PacketShapeData PacketShapeData::remove(const std::uint64_t networkId, const std::optional<std::int32_t> dimensionId)
{
    return PacketShapeData{networkId,    std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                           std::nullopt, std::nullopt, dimensionId,  std::nullopt, nullptr};
}

PacketShapeData PacketShapeData::line(const std::uint64_t networkId, const math::Vector3 &location,
                                      const math::Vector3 &lineEndLocation, const std::optional<::bedrock_protocol::color::Color> &color,
                                      const std::optional<std::int32_t> dimensionId,
                                      const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::LINE,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeLinePayload>(lineEndLocation)};
}

PacketShapeData PacketShapeData::box(const std::uint64_t networkId, const math::Vector3 &location,
                                     const math::Vector3 &boxBound, const std::optional<float> scale,
                                     const std::optional<::bedrock_protocol::color::Color> &color,
                                     const std::optional<std::int32_t> dimensionId,
                                     const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::BOX,
                           location,
                           scale,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeBoxPayload>(boxBound)};
}

PacketShapeData PacketShapeData::sphere(const std::uint64_t networkId, const math::Vector3 &location,
                                        const std::uint8_t segments, const std::optional<float> scale,
                                        const std::optional<::bedrock_protocol::color::Color> &color,
                                        const std::optional<std::int32_t> dimensionId,
                                        const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::SPHERE,
                           location,
                           scale,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeCircleOrSpherePayload>(segments)};
}

PacketShapeData PacketShapeData::circle(const std::uint64_t networkId, const math::Vector3 &location,
                                        const std::uint8_t segments, const std::optional<float> scale,
                                        const std::optional<::bedrock_protocol::color::Color> &color,
                                        const std::optional<std::int32_t> dimensionId,
                                        const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::CIRCLE,
                           location,
                           scale,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeCircleOrSpherePayload>(segments)};
}

PacketShapeData PacketShapeData::text(const std::uint64_t networkId, const math::Vector3 &location, std::string text,
                                      const bool useRotation, const std::optional<::bedrock_protocol::color::Color> &backgroundColor,
                                      const bool depthTest, const bool showBackface, const bool showTextBackface,
                                      const std::optional<::bedrock_protocol::color::Color> &color,
                                      const std::optional<std::int32_t> dimensionId,
                                      const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::TEXT,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeTextPayload>(std::move(text), useRotation, backgroundColor,
                                                                       depthTest, showBackface, showTextBackface)};
}

PacketShapeData PacketShapeData::arrow(const std::uint64_t networkId, const math::Vector3 &location,
                                       const math::Vector3 &lineEndLocation, const std::optional<float> scale,
                                       const std::optional<::bedrock_protocol::color::Color> &color,
                                       const std::optional<float> arrowHeadLength,
                                       const std::optional<float> arrowHeadRadius,
                                       const std::optional<std::uint8_t> segments,
                                       const std::optional<std::int32_t> dimensionId,
                                       const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::ARROW,
                           location,
                           scale,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeArrowPayload>(lineEndLocation, arrowHeadLength,
                                                                        arrowHeadRadius, segments)};
}

PacketShapeData PacketShapeData::cylinder(const std::uint64_t networkId, const math::Vector3 &location,
                                          const math::Vector2 &radiusX, const math::Vector2 &radiusZ,
                                          const float height, const std::uint8_t segments,
                                          const std::optional<::bedrock_protocol::color::Color> &color,
                                          const std::optional<std::int32_t> dimensionId,
                                          const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::CYLINDER,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeCylinderPayload>(radiusX, radiusZ, height, segments)};
}

PacketShapeData PacketShapeData::pyramid(const std::uint64_t networkId, const math::Vector3 &location,
                                         const float width, const float height, const std::optional<float> depth,
                                         const std::optional<::bedrock_protocol::color::Color> &color,
                                         const std::optional<std::int32_t> dimensionId,
                                         const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::PYRAMID,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapePyramidPayload>(width, depth, height)};
}

PacketShapeData PacketShapeData::ellipsoid(const std::uint64_t networkId, const math::Vector3 &location,
                                           const math::Vector3 &radii, const std::uint8_t segmentsPerAxis,
                                           const std::optional<::bedrock_protocol::color::Color> &color,
                                           const std::optional<std::int32_t> dimensionId,
                                           const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::ELLIPSOID,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeEllipsoidPayload>(radii, segmentsPerAxis)};
}

PacketShapeData PacketShapeData::cone(const std::uint64_t networkId, const math::Vector3 &location,
                                      const math::Vector2 &radii, const float height, const std::uint8_t segments,
                                      const std::optional<::bedrock_protocol::color::Color> &color,
                                      const std::optional<std::int32_t> dimensionId,
                                      const std::optional<std::uint64_t> attachedToEntityId)
{
    return PacketShapeData{networkId,
                           PrimitiveShapeType::CONE,
                           location,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           std::nullopt,
                           color,
                           dimensionId,
                           attachedToEntityId,
                           std::make_unique<PrimitiveShapeConePayload>(radii, height, segments)};
}

PacketShapeData PacketShapeData::read(encoding::ByteBufferReader &in)
{
    const auto networkId = VarInt::readUnsignedLong(in);
    const auto shapeType = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return PrimitiveShapeTypeFromPacket(static_cast<std::int32_t>(Byte::readUnsigned(in)));
    });
    auto location = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CommonTypes::getVector3(in);
    });
    const auto scale = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    auto rotation = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CommonTypes::getVector3(in);
    });
    const auto totalTimeLeft = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    const auto maximumRenderDistance = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return LE::readFloat(in);
    });
    auto color = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return ::bedrock_protocol::color::Color::fromARGB(LE::readUnsignedInt(in));
    });
    const auto dimensionId = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return VarInt::readSignedInt(in);
    });
    const auto attachedToEntityId = CommonTypes::readOptional(in, [](encoding::ByteBufferReader &in) {
        return CommonTypes::getActorRuntimeId(in);
    });

    const auto payloadType = static_cast<std::int32_t>(VarInt::readUnsignedInt(in));
    //WTF IS THIS HORROR SHOW
    if ((shapeType.has_value() && payloadType != getPayloadType(*shapeType) &&
         payloadType != PrimitiveShapePayloadType::PAYLOAD_TYPE_NONE) ||
        (!shapeType.has_value() && payloadType != PrimitiveShapePayloadType::PAYLOAD_TYPE_NONE)) {
        throw PacketDecodeException("Unexpected payload type " + std::to_string(payloadType) +
                                    " for provided shape type " +
                                    (shapeType.has_value() ? shapeTypeName(*shapeType) : "(not set)"));
    }

    std::unique_ptr<PrimitiveShapePayload> payload;
    switch (payloadType) {
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_NONE:
        payload = nullptr;
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_ARROW:
        payload = std::make_unique<PrimitiveShapeArrowPayload>(PrimitiveShapeArrowPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_TEXT:
        payload = std::make_unique<PrimitiveShapeTextPayload>(PrimitiveShapeTextPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_BOX:
        payload = std::make_unique<PrimitiveShapeBoxPayload>(PrimitiveShapeBoxPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_LINE:
        payload = std::make_unique<PrimitiveShapeLinePayload>(PrimitiveShapeLinePayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_CIRCLE_OR_SPHERE:
        payload = std::make_unique<PrimitiveShapeCircleOrSpherePayload>(PrimitiveShapeCircleOrSpherePayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_CYLINDER:
        payload = std::make_unique<PrimitiveShapeCylinderPayload>(PrimitiveShapeCylinderPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_PYRAMID:
        payload = std::make_unique<PrimitiveShapePyramidPayload>(PrimitiveShapePyramidPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_ELLIPSOID:
        payload = std::make_unique<PrimitiveShapeEllipsoidPayload>(PrimitiveShapeEllipsoidPayload::read(in));
        break;
    case PrimitiveShapePayloadType::PAYLOAD_TYPE_CONE:
        payload = std::make_unique<PrimitiveShapeConePayload>(PrimitiveShapeConePayload::read(in));
        break;
    default:
        throw PacketDecodeException("Unknown payload type " + std::to_string(payloadType));
    }

    return PacketShapeData{networkId,
                           shapeType,
                           std::move(location),
                           scale,
                           std::move(rotation),
                           totalTimeLeft,
                           maximumRenderDistance,
                           std::move(color),
                           dimensionId,
                           attachedToEntityId,
                           std::move(payload)};
}

void PacketShapeData::write(encoding::ByteBufferWriter &out) const
{
    VarInt::writeUnsignedLong(out, networkId);
    CommonTypes::writeOptional(out, type, [](encoding::ByteBufferWriter &out, const PrimitiveShapeType type) {
        Byte::writeUnsigned(out, static_cast<std::uint8_t>(type));
    });
    CommonTypes::writeOptional(out, location, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, scale, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    CommonTypes::writeOptional(out, rotation, [](encoding::ByteBufferWriter &out, const math::Vector3 &v) {
        CommonTypes::putVector3(out, v);
    });
    CommonTypes::writeOptional(out, totalTimeLeft, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    CommonTypes::writeOptional(out, maximumRenderDistance, [](encoding::ByteBufferWriter &out, const float v) {
        LE::writeFloat(out, v);
    });
    CommonTypes::writeOptional(out, color, [](encoding::ByteBufferWriter &out, const ::bedrock_protocol::color::Color &color) {
        LE::writeUnsignedInt(out, color.toARGB());
    });
    CommonTypes::writeOptional(out, dimensionId, [](encoding::ByteBufferWriter &out, const std::int32_t dimensionId) {
        VarInt::writeSignedInt(out, dimensionId);
    });
    CommonTypes::writeOptional(out, attachedToEntityId,
                               [](encoding::ByteBufferWriter &out, const std::uint64_t entityId) {
                                   CommonTypes::putActorRuntimeId(out, entityId);
                               });

    VarInt::writeUnsignedInt(out, static_cast<std::uint32_t>(
                                      payload != nullptr ? payload->getTypeId()
                                                         : PrimitiveShapePayloadType::PAYLOAD_TYPE_NONE));
    if (payload != nullptr) {
        payload->write(out);
    }
}

}  // namespace bedrock_protocol::types::shape
