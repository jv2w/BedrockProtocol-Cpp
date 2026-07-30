/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/camera/CameraSetInstructionEaseType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "bedrock_protocol/protocol/types/camera/CameraSetInstructionEaseType.h"

#include <array>
#include <stdexcept>
#include <utility>

namespace bedrock_protocol::types::camera {
namespace {

using EaseTypeName = std::pair<std::uint8_t, std::string_view>;

constexpr std::array<EaseTypeName, 32> EASE_TYPE_NAMES = {{
    {CameraSetInstructionEaseType::LINEAR, "linear"},
    {CameraSetInstructionEaseType::SPRING, "spring"},
    {CameraSetInstructionEaseType::IN_SINE, "in_sine"},
    {CameraSetInstructionEaseType::OUT_SINE, "out_sine"},
    {CameraSetInstructionEaseType::IN_OUT_SINE, "in_out_sine"},
    {CameraSetInstructionEaseType::IN_QUAD, "in_quad"},
    {CameraSetInstructionEaseType::OUT_QUAD, "out_quad"},
    {CameraSetInstructionEaseType::IN_OUT_QUAD, "in_out_quad"},
    {CameraSetInstructionEaseType::IN_CUBIC, "in_cubic"},
    {CameraSetInstructionEaseType::OUT_CUBIC, "out_cubic"},
    {CameraSetInstructionEaseType::IN_OUT_CUBIC, "in_out_cubic"},
    {CameraSetInstructionEaseType::IN_QUART, "in_quart"},
    {CameraSetInstructionEaseType::OUT_QUART, "out_quart"},
    {CameraSetInstructionEaseType::IN_OUT_QUART, "in_out_quart"},
    {CameraSetInstructionEaseType::IN_QUINT, "in_quint"},
    {CameraSetInstructionEaseType::OUT_QUINT, "out_quint"},
    {CameraSetInstructionEaseType::IN_OUT_QUINT, "in_out_quint"},
    {CameraSetInstructionEaseType::IN_EXPO, "in_expo"},
    {CameraSetInstructionEaseType::OUT_EXPO, "out_expo"},
    {CameraSetInstructionEaseType::IN_OUT_EXPO, "in_out_expo"},
    {CameraSetInstructionEaseType::IN_CIRC, "in_circ"},
    {CameraSetInstructionEaseType::OUT_CIRC, "out_circ"},
    {CameraSetInstructionEaseType::IN_OUT_CIRC, "in_out_circ"},
    {CameraSetInstructionEaseType::IN_BACK, "in_back"},
    {CameraSetInstructionEaseType::OUT_BACK, "out_back"},
    {CameraSetInstructionEaseType::IN_OUT_BACK, "in_out_back"},
    {CameraSetInstructionEaseType::IN_ELASTIC, "in_elastic"},
    {CameraSetInstructionEaseType::OUT_ELASTIC, "out_elastic"},
    {CameraSetInstructionEaseType::IN_OUT_ELASTIC, "in_out_elastic"},
    {CameraSetInstructionEaseType::IN_BOUNCE, "in_bounce"},
    {CameraSetInstructionEaseType::OUT_BOUNCE, "out_bounce"},
    {CameraSetInstructionEaseType::IN_OUT_BOUNCE, "in_out_bounce"},
}};

}  // namespace

std::string CameraSetInstructionEaseType::toName(std::uint8_t value)
{
    for (const auto &[rawValue, name] : EASE_TYPE_NAMES) {
        if (rawValue == value) {
            return std::string(name);
        }
    }

    throw std::invalid_argument("Invalid raw value \"" + std::to_string(value) + "\" for EaseType.");
}

std::uint8_t CameraSetInstructionEaseType::fromName(std::string_view name)
{
    for (const auto &[rawValue, candidate] : EASE_TYPE_NAMES) {
        if (candidate == name) {
            return rawValue;
        }
    }

    throw std::invalid_argument("Invalid raw value \"" + std::string(name) + "\" for EaseType.");
}

}  // namespace bedrock_protocol::types::camera
