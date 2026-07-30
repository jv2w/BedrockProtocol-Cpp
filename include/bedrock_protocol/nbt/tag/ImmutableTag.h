/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/tag/ImmutableTag.php
 * Copyright (C) 2014-2022 PocketMine Team <https://github.com/pmmp/BedrockProtocol>
 *
 * BedrockProtocol is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#pragma once

#include "bedrock_protocol/nbt/tag/Tag.h"

namespace bedrock_protocol::nbt::tag {

/**
 * Marker base class for tags whose value can never change after construction.
 *
 * Deviation from PHP: PHP's ImmutableTag implements makeCopy() as `return $this` (immutable types
 * don't need to be copied) because tags are reference types there. C++ tags are owned values, so
 * subclasses still have to produce a real copy in clone(); the class remains as a hierarchy marker.
 */
class ImmutableTag : public Tag {};

}  // namespace bedrock_protocol::nbt::tag
