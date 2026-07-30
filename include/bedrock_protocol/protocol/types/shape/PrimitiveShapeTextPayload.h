/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/shape/PrimitiveShapeTextPayload.php
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
#include <utility>

#include "bedrock_protocol/color/Color.h"
#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapePayload.h"
#include "bedrock_protocol/protocol/types/shape/PrimitiveShapeType.h"

namespace bedrock_protocol::types::shape {

class PrimitiveShapeTextPayload final : public PrimitiveShapePayload {
public:
    static constexpr std::int32_t ID = PrimitiveShapePayloadType::PAYLOAD_TYPE_TEXT;

    PrimitiveShapeTextPayload(std::string text, const bool useRotation,
                              const std::optional<color::Color> &backgroundColor, const bool depthTest,
                              const bool showBackface, const bool showTextBackface)
        : text(std::move(text)), useRotation_(useRotation), backgroundColor(backgroundColor), depthTest(depthTest),
          showBackface(showBackface), showTextBackface(showTextBackface)
    {
    }

    [[nodiscard]] std::int32_t getTypeId() const override { return ID; }

    [[nodiscard]] const std::string &getText() const { return text; }

    [[nodiscard]] bool useRotation() const { return useRotation_; }

    [[nodiscard]] const std::optional<color::Color> &getBackgroundColor() const { return backgroundColor; }

    [[nodiscard]] bool hasDepthTest() const { return depthTest; }

    [[nodiscard]] bool hasShowBackface() const { return showBackface; }

    [[nodiscard]] bool hasShowTextBackface() const { return showTextBackface; }

    /**
     * @throws DataDecodeException
     */
    static PrimitiveShapeTextPayload read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const override;

    [[nodiscard]] std::unique_ptr<PrimitiveShapePayload> clone() const override
    {
        return std::make_unique<PrimitiveShapeTextPayload>(*this);
    }

private:
    std::string text;
    /** PHP's $useRotation; renamed to avoid clashing with the useRotation() accessor. */
    bool useRotation_;
    std::optional<color::Color> backgroundColor;
    bool depthTest;
    bool showBackface;
    bool showTextBackface;
};

}  // namespace bedrock_protocol::types::shape
