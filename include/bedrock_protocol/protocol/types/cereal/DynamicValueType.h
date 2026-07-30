/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: src/types/cereal/DynamicValueType.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <cstdint>

namespace bedrock_protocol::types::cereal {

/**
 * These values aren't present in the spec.
 * As of 1.26.30, these were obtained from BDS symbols for the following types:
 *
 * Bedrock::DDUI::DataStoreChange
 * cereal::DynamicValue
 *
 * The positions of the types in the std::variant used by cereal::DynamicValue are the values of this enum.
 *
 * Since cereal::DynamicValue appears non-specific to DDUI, it's possible this type may appear elsewhere in the protocol
 * in the future.
 */
class DynamicValueType final {
public:
    DynamicValueType() = delete;

    static constexpr std::uint32_t NULL_ = 0;  // renamed from PHP `NULL` - reserved word in C++
    static constexpr std::uint32_t BOOL = 1;
    static constexpr std::uint32_t LONG = 2;
    static constexpr std::uint32_t DOUBLE = 3;
    static constexpr std::uint32_t STRING = 4;
    static constexpr std::uint32_t LIST = 5;
    static constexpr std::uint32_t MAP = 6;
};

}  // namespace bedrock_protocol::types::cereal
