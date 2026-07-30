/*
 * This file is part of BedrockProtocol for Endstone.
 * C++ port of the PHP original: src/types/Experiments.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include <string>
#include <utility>
#include <vector>

#include "bedrock_protocol/encoding/ByteBufferReader.h"
#include "bedrock_protocol/encoding/ByteBufferWriter.h"

namespace bedrock_protocol::types {

class Experiments final {
public:
    /**
     * @note experiments is an ordered map of experiment name => enabled; the encoding depends on the
     *       iteration order, so it is stored as a vector of pairs rather than a hash map.
     */
    /** Not present in the PHP original; needed because LevelSettings::read() default-constructs the owner first. */
    Experiments() = default;

    Experiments(std::vector<std::pair<std::string, bool>> experiments, bool hasPreviouslyUsedExperiments)
        : experiments(std::move(experiments)), hasPreviouslyUsedExperiments_(hasPreviouslyUsedExperiments)
    {
    }

    [[nodiscard]] const std::vector<std::pair<std::string, bool>> &getExperiments() const { return experiments; }

    [[nodiscard]] bool hasPreviouslyUsedExperiments() const { return hasPreviouslyUsedExperiments_; }

    /** @throws DataDecodeException */
    static Experiments read(encoding::ByteBufferReader &in);

    void write(encoding::ByteBufferWriter &out) const;

private:
    std::vector<std::pair<std::string, bool>> experiments;
    /** Named with a trailing underscore only because hasPreviouslyUsedExperiments() is the getter's name. */
    bool hasPreviouslyUsedExperiments_ = false;
};

}  // namespace bedrock_protocol::types
