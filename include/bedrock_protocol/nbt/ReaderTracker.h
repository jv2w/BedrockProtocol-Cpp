/*
 * This file is part of BedrockProtocol-Cpp.
 * C++ port of the PHP original: pocketmine/nbt src/ReaderTracker.php
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

#include "bedrock_protocol/nbt/NbtDataException.h"

namespace bedrock_protocol::nbt {

class ReaderTracker {
public:
    explicit ReaderTracker(int maxDepth) : maxDepth(maxDepth) {}

    /**
     * @throws NbtDataException if the recursion depth is too deep
     */
    template <typename Closure>
    void protectDepth(Closure &&execute)
    {
        if (maxDepth > 0 && ++currentDepth > maxDepth) {
            --currentDepth;
            throw NbtDataException("Nesting level too deep: reached max depth of " + std::to_string(maxDepth) +
                                   " tags");
        }
        // The decrement MUST be done by a destructor, not by `catch (...) { --currentDepth; throw; }`.
        //
        // protectDepth nests once per NBT level, so a catch-and-rethrow here puts an exception handler
        // on all `maxDepth` frames. When the depth limit finally fires, unwinding then performs a
        // *rethrow* at every one of those frames, and each rethrow runs the throw machinery on top of
        // the stack it has not finished unwinding. Measured cost was over 8 KB of stack per level: with
        // maxDepth 512 the limit check fired correctly and the resulting unwind still overflowed a 4 MB
        // stack, killing the process. A ~1 KB payload of nested TAG_Compound aimed at any NBT-carrying
        // packet was a remote kill of the server.
        //
        // A destructor is run by the unwinder itself and adds no frames, so the unwind is now flat.
        DepthGuard guard(currentDepth);
        std::forward<Closure>(execute)();
    }

private:
    /** Restores the depth counter on both the normal and the exceptional path, without a handler. */
    class DepthGuard {
    public:
        explicit DepthGuard(int &depth) noexcept : depth_(depth) {}
        ~DepthGuard() { --depth_; }
        DepthGuard(const DepthGuard &) = delete;
        DepthGuard &operator=(const DepthGuard &) = delete;

    private:
        int &depth_;
    };

    int maxDepth;
    int currentDepth = 0;
};

}  // namespace bedrock_protocol::nbt
